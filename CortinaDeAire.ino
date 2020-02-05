/*
 * Copyright CRCibernetica S.A. 2020
 * MIT License
 * */
#include "RTClib.h"   // clock library https://github.com/adafruit/RTClib
#include <Bounce2.h>  // debounce library https://github.com/thomasfredericks/Bounce2

#define RELAYPIN  8 // Relay R3 on Industrial Shield
#define PUERTAPIN A5 // I0.9 on Industrial Shield

bool ESTADOCORTINA = false;
bool ESTADOPUERTA = false;
long previousMillis = 0;

RTC_DS3231 rtc;
Bounce puerta = Bounce();

void setup() {
  Serial.begin(115200);

  Serial.println(F("Inicializando entradas y salidas"));
  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, LOW);
  puerta.attach(PUERTAPIN);
  puerta.interval(5);

  Serial.println(F("Inicializando reloj"));
  if (! rtc.begin()) {
    Serial.println(F("Error de reloj...revisar bateria y conexiones"));
    while (1);
  } else {
    Serial.println(F("Conexion a reloj exitosa"));
    imprimirFechaHora();
  }
}

void loop() {
  if (millis() - previousMillis > 1000) { // revisar el reloj cada 1000 millisegundos
    DateTime now = rtc.now();
    bool debePrenderCortina = now.hour() >= 15 && now.minute() <= 1;
    if (debePrenderCortina != ESTADOCORTINA) {
      ESTADOCORTINA = debePrenderCortina;
      Serial.print(F("ESTADOCORTINA: "));
      Serial.println(ESTADOCORTINA);
      digitalWrite(RELAYPIN, ESTADOCORTINA);
    }
    previousMillis = millis();
  }
  
  puerta.update();
  int val = puerta.read();
  if (val != ESTADOPUERTA) {
    ESTADOPUERTA = val;
    Serial.print(F("ESTADO PUERTA: "));
    Serial.println(ESTADOPUERTA);
  }
}

void imprimirFechaHora() {
  DateTime now = rtc.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(now.dayOfTheWeek());
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  Serial.print(F("Temperature: "));
  Serial.print(rtc.getTemperature());
  Serial.println(" C");

  Serial.println();
}
