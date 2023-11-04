#include <esp_now.h>
#include <WiFi.h>   
#include <WebServer.h>           

const int ledConfirmacao = 2;

const uint8_t macAddrMestre1[] = { 0x94, 0xE6, 0x86, 0x3D, 0x6D, 0x1C }; // Endereço MAC do dispositivo mestre 1. Coloque o endereço MAC de sua placa aqui
const uint8_t macAddrMestre2[] = { 0xEC, 0x62, 0x60, 0x93, 0xC8, 0xBC }; // Endereço MAC do dispositivo mestre 2. Coloque o endereço MAC de sua placa aqui

const char* ssid = "VIVOFIBRA-D591";  // Entre SSID aqui
const char* password = "UxchEdjmqy";  // Insira a senha aqui
WebServer server(80);

typedef struct struct_message {

    float temperaturaEnviada;
    float umidadeEnviada;

    float temperaturaEnviada1;
    float umidadeEnviada1;
} struct_message;

struct_message myData;

float armazenatemperatura = myData.temperaturaEnviada;
float armazenaumidade = myData.umidadeEnviada;

typedef struct dadosSensores {
  float temperatura;
  float umidade;
} dadosSensores;

dadosSensores dadosSensor1;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.println("-----------------------------------------------------------");
  Serial.println("-----------------------------------------------------------");
  Serial.println("DADOS SENSOR 1");
  Serial.print(myData.umidadeEnviada);
  Serial.println("%");
  Serial.print(myData.temperaturaEnviada);
  Serial.println("ºC");
  Serial.println("-----------------------------------------------------------");

  Serial.println("-----------------------------------------------------------");
  Serial.println("DADOS SENSOR 2");
  Serial.print(dadosSensor1.umidade);
  Serial.println("%");
  Serial.print(dadosSensor1.temperatura);
  Serial.println("ºC");
  Serial.println("-----------------------------------------------------------");
  Serial.println("-----------------------------------------------------------");
}

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(ledConfirmacao, OUTPUT);

  WiFi.disconnect();    // Desconecta de qualquer rede WiFi previamente conectada

  Serial.println("Conectando à ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
     Serial.print(".");
     digitalWrite(ledConfirmacao, 1);
     delay(500);
     digitalWrite(ledConfirmacao, 0);
     delay(500);
  }
   Serial.println("");
  Serial.println("WiFi conectado..!");
  Serial.print("IP obtido: ");  
  Serial.println(WiFi.localIP());
 // server.on("/", handle_OnConnect);
  //server.onNotFound(handle_NotFound);
  //server.begin();
  Serial.println("Servidor HTTP iniciado");
  digitalWrite(ledConfirmacao, 1);

  WiFi.mode(WIFI_STA);  // Define o modo WiFi como Station (cliente)
  Serial.begin(115200);
  Serial.println(WiFi.macAddress());

  if (esp_now_init() == ESP_OK) {   // Inicializa o ESP-NOW e verifica se há erros
  Serial.println("ESP-NOW conectado..!");
                 
  } else if(esp_now_init() != ESP_OK ){
 
  
     ESP.restart();  
  }

   esp_now_register_recv_cb(OnDataRecv);
}

void loop() {

}
/*
void handle_OnConnect() {

  server.send(200, "text/html", SendHTML((armazenatemperatura, armazenaumidade)); 
}
void handle_NotFound(){
  server.send(404, "text/plain", "Não encontrado");
}
String SendHTML(armazenatemperatura, armazenatemperatura){
  String ptr = "<!DOCTYPE html> <html lang=pt-br>\n";
  ptr +="<meta charset=utf-8>";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Boletim meteorológico ESP32</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>Boletim meteorológico ESP32</h1>\n";
  
  ptr +="<p>Temperatura: ";
  ptr +=(int)Temperaturestat;
  ptr +="°C</p>";
  ptr +="<p>Umidade: ";
  ptr +=(int)Humiditystat;
  ptr +="%</p>";
  
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}*/