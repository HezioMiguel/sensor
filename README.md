# Sistema Dual-Node de Monitoramento de Qualidade do Ar (IoT)

Este projeto consiste em um sistema de Internet das Coisas (IoT) focado na detecção e monitoramento de gases nocivos em tempo real. A arquitetura foi dividida em dois microcontroladores distintos para separar a carga de processamento de hardware da carga de processamento de rede.

## Arquitetura do Sistema

O projeto utiliza o padrão Sensor Node + Gateway:

1. Nó de Hardware (Arduino / ATmega328P):
   - Responsável exclusivamente por interagir com o mundo físico em tempo real.
   - Realiza a leitura analógica contínua do sensor MQ-135.
   - Executa a lógica de segurança local (acionamento de Buzzer em diferentes frequências dependendo do nível de perigo), garantindo que o alarme toque instantaneamente mesmo em caso de falha na rede Wi-Fi.
   - Transmite os dados processados via protocolo Serial (UART) para o módulo de rede.

2. Nó de Rede (ESP8266):
   - Atua como Gateway e Web Server embutido.
   - Recebe os dados via comunicação Serial assíncrona do Arduino.
   - Hospeda uma interface Web (HTML/CSS/JS) diretamente em sua memória flash.
   - Utiliza requisições HTTP assíncronas (API Fetch) no frontend para atualizar a leitura do sensor no navegador do usuário a cada 1 segundo, operando como uma Single Page Application (SPA).

## Tecnologias e Componentes
- Linguagem: C++ (Orientação a Objetos) e JavaScript/HTML.
- Microcontroladores: ESP8266 (NodeMCU/Wemos) e Arduino Uno/Nano.
- Sensor: MQ-135 (Sensor de Qualidade do Ar).
- Atuador: Buzzer ativo para alarmes de hardware.
- Protocolos: HTTP, TCP/IP, UART.

## Regras de Negócio e Alertas

O sistema classifica a qualidade do ar em três faixas operacionais, refletidas tanto no hardware quanto na interface web:

- 0 a 99 (Ar Limpo): Ambiente seguro. Interface web exibe cor indicativa de normalidade (verde). Hardware permanece silencioso.
- 100 a 299 (Moderado): Presença detectada de impurezas no ar. Interface web exibe estado de atenção (amarelo). O hardware emite bipes espaçados (intervalos de 700ms) para alertar os presentes.
- 300+ (Poluído/Crítico): Nível de toxicidade elevado. Risco à saúde. Interface web exibe estado de emergência (vermelho). O hardware entra em modo de alarme contínuo com bipes rápidos (intervalos de 100ms) sinalizando necessidade de evacuação ou ventilação imediata.

## Como Reproduzir o Projeto

1. Configuração de Hardware:
   - Conecte o pino analógico do sensor MQ-135 ao pino A0 do Arduino.
   - Conecte o sinal do Buzzer ao pino digital 8 do Arduino.
   - Estabeleça a comunicação Serial conectando o pino TX do Arduino ao pino RX do ESP8266 (recomenda-se o uso de um divisor de tensão ou conversor lógico de nível, dado que o ESP8266 opera em 3.3V e o Arduino em 5V).
   - Unifique o GND de todos os componentes e placas.

2. Configuração de Software:
   - Compile e faça o upload do código referente ao sensor para o Arduino (ATmega328P).
   - No código referente ao servidor web, insira as credenciais da rede Wi-Fi (SSID e Senha) nas constantes globais.
   - Compile e faça o upload do código do servidor para o ESP8266.
   - Abra o Monitor Serial conectado ao ESP8266 para visualizar o endereço IP local atribuído pelo roteador.
   - Acesse o endereço IP via navegador de internet em qualquer dispositivo conectado à mesma rede local.
