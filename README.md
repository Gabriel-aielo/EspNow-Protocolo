# EspNow-Protocolo

Este repositório consiste na aplicação do Esp-Now em um projeto de faculdade. Aqui está estruturada a comunicação Master - Slave, transferindo os dados de um sensor de umidade, temperatura e dados de uma célula de carga. 

Até o momento a ESP32 Master está configurada para realizar a requisição das ESP´s slave, e enviar os dados para um Web servidor local (web server dentro da ESP).

O objetivo final deste projeto é enviar os dados via comunicação MQTT para o node-red, utilizando arquivos JSON, e tratar estes dados numa dashboard. 

O trabalho esta sendo desenvolvido em equipe, no Intituto Federal de São Paulo - Campus Guarulhos. Este projeto faz parte do programa industria 4.0, um projeto incentivado pelo Governo Federal;.

