#include <WiFi.h>
#include <WiFiClient.h>


const char* obd_host = "192.168.0.10";
const int obd_port = 35000;

const char* obd_ssid = "WiFi_OBDII";
const char* obd_pass = "";

unsigned long startTimer = 0;

WiFiClient client;

void send_command(char* command, unsigned long delay){
  char c;
  client.print(command);
  while(millis() - startTimer < delay);
  while(client.available()){
    c = client.read();
    Serial.print(c);
  }
  Serial.println("");
  startTimer = millis();
}

void setup() {
  Serial.begin(115200);

  // Connessione alla rete Wi-Fi
  WiFi.begin(obd_ssid, obd_pass);
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

  send_command("ATZ\r\n", 3000);
  send_command("ATE0\r\n", 0);
  send_command("ATL0\r\n", 0);
  send_command("ATH1\r\n", 0);
  send_command("01 00\r\n", 0);
  send_command("ATDPN\r\n", 0);
  //Serial.println("Benza");
  send_command("01 2F\r\n", 0);
  //Serial.write(0xFF);
  //Serial.write(1);
  //Serial.write(0xFE);
  

}

