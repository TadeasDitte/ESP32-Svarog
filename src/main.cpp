#include <Rfid.h>

#include "WebServer.h"

Rfid rfid;
SvarogWebServer web;

void setup() {
  Serial.begin(115200);

  rfid.begin();
  web.begin();
}

void loop() {
  web.loop();
}