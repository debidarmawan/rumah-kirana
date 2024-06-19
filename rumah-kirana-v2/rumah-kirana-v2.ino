#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
// #include <UniversalTelegramBot.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", 25200); // 25200 represents the timezone offset in seconds for UTC+7

const char* ssid       = "Kirana";
const char* password   = "Anatri23";
// const char* BOT_TOKEN = "5942587653:AAHBmInVp1q8tMX14hBw4u1DJKEFKl1P5Pg";
const int lampPin = 5;
bool lampOn = false;
bool lampOff = false;

// UniversalTelegramBot bot("5942587653:AAHBmInVp1q8tMX14hBw4u1DJKEFKl1P5Pg");

void setup(){
  Serial.begin(115200);

  //connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize NTP client
  timeClient.begin();

  // Ensure initial time synchronization
  Serial.println("Synchronizing time with NTP server...");
  while(!timeClient.update()) {
    Serial.println("Failed to get time from NTP server, retrying...");
    timeClient.forceUpdate();
    delay(2000);
  }
  Serial.println("Time synchronized successfully.");

  pinMode(lampPin, OUTPUT);

  // bot.sendMessage("System started up!");
}

void loop(){
  timeClient.update();

  int hour = timeClient.getHours();
  int minute = timeClient.getMinutes();

  Serial.println(timeClient.getFormattedTime());

  if (hour < 6) {
    if (!lampOn) {
      digitalWrite(lampPin, HIGH);
      lampOn = true;

      Serial.print("Lamp Turned On By Time at ");
      Serial.print(hour);
      Serial.print(":");
      Serial.println(minute);
    }
  }

  if (hour >= 18) {
    if (!lampOn) {
      digitalWrite(lampPin, HIGH);
      lampOn = true;

      Serial.print("Lamp Turned On By Time at ");
      Serial.print(hour);
      Serial.print(":");
      Serial.println(minute);
    }
  }

  if (hour >= 6 && hour < 17) {
    if (lampOn) {
      digitalWrite(lampPin, LOW);
      lampOn = false;

      Serial.print("Lamp Turned Off By Time at ");
      Serial.print(hour);
      Serial.print(":");
      Serial.println(minute);
    }
  }

  delay(60000);
}
