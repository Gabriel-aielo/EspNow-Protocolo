#include <esp_now.h>
#include <WiFi.h>              

const int ledConfirmacao = 2;

const uint8_t macAddrMestre1[] = { 0x94, 0xE6, 0x86, 0x3D, 0x6D, 0x1C }; // Endereço MAC do dispositivo mestre 1. Coloque o endereço MAC de sua placa aqui
const uint8_t macAddrMestre2[] = { 0xEC, 0x62, 0x60, 0x93, 0xC8, 0xBC }; // Endereço MAC do dispositivo mestre 2. Coloque o endereço MAC de sua placa aqui
const uint8_t slaveMacAddress[] = { 0x24, 0x6F, 0x28, 0xB6, 0x05, 0x68 };

//dados em relação a esp1; relação ao código master 2
typedef struct {
  char id[5];
  float temperaturaEnviada1;
  float umidadeEnviada1;
} dadosEsp1;
  //guarda os dados de temperatura e umidade para serem enviados
  dadosEsp1 dadosSensor1;

//dados em relação com a esp2; relação ao código master
typedef struct {
  char id[5];
  float temperaturaEnviada2;
  float umidadeEnviada2;
} dadosEsp2;  

//guarda os dados de temperatura e umidade para serem enviados
dadosEsp2 dadosSensor2;


void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
 
  dadosEsp1* dadosSensor1 = (dadosEsp1*)incomingData;
  dadosEsp2* dadosSensor2 = (dadosEsp2*)incomingData;

  char confere1[5] = "ESP1";
  char confere2[5] = "ESP2";

  char valorrecebido[30];

  strcpy(valorrecebido, dadosSensor1->id);

    if (strcmp(valorrecebido, confere1) == 0) {
      Serial.println("-----------------------------------------------------------");
      Serial.println("DADOS SENSOR 1");
      Serial.println(dadosSensor1->id);
      Serial.print(dadosSensor1->umidadeEnviada1);
      Serial.println("%");
      Serial.print(dadosSensor1->temperaturaEnviada1);
      Serial.println("ºC");
      Serial.println("-----------------------------------------------------------");

    } else if (strcmp(valorrecebido, confere2) == 0) {
      Serial.println("-----------------------------------------------------------");
      Serial.println("DADOS SENSOR 2");
      Serial.print(dadosSensor2->umidadeEnviada2);
      Serial.println("%");
      Serial.print(dadosSensor2->temperaturaEnviada2);
      Serial.println("ºC");
      Serial.println("-----------------------------------------------------------");
  }

}


void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(ledConfirmacao, OUTPUT);

  WiFi.disconnect();    // Desconecta de qualquer rede WiFi previamente conectada
  WiFi.mode(WIFI_STA);  // Define o modo WiFi como Station (cliente)
  Serial.begin(115200);
  Serial.println(WiFi.macAddress());

  if (esp_now_init() == ESP_OK) {   // Inicializa o ESP-NOW e verifica se há erros
  digitalWrite(ledConfirmacao, 1);
                 
  } else if(esp_now_init() != ESP_OK ){
  digitalWrite(ledConfirmacao, 1);
  delay(500);
  digitalWrite(ledConfirmacao, 0);
  delay(500);
  
     ESP.restart();  
  }

   esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  delay(1);  // Delay de 1 milissegundo
}