#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* nomerede = "Cyber Hezio";
const char* senharede = "08052005";

ESP8266WebServer server(80);
String valorMQ = "0";  // valor recebido do Mega

const char* html = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
  <meta charset="UTF-8">
  <title>MQ135 - Qualidade do Ar</title>
  <style>
    body {
      background: #00000;
      color: #ffffff
      font-family: Arial, sans-serif;
      text-align: center;
      padding: 30px;
    }
    h1 {
      margin-bottom: 20px;
    }
    #valor {
      font-size: 4rem;
      margin: 20px 0;
    }
    #status {
      font-size: 1.5rem;
      padding: 10px;
      border-radius: 8px;
      display: inline-block;
    }
    .bom { background: #green; color: #gold; }
    .moderado { background: #yellow; color: #000000; }
    .ruim { background: #ff0000; color: #000000; }
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
          if (val < 100) {
            status.textContent = "Ar Limpo";
            status.classList.add("bom");
          } else if (val < 300) {
            status.textContent = "Moderado";
            status.classList.add("moderado");
          } else {
            status.textContent = "Poluído";
            status.classList.add("ruim");
          }
        });
    }, 1000);
  </script>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(9600);  // Lê os dados do Mega
  WiFi.begin(nomerede, senharede);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  server.on("/", []() {
    server.send(200, "text/html", html);
  });

  server.on("/valor", []() {
    server.send(200, "text/plain", valorMQ);
  });

  server.begin();
}

void loop() {
  if (Serial.available()) {
    valorMQ = Serial.readStringUntil('\n');
    valorMQ.trim();  // remove espaços em branco
  }

  server.handleClient();
}
