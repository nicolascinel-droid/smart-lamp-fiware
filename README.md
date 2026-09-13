# Smart Lamp - FIWARE 💡

Projeto acadêmico desenvolvido para a disciplina de Edge Computing da FIAP.

O projeto consiste em uma Smart Lamp utilizando ESP32, sensor LDR, MQTT e a plataforma FIWARE. O sistema permite enviar dados de luminosidade do ESP32 para o FIWARE e controlar o LED do ESP32 remotamente através de comandos enviados pelo Postman.

## 👨‍💻 Autores

- Nicolas Cinel
- Luis Fernando
- Kelvin Lucas
- Leonardo Formigari

## 🛠️ Tecnologias utilizadas

- ESP32 DEVKIT V1
- Sensor LDR
- FIWARE
- Orion Context Broker
- IoT Agent MQTT
- Mosquitto MQTT
- Postman
- Wokwi
- AWS EC2
- Arduino / C++

## 💡 Funcionamento

O ESP32 realiza a leitura da luminosidade através do sensor LDR e envia os valores para o FIWARE utilizando MQTT.

Também é possível realizar a comunicação no sentido FIWARE → ESP32. Através do Postman, são enviados os comandos `on` e `off`, permitindo ligar e desligar o LED onboard do ESP32.

## 📡 Entidade utilizada

Device ID:

`lamp002`

Entity:

`urn:ngsi-ld:Lamp:002`

## 🔗 Simulação no Wokwi

https://wokwi.com/projects/475087263137443841

## ☁️ FIWARE

O FIWARE foi configurado em uma instância EC2 na AWS utilizando o projeto FIWARE Descomplicado.

Principais componentes utilizados:

- Orion Context Broker
- IoT Agent MQTT
- Mosquitto
- MongoDB
- STH-Comet

## 🔄 Comunicação

### ESP32 → FIWARE

O ESP32 envia:

- Estado do LED
- Valor de luminosidade do sensor LDR

### FIWARE → ESP32

Através do Postman são enviados os comandos:

- `on` → Liga o LED
- `off` → Desliga o LED

## 📁 Código

O código principal do projeto está disponível no arquivo:

`smart_lamp.ino`

O código contém comentários explicando as principais funções utilizadas no projeto.
