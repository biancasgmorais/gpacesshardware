#include <SPI.h> //biblioteca serial
#include <MFRC522.h> //biblioteca do RFID
#include <Wire.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
 
// Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);

#define SS_PIN 10
#define RST_PIN 9

//Pino ligado ao pino IN1 do modulo
int porta_rele1 = 7;

SoftwareSerial ArduinoUno(5,6);
MFRC522 mfrc522(SS_PIN, RST_PIN); //cria a instancia da biblioteca

int st;
String data;

void setup() {

 Serial.begin(9600);
 ArduinoUno.begin(9600);
 
 SPI.begin();
 mfrc522.PCD_Init(); //inicia MFRC522
 Serial.println("Aproxime o cartao do leitor!");
 Serial.println();
 
 //Define o número de colunas e linhas do LCD:
 lcd.begin(16,2);
 
 pinMode(porta_rele1, OUTPUT);
 digitalWrite(porta_rele1, HIGH);
}


void loop() {
  
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Aproxime o ");
   lcd.setCursor(0,1); 
   lcd.print("cartao do leitor");
   digitalWrite(porta_rele1, HIGH);
   
  //Procura por novas tags
  if( ! mfrc522.PICC_IsNewCardPresent()){
    return;
  }
  //Seleciona um cartão
  if( ! mfrc522.PICC_ReadCardSerial()){
    return;
  }
  
  //UID da tag na porta serial
  Serial.print("UID da tag: ");
  String conteudo;
  String conteudo2 = "";
  
  //Descodificando o ID da tag
  for(byte i = 0; i<mfrc522.uid.size; i++){
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  Serial.println();

  //Evita que ao manter a tag no sensor ele envie a mesma tag varias vezes  
  while (conteudo != conteudo2){

     conteudo2 = conteudo;

    //transforma a string em char para enviar pela porta serial
    char* pt1 = (char*) malloc(sizeof(char)*conteudo.length()+1);
    conteudo.toCharArray(pt1, conteudo.length() + 1);
    
    //escreve na porta serial o char*
    ArduinoUno.write(pt1);
    Serial.print(pt1);
    Serial.println();

    //recebe o resultado da busca no banco
    delay(5000);
    st = ArduinoUno.read();
    Serial.println(st);
      

      if(st == 15){
        
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Acesso Liberado");
        
        delay(200);
        
        digitalWrite(porta_rele1, LOW);  //Liga rele 1
        delay(500);
        digitalWrite(porta_rele1, HIGH);  //Desliga rele 1
  
        Serial.println();

      }

       if(st == 10){
        
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Acesso Negado");
  
        delay(200); 
        
        Serial.println();

       }
  }
}
