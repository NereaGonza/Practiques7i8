# PRACTICA 7 : Buses de comunicación III (I2S)

>Objetivo: Describir el funcionamiento del bus I2S y realizar una
practica para comprender su funcionamiento


## Funcionalidad de la práctica
---------------------------------
>1. Describir la salida por el puerto serie
>2. Explicar el funcionamiento


>2. Part 1
```
#include "AudioGeneratorAAC.h"
#include "AudioOutputI2S.h"
#include "AudioFileSourcePROGMEM.h"
#include "sampleaac.h"
AudioFileSourcePROGMEM *in;
AudioGeneratorAAC *aac;
AudioOutputI2S *out;
void setup(){
Serial.begin(115200);
in = new AudioFileSourcePROGMEM(sampleaac, sizeof(sampleaac));
aac = new AudioGeneratorAAC();
out = new AudioOutputI2S();
out -> SetGain(0.125);
out -> SetPinout(26,25,22);
aac->begin(in, out);
}
void loop(){
if (aac->isRunning()) {
aac->loop();
} else {
aac -> stop();
Serial.printf("Sound Generator\n");
delay(1000);
}
}
```

>Establecemos las librerías. Se encargan de: 
AudioGeneratorAAC: genera la salida de audio.
AudioOutputI2S: Clase base para el puerto de interfaz I2S
AudioFileSourcePROGMEM: almacena un "archivo" como una matriz PROGMEM y lo utiliza como datos de fuente de audio
Sampleaac: archivo de encabezado que almacena el archivo de audio como matriz
```
#include "AudioGeneratorAAC.h"
#include "AudioOutputI2S.h"
#include "AudioFileSourcePROGMEM.h"
#include "sampleaac.h"
```
Para cargar el código Arduino con el archivo de encabezado al EPS32, es importante que el archivo .ino y el encabezado (archivo .h) estén en la misma carpeta.

Tal y como hemos hecho en las prácticas anteriores, establecemos la velocidad en baudios en 115200 e inicializamos los archivos de encabezado. 

El objeto AudioOutputI2S tiene diferentes funciones: utilizaremos la función SetGain, que se encarga de reducir el volumen del altavoz y definimos el pinout con la función SetPinout . En este caso, escogeremos:

Reloj serial (SCK) = 26
Selección de palabra (WS) = 25
Datos en serie (SD) = 22
No obstante, se pueden poner los pines que queramos. 

El último paso de la función de configuración es conectar los datos de sonido de entrada desde la memoria interna del programa a la salida de audio I2S con la función de inicio AudioGeneratorAAC . 

```
AudioFileSourcePROGMEM *in;
AudioGeneratorAAC *aac;
AudioOutputI2S *out;
void setup(){
Serial.begin(115200);
in = new AudioFileSourcePROGMEM(sampleaac, sizeof(sampleaac));
aac = new AudioGeneratorAAC();
out = new AudioOutputI2S();
out -> SetGain(0.125);
out -> SetPinout(26,25,22);
aac->begin(in, out);
}
```
En la función de bucle, el generador de audio continúa funcionando hasta que toda la matriz de sonido pasa por el generador. Cuando el generador está listo, deja de funcionar y en la salida "serial" el generador de sonido estará listo para funcionar.

```
void loop(){
if (aac->isRunning()) {
aac->loop();
} else {
aac -> stop();
Serial.printf("Sound Generator\n");
delay(1000);
}
}
```





# PART 2 
>2. Explicación

```
#include "Audio.h"
#include "SD.h"
#include "FS.h"
// Digital I/O used
#define SD_CS 5
#define SPI_MOSI 23
#define SPI_MISO 19
#define SPI_SCK 18
#define I2S_DOUT 25
#define I2S_BCLK 27
#define I2S_LRC 26
Audio audio;
void setup(){
pinMode(SD_CS, OUTPUT);
digitalWrite(SD_CS, HIGH);
SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
Serial.begin(115200);
SD.begin(SD_CS);
audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
audio.setVolume(10); // 0...21
audio.connecttoFS(SD, "Ensoniq-ZR-76-01-Dope-77.wav");
}
void loop(){
audio.loop();
}
// optional
void audio_info(const char *info){
Serial.print("info "); Serial.println(info);
}
void audio_id3data(const char *info){ //id3 metadata
Serial.print("id3data ");Serial.println(info);
}
void audio_eof_mp3(const char *info){ //end of file
Serial.print("eof_mp3 ");Serial.println(info);
}
void audio_showstation(const char *info){
Serial.print("station ");Serial.println(info);
}
void audio_showstreaminfo(const char *info){
Serial.print("streaminfo ");Serial.println(info);
}
void audio_showstreamtitle(const char *info){
Serial.print("streamtitle ");Serial.println(info);
}
void audio_bitrate(const char *info){
Serial.print("bitrate ");Serial.println(info);
}
void audio_commercial(const char *info){ //duration in sec
Serial.print("commercial ");Serial.println(info);
}
void audio_icyurl(const char *info){ //homepage
Serial.print("icyurl ");Serial.println(info);
}
void audio_lasthost(const char *info){ //stream URL played
Serial.print("lasthost ");Serial.println(info);
}
void audio_eof_speech(const char *info){
Serial.print("eof_speech ");Serial.println(info);
}

```

Establecemos las librerías: Audio , SD y FS, también definiremos los pines.

```
#include "Audio.h"
#include "SD.h"
#include "FS.h"
// Digital I/O used
#define SD_CS 5
#define SPI_MOSI 23
#define SPI_MISO 19
#define SPI_SCK 18
#define I2S_DOUT 25
#define I2S_BCLK 27
#define I2S_LRC 26
```


Generamos una variable audio. Establecemos las funciones: 
PinMode: permite configurar a cada pin, de forma individual, como entrada o como salida. 
DigitalWrite: permite escribir valores lógicos digitales en un pin de Salida de una tarjeta Arduino.
SPI.begin inicia el bus de SPI. 
Tal y como hemos hecho en las prácticas anteriores, establecemos la velocidad en baudios en 115200 e inicializamos los archivos de encabezado (Serial.begin).

SD.begin inicializa la biblioteca SD y la tarjeta, como parámetro se le indica el pin CS al que está conectado el modulo, si no se especifica cspin , se usa el valor por defecto del CS por hardware. Los demás pines deben estar conectados al SPI por hardware del dispositivo. 

Por último, indicaremos los pines, el volumen y los conectores FS. 
```
Audio audio;
void setup(){
pinMode(SD_CS, OUTPUT);
digitalWrite(SD_CS, HIGH);
SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
Serial.begin(115200);
SD.begin(SD_CS);
audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
audio.setVolume(10); // 0...21
audio.connecttoFS(SD, "Ensoniq-ZR-76-01-Dope-77.wav");
}
```

Por último creamos un bucle para que se ejecute constantemente el audio. 

```
void loop(){
audio.loop();
}
```

Este apartado es opcional. Lo añadiremos siempre que querramos mostrar ciertos mensajes por el monitor.


```
// optional
void audio_info(const char *info){
Serial.print("info "); Serial.println(info);
}

void audio_id3data(const char *info){ //id3 metadata
Serial.print("id3data ");Serial.println(info);
}

void audio_eof_mp3(const char *info){ //end of file
Serial.print("eof_mp3 ");Serial.println(info);
}
void audio_showstation(const char *info){
Serial.print("station ");Serial.println(info);
}
void audio_showstreaminfo(const char *info){
Serial.print("streaminfo ");Serial.println(info);
}
void audio_showstreamtitle(const char *info){
Serial.print("streamtitle ");Serial.println(info);
}
void audio_bitrate(const char *info){
Serial.print("bitrate ");Serial.println(info);
}
void audio_commercial(const char *info){ //duration in sec
Serial.print("commercial ");Serial.println(info);
}
void audio_icyurl(const char *info){ //homepage
Serial.print("icyurl ");Serial.println(info);
}
void audio_lasthost(const char *info){ //stream URL played
Serial.print("lasthost ");Serial.println(info);
}
void audio_eof_speech(const char *info){
Serial.print("eof_speech ");Serial.println(info);
}
```