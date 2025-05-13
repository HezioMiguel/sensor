class SensorMQ135 {
private:
    int pinoSensor;

public:
    SensorMQ135(int pino) {
        pinoSensor = pino;
    }

    void iniciar() {
        pinMode(pinoSensor, INPUT);  // Defina o pino como entrada
    }

    int lerValor() {
        return analogRead(pinoSensor);  // Lê o valor analógico do sensoe
    }
};

// Defina o pino do buzzer
const int pinoBuzzer = 8;  // Conecta o buzzer ao pino digital do arduino 8

SensorMQ135 sensor(A0);  // Conecte o sensor no pino A0

void setup() {
    Serial.begin(9600);  // Inicia a comunicação serial com o ESP8266
    sensor.iniciar();  // Inicializa o sensor
    pinMode(pinoBuzzer, OUTPUT);  // Inicializa o pino do buzzer
}

void loop() {
    int valor = sensor.lerValor();  // Lê o valor do sensor
    Serial.println(valor);  // Envia o valor para o ESP8266 via serial

    // Controle do buzzer baseado no valor do sensor MQ135
    if (valor >= 100 && valor < 300) {
        // Nível moderado - bip lento
        tone(pinoBuzzer, 1000);  
        delay(300);
        noTone(pinoBuzzer);
        delay(700);
    }
    else if (valor >= 300) {
        // Nível alto - bip rápido
        tone(pinoBuzzer, 1000);
        delay(100);
        noTone(pinoBuzzer);
        delay(100);
    }
    else {
        // Nível baixo - sem som
        noTone(pinoBuzzer);
        delay(1000);
    }
}
