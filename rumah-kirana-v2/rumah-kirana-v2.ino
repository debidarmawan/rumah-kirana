#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>
#include <NTPClient.h>
#include <UniversalTelegramBot.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", 25200);  // 25200 represents the timezone offset in seconds for UTC+7

const char* ssid = "Kirana";
const char* password = "Anatri23";
const char* BOT_TOKEN = "7271170210:AAHMSo9I0Cor6uVOv4igcbC_eEHcROUJFgo";
const int lampPin = 5;
bool lampOn = false;
bool lampOff = false;

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

String chatId = "641416700";

int syncAttempts = 0;
const int maxSyncAttempts = 3;  // Maximum number of synchronization attempts

void setup() {
  Serial.begin(115200);

  //connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }
  Serial.println("CONNECTED");
  // Serial.println("IP address: ");
  // Serial.println(WiFi.localIP());

  // Initialize NTP client
  timeClient.begin();

  // Ensure initial time synchronization
  Serial.println("Synchronizing time with NTP server ");
  if (!timeClient.update()) {
    Serial.println("Failed to get time from NTP server, retrying ");
    while (!timeClient.update()) {
      Serial.println(".");
      timeClient.forceUpdate();
      delay(10000);

      syncAttempts++;

      // Check if retry limit exceeded
      if (syncAttempts >= maxSyncAttempts) {
        Serial.println("Failed to synchronize time after 30 seconds. Restarting ESP32...");
        delay(5000);  // Delay for 5 second before restart

        // Restart ESP32
        ESP.restart();
      }
    }
  }
  Serial.println("Time synchronized successfully.");

  pinMode(lampPin, OUTPUT);

  // bot.sendMessage(chatId, "System started up!");
  bool messageSent = bot.sendMessage(chatId, "System started up!", "");

  if (messageSent) {
    Serial.println("Message sent successfully!");
  } else {
    Serial.println("Failed to send message. Check botToken, chatId, or network connection.");
  }
}

void loop() {
  timeClient.update();

  int hour = timeClient.getHours();
  int minute = timeClient.getMinutes();

  // Serial.println(timeClient.getFormattedTime());

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
