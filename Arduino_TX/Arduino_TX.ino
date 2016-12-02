/*
  Leitura e Publicação dos dados dos Sensores
  Autores: José Adriano Filho
  Data: Novembro/2016

  Este é o programa de leitura de tempertura (do sensor DHT22).
  O programa le a temperatura em um periodo de 10 segundos e mostra,
  no Serial Monitor, a temperatura lida.
*/

#include<stdlib.h>
#include "DHT.h"
#include <SoftwareSerial.h> // Biblioteca SoftwareSerial

//defines gerais
#define DHTPIN                4      //pino do Arduino que está conectado ao pino de dados do sensor
#define DHTTYPE               DHT22  //estabelece que o sensor utilizado é o DHT22 (é possível selecionar entre DHT11, DHT22 e DHT21)
#define TEMPO_LEITURA_SENSOR  10000  //tempo entre leituras de temperatura

//variáveis globais
DHT dht(DHTPIN, DHTTYPE);   //objeto para comunicar com sensor de temperatura DHT22
SoftwareSerial comunicacaoESP(2, 3); // objeto para comunicação serial com o ESP8266 nas Portas 2(RX) e 3(TX)

//Definição de variáveis
int UVOUT = A0; 
int REF_3V3 = A1; 
int particula = 8;
unsigned long duracao;
unsigned long tempoAmostra_ms = 30000;
unsigned long pulsoBaixoOcupado = 0;
float compara = 0;
float concentracao = 0;

//Função setup (inicializações do programa)
void setup()
{
  Serial.begin(9600);
  Serial.println("======Estacao de Controle Atmosferico - IoT======");
  comunicacaoESP.begin(19200); // inicia a comunicação serial com o ESP8266

  //inicia a comunicação com o sensor DHT22
  dht.begin();

  //Configura pinos do sensor UV
  pinMode(UVOUT, INPUT);
  pinMode(REF_3V3, INPUT);
  
  //configurar pinos do sensor de particulas
  pinMode(particula, INPUT);
}

//loop (programa principal)
void loop()
{
  String sensores = ""; 
  float TemperaturaLida;
  float UmidadeLida;
  
  //medição do material particulado
  duracao = pulseIn(particula, LOW);
  pulsoBaixoOcupado = pulsoBaixoOcupado + duracao;

  compara = pulsoBaixoOcupado/(tempoAmostra_ms*10.0);  // porcentagem (de 0 a 100%)
  concentracao = 1.1*pow(compara,3)-3.8*pow(compara,2)+520*compara+0.62; // dados do datasheet
  pulsoBaixoOcupado = 0;

  //leitura dos sensores
    char buffer[10];
    //UV
    //float uv = analogRead(uvPin);
    //uv = uv * 0.0049; //converter para V
    //uv = uv * 307; //converter para mW/m²
    //uv = uv/200; //índice UV

    int uvNivel = averageAnalogRead(UVOUT);
    int refNivel = averageAnalogRead(REF_3V3);

     //Use the 3.3V power pin as a reference to get a very accurate output value from sensor
    float outputVoltage = 3.3 / refNivel * uvNivel;

    float uvIntensidade = mapfloat(outputVoltage, 0.99, 2.8, 0.0, 15.0); //Convert the voltage to a UV intensity level
    
    //le a temperatura (em graus Celsius)
    TemperaturaLida = dht.readTemperature();
    UmidadeLida = dht.readHumidity();

    //Checa se falta alguma leitura
    if (isnan(uvIntensidade)) {
    uvIntensidade = 0.0;
    } 
    if (isnan(UmidadeLida)) {
    UmidadeLida = 0.0;
    } 
    if (isnan(TemperaturaLida)) {
    TemperaturaLida = 0.0;
    } 
    if (isnan(concentracao)) {
    concentracao = 0.0;
    } 
    
    
    //conversão dos dados lidos para strings
    String uvStr = dtostrf(uvIntensidade, 4, 1, buffer);
    uvStr.replace(" ","");
    String umidadeStr = dtostrf(UmidadeLida, 4, 1, buffer);
    umidadeStr.replace(" ","");
    String temperaturaStr = dtostrf(TemperaturaLida, 4, 1, buffer);
    temperaturaStr.replace(" ","");
    String particulaStr = dtostrf(concentracao, 4, 1, buffer);
    particulaStr.replace(" ","");
   
   sensores = uvStr + ",";
   sensores += umidadeStr + ",";
   sensores += temperaturaStr + ",";
   sensores += particulaStr;

   Serial.print("Sensores = ");
   Serial.println(sensores);
   //Envia para o ESP8266
    //comunicacaoESP.println(sensores); // envio do valor da temperatura para o módulo ESP

  delay(TEMPO_LEITURA_SENSOR);

}

//Takes an average of readings on a given pin
//Returns the average
int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return(runningValue);  
}

//The Arduino Map function but for floats
//From: http://forum.arduino.cc/index.php?topic=3922.0
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

