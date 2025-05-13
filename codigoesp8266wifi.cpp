#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// CONFIGURAÇÃO DO NOME DO WIFI E DA SENHA 
const char* nomeDaRede = "Cyber Hezio";
const char* senhaDaRede = "08052005";

// CLASSE DO SENSOR QUE MEDE OS GASES NOCIVOS
class MQ135 {
private:
    int valorAtual;

public:
    void iniciar() {
        // Nada necessário aqui agora
    }

    void atualizarValor() {
        if (Serial.available()) {
            String leitura = Serial.readStringUntil('\n');
            valorAtual = leitura.toInt();  // Converte e armazena o valor
        }
    }

    String pegaValor() {
        return String(valorAtual);  // Retorna como string
    }
};

// CLASSE DO SERVIDOR WEB
class ServidorMQ135 {
private:
    ESP8266WebServer server;
    MQ135* sensor;

    const char* html = R"rawliteral(
    <!DOCTYPE html>
    <html lang="pt-BR">
    <head>
        <meta charset="UTF-8">
        <title>Qualidade do Ar</title>
        <style>

            body.bom { background: #008000; color: #000000; }
            body.media { background: #ffdb58; color: #000000; }
            body.ruim { background: #ff0000; color: #000000; }

            body {
                transition: background 0.5s ease;
                background: #000000;
                font-family: Arial, sans-serif;
                text-align: center;
                padding: 30px;
                color: white;
            }
            h1 { margin-bottom: 20px; }
            #valor { font-size: 4rem; margin: 20px 0; }
            #status {
                font-size: 2.5rem;
                padding: 20px 40px;
                border-radius: 12px;
                display: inline-block;
                background: rgba(0,0,0,0.3)
            }
            
        </style>
    </head>
    <body>
        <h1>Qualidade do Ar</h1>
        <div id="valor">--</div>
        <div id="status">Carregando...</div>
        <script>
        setInterval(() => {
            fetch("/valor")
            .then(resp => resp.text())
            .then(data => {
                const val = parseInt(data);
                document.getElementById("valor").textContent = val;
                const status = document.getElementById("status");
                status.className = "";
                document.body.classList.remove("bom", "media", "ruim");
                if (val < 100) {
                    status.textContent = "Ar Limpo";
                    document.body.classList.add("bom");
                } else if (val < 300) {
                    status.textContent = "Moderado";
                    document.body.classList.add("media");
                } else {
                    status.textContent = "Poluído";
                    document.body.classList.add("ruim");
                }
            });
        }, 1000);
        </script>
    
<ul style="margin-top: 30px; font-size: 1.2rem; text-align: left; display: inline-block;">

    <li><strong>0–99:</strong> Ar Limpo : ar sem impuresas, proprio para o ambiente.</li> 
    <li><strong>100–299:</strong> Moderado: apresenta impuresas e pode causar irritações,dor de cabeça, tontura, entre outros encomodos.</li> 
    <li><strong>300+:</strong> Poluído: Tu morre.</li>
</ul>


    </body>
    </html>
    )rawliteral";

public:
    ServidorMQ135(MQ135* s) : server(80), sensor(s) {}

    void iniciar() {
        server.on("/", [this]() {
            server.send(200, "text/html", html);
        });

        server.on("/valor", [this]() {
            server.send(200, "text/plain", sensor->pegaValor());
        });

        server.begin();
        Serial.println("Servidor iniciado na porta 80");
    }

    void atualizar() {
        server.handleClient();
    }
};

// Instâncias globais
MQ135 sensorMQ;
ServidorMQ135 servidor(&sensorMQ);

void setup() {
    Serial.begin(9600); // Comunicação com ATmega328P
    sensorMQ.iniciar();

    WiFi.begin(nomeDaRede, senhaDaRede);
    Serial.println("Conectando-se à rede Wi-Fi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nConectado com sucesso!");
    servidor.iniciar();
}


void loop() {
    sensorMQ.atualizarValor();  // Recebe valor do ATmega
    servidor.atualizar();             // Atualiza servidor web
}
