//Esp I
#include <esp_now.h>    
#include <WiFi.h>              
#include "DHT.h"

#define DHTTYPE DHT11   // DHT 11
#define ID "ESP1"
uint8_t DHTPin = 4; 
DHT dht(DHTPin, DHTTYPE); 
float temperatura;
float umidade;

const int ledConfirmacao = 2;

const uint8_t slaveMacAddress[] = { 0x24, 0x6F, 0x28, 0xB6, 0x05, 0x68 };  // Define o endereço MAC do dispositivo escravo. Coloque o endereço MAC de sua placa aqui

typedef struct {
  char id[5] = "ESP1";
  float temperaturaEnviada1;
  float umidadeEnviada1;
} dadosEsp1;

  //guarda os dados de temperatura e umidade para serem enviados
  dadosEsp1 dadosSensor1;

esp_now_peer_info_t peerInfo;  // Cria uma estrutura esp_now_peer_info_t, que é utilizada para registrar informações sobre um peer (dispositivo) que será adicionado à rede ESPNOW

// Função que é chamada quando dados são recebidos via ESP-NOW

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
} 

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(ledConfirmacao, OUTPUT);

  pinMode(DHTPin, INPUT);
  dht.begin(); 

  // Desconecta de alguma conexão WiFi anterior e define o modo estação (STA)
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);

  // Inicia a biblioteca ESP-NOW e, caso ocorra algum erro, reinicia o dispositivo
 if (esp_now_init() == ESP_OK) {   // Inicializa o ESP-NOW e verifica se há erros
  digitalWrite(ledConfirmacao, 1);
                 
  } else if(esp_now_init() != ESP_OK ){
  digitalWrite(ledConfirmacao, 1);
  delay(500);
  digitalWrite(ledConfirmacao, 0);
  delay(500);
  
     ESP.restart();  
  }

  esp_now_register_send_cb(OnDataSent);  // Registra a função de callback que é chamada quando os dados são enviados

//registrando o escravo
  peerInfo.channel = 0;                            // Define o canal de comunicação como 0 na estrutura peerInfo
  peerInfo.encrypt = false;                        // Define a encriptação como desativada na estrutura peerInfo
  memcpy(peerInfo.peer_addr, slaveMacAddress, 6);  // Copia o endereço MAC do escravo para a estrutura peerInfo

//adicionando o escravo
  if (esp_now_add_peer(&peerInfo) == ESP_OK) {  // Caso não seja possível adicionar o escravo, exibe mensagem de falha no display e reinicia o dispositivo
    digitalWrite(ledConfirmacao,1);

  } else if(esp_now_add_peer(&peerInfo) != ESP_OK){
      digitalWrite(ledConfirmacao, 1);
      delay(500);
      digitalWrite(ledConfirmacao, 0);
      delay(500);
  
     ESP.restart(); 
  }
}

//leitura de daods do  sensor
void leituraDeDados() {

  temperatura = dht.readTemperature(); // Obtém os valores da temperatura
 umidade = dht.readHumidity(); // Obtém os valores da umidade

  //Se a leitura foi bem sucedida

    //Guarda na struct a id e os valores da umidade e da temperatura
    strcpy(dadosSensor1.id, ID);
    dadosSensor1.umidadeEnviada1 = umidade;
    dadosSensor1.temperaturaEnviada1 = temperatura;
  

 // temperatura = dht.readTemperature(); // Obtém os valores da temperatura
  //umidade = dht.readHumidity(); // Obtém os valores da umidade
}

//envio dos dados do sensor
void envioDadosSensor(){
  //Envia a struct para o gateway
  esp_err_t result = esp_now_send(slaveMacAddress, (uint8_t *) &dadosSensor1, sizeof(dadosSensor1));
  Serial.print("Send Status: ");
  //Se o envio foi bem sucedido
  if (result == ESP_OK) {
    Serial.println("Success");
  }
  //Se aconteceu algum erro no envio
  else {
    Serial.println("Error");
  }
}

void loop() {

  leituraDeDados();

  envioDadosSensor();

  Serial.println("-----------------------------------------------------------");
  Serial.println("-----------------------------------------------------------");
  Serial.print(umidade);
  Serial.println("%");
  Serial.print(temperatura);
  Serial.println("ºC");
  Serial.println("-----------------------------------------------------------");
  Serial.println( dadosSensor1.id);
  Serial.println("-----------------------------------------------------------");
  Serial.println("-----------------------------------------------------------");
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(slaveMacAddress, (uint8_t *) &dadosSensor1, sizeof(dadosSensor1));
   
  if (result == ESP_OK) {
    Serial.println("Dados enviados");
  }
  else {
    Serial.println("Falha na conexão com o escravo");
  }
  delay(10000);
}

