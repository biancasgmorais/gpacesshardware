#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <SoftwareSerial.h>

#ifndef STASSID
#define STASSID "Wz2"
#define STAPSK  "wnba18172428"
#endif

SoftwareSerial NodeMCU(D6, D5);
String data;

void setup() {
  
  Serial.begin(9600);
  NodeMCU.begin(9600);
  pinMode(D1,INPUT);
  Serial.println();
  
  //Conexão WIFI com senha e rede
  WiFi.begin(STASSID, STAPSK);

  //Enquanto não houver conexão será impresso pontos
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

}

void loop() {

  //lendo a string enviada do Arduino
  data = NodeMCU.readString();
  Serial.println("RECEBIDO: "+data);
    
 //Conexão WIFI - Aguarda a conexão
  if ((WiFi.status() == WL_CONNECTED)) {


    HTTPClient http;
    if(digitalRead(D1)==1)
    {
       Serial.println("Botão pressionado");
       
       Serial.print("Iniciando solicitação HTTP...\n");
      // Configura a URL e o servidor recebido
      int httpCode = http.begin("http://gpacessbackend.herokuapp.com/temptag"); //HTTP
      http.addHeader("Content-Type", "application/json");
      
      httpCode = http.POST("{\"tag_temp\":\""+data+"\"}");
  
      // httpCode quando da negativo é erro de solicitação
      if (httpCode > 0) {
        //O cabeçalho HTTP foi enviado e o cabeçalho de resposta do servidor foi tratado
        Serial.printf("Metodo POST... código: %d\n", httpCode);
        
        //Resposta encontrada no servidor
        if (httpCode == HTTP_CODE_OK) {
          Serial.printf("Metodo POST Funcionou"); 
        } 
       
      } else {
        Serial.printf("Metodo POST falhou, erro: %s\n", http.errorToString(httpCode).c_str());    }
        http.end();
        delay(1000);
    } 
    
    
    else {
    Serial.print("Iniciando solicitação HTTP...\n");
    // Configura a URL e o servidor recebido
    int httpCode = http.begin("http://gpacessbackend.herokuapp.com/usage"); //HTTP
    http.addHeader("Content-Type", "application/json");
    
    httpCode = http.POST("{\"tag_user\":\""+data+"\"}");

    // httpCode quando da negativo é erro de solicitação
    if (httpCode > 0) {
      //O cabeçalho HTTP foi enviado e o cabeçalho de resposta do servidor foi tratado
      Serial.printf("Metodo POST... código: %d\n", httpCode);
      
      //Resposta encontrada no servidor
      if (httpCode == HTTP_CODE_OK) {
        NodeMCU.write(15);
      } else{
        if (httpCode == HTTP_CODE_UNAUTHORIZED) {
          NodeMCU.write(10);
        }
      }
     
    } else {
      Serial.printf("Metodo POST falhou, erro: %s\n", http.errorToString(httpCode).c_str());    }

    http.end();
  }
    delay(1000);
  }


}
