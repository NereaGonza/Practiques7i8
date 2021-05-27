
#include "AudioGeneratorAAC.h"
#include "AudioOutputI2S.h"
#include "AudioFileSourcePROGMEM.h"
#include "sampleaac.h"
#include "FS.h"
#include "HTTPClient.h"
#include "SPI.h"
#include "SD.h"
#include "SPIFFS.h"

AudioFileSourcePROGMEM *in;
AudioGeneratorAAC *aac;
AudioOutputI2S *out;

void setup(){
  Serial.begin(115200);
  in = new AudioFileSourcePROGMEM(sampleaac, sizeof(sampleaac)); //implir una variaable que enviarem per reporduir
  aac = new AudioGeneratorAAC();
  out = new AudioOutputI2S();
  out -> SetGain(0.125);
  out -> SetPinout(26,25,22); //configuració pins de la transmissio
  aac->begin(in, out); //identifica el canal d entrada i de sortida
}
void loop(){
  if (aac->isRunning()) { //si esta funcionant la sortida fa un bucle
    aac->loop();
  } 
  else { // si no
    aac -> stop(); //el para
    Serial.printf("Sound Generator\n"); //treu un missatge
    delay(1000);
  }
}