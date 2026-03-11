#include "WebServer.h"

void SvarogWebServer::begin()
{
	WiFi.mode(WIFI_AP);
	WiFi.softAP(AP_SSID, AP_PASSWORD);

	registerRoutes();
	_server.begin();

	Serial.println("[Web] AP started");
	Serial.print("[Web] SSID: ");
	Serial.println(AP_SSID);
	Serial.print("[Web] IP: ");
	Serial.println(WiFi.softAPIP());
}

void SvarogWebServer::loop()
{
	// Reserved for future periodic web tasks.
}

void SvarogWebServer::registerRoutes()
{
	_server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request)
			   {
		const char *page =
			"<!doctype html><html><head><meta name='viewport' content='width=device-width,initial-scale=1'/>"
			"<title>Svarog</title><style>body{font-family:monospace;max-width:680px;margin:30px auto;padding:0 14px;}"
			"button{margin:4px;padding:10px 14px;}code{background:#eee;padding:2px 6px;border-radius:4px;}</style></head><body>"
			"<h2>Svarog Feature Launcher</h2><p>Use these placeholders while building RFID and other modules.</p>"
			"<button onclick=\"start('rfid-read')\">Start RFID Read</button>"
			"<button onclick=\"start('rfid-save')\">Start RFID Save</button>"
			"<button onclick=\"start('rfid-emulate')\">Start RFID Emulate</button>"
			"<p>API: <code>/api/status</code> and <code>/api/feature/start?name=...</code></p>"
			"<pre id='status'>loading...</pre>"
			"<script>async function refresh(){let r=await fetch('/api/status');document.getElementById('status').textContent=await r.text();}"
			"async function start(n){await fetch('/api/feature/start?name='+encodeURIComponent(n));refresh();}"
			"refresh();setInterval(refresh,2000);</script></body></html>";
		request->send(200, "text/html", page); });

	_server.on("/api/status", HTTP_GET, [this](AsyncWebServerRequest *request)
			   { request->send(200, "application/json", statusJson()); });

	_server.on("/api/features", HTTP_GET, [](AsyncWebServerRequest *request)
			   { request->send(200, "application/json", "{\"features\":[\"rfid-read\",\"rfid-save\",\"rfid-emulate\"]}"); });

	_server.on("/api/feature/start", HTTP_GET, [this](AsyncWebServerRequest *request)
			   {
		if (!request->hasParam("name"))
		{
			request->send(400, "application/json", "{\"ok\":false,\"error\":\"missing name\"}");
			return;
		}

		_activeFeature = request->getParam("name")->value();
		Serial.print("[Web] Feature requested: ");
		Serial.println(_activeFeature);
		request->send(200, "application/json", String("{\"ok\":true,\"active\":\"") + _activeFeature + "\"}"); });

	_server.onNotFound([](AsyncWebServerRequest *request)
					   { request->send(404, "application/json", "{\"ok\":false,\"error\":\"not found\"}"); });
}

String SvarogWebServer::statusJson() const
{
	String json = "{";
	json += "\"ap\":\"";
	json += AP_SSID;
	json += "\",";
	json += "\"ip\":\"";
	json += WiFi.softAPIP().toString();
	json += "\",";
	json += "\"activeFeature\":\"";
	json += _activeFeature;
	json += "\"}";

	return json;
}

