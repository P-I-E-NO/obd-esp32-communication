#include <WiFi.h>
#include <WiFiClient.h>

// Impostazioni OBD
const char* obd_host = "192.168.0.10";
const int obd_port = 35000;

unsigned long startTimer = 0;

WiFiClient client;

void setup() {
  Serial.begin(115200);

  // Connessione alla rete Wi-Fi
  WiFi.begin("WiFi_OBDII", "");
  while (WiFi.status() != WL_CONNECTED) {
    startTimer = millis();
    while(millis() - startTimer < 500);
    Serial.print(".");
  }
}

void loop() {
  // Connessione al dispositivo OBD
  client.connect(obd_host, obd_port);
  if (!client.connected()) {
    Serial.println("Errore: Connessione OBD fallita!");
    return;
  }

  Serial.println("Connesso al dispositivo OBD!");

  char c;
  client.print("ATZ\r\n");
  while(millis() - startTimer < 3000);
  while(client.available()){
    c = client.read();
    Serial.print(c);
  }
  Serial.println("");
  startTimer = millis();

  client.print("ATE0\r\n");
  while(millis() - startTimer < 1000);
  while(client.available()){
    c = client.read();
    Serial.print(c);
  }
  Serial.println("");
  startTimer = millis();

  client.print("ATL0\r\n");
  while(millis() - startTimer < 1000);
  startTimer = millis();
  while(client.available()){
    c = client.read();
    Serial.print(c);
  }
  Serial.println("");

  client.print("ATH1\r\n");
  while(millis() - startTimer < 1000);
  while(client.available()){
    c = client.read();
    Serial.print(c);
  }
  Serial.println("");
  startTimer = millis();

  client.print("01 00\r\n");
  while(millis() - startTimer < 1000);
  while(client.available()){
    c = client.read();
    Serial.print(c);
  }
  Serial.println("");
  startTimer = millis();

  client.print("ATDPN\r\n");
  while(millis() - startTimer < 1000);
  while(client.available()){
    c = client.read();
    Serial.print(c);
  }
  Serial.println("");
  startTimer = millis();

  client.print("01 2F\r\n");
  while(millis() - startTimer < 1000);
  Serial.write(0xFF);
  Serial.write(1);
  while(client.available()){
    c = client.read();
    Serial.write(c);
  }
  Serial.write(0xFE);
  Serial.println("");
  startTimer = millis();
}

