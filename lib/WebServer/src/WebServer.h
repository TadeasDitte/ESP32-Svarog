#ifndef SVAROG_WEBSERVER_H
#define SVAROG_WEBSERVER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include "const.h"

class SvarogWebServer
{
public:
	void begin();
	void loop();

private:
	AsyncWebServer _server{80};
	String _activeFeature = "none";

	void registerRoutes();
	String statusJson() const;
};

#endif