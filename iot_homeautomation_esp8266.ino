#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <DHT.h>

// Pin definitions
int led1 = D0;
int led2 = D1;
int led3 = D2;
int led4 = D4;
int led5 = D5;
int led6 = D6;
int led7 = D7;
int loff = D8;

int count = 0;

#define DHTPIN D3
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);


// MQTT client setup
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Feeds
Adafruit_MQTT_Publish Temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temp1");
Adafruit_MQTT_Publish Humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Humidity");
Adafruit_MQTT_Publish Light1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Light1");
Adafruit_MQTT_Publish Light2 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Light2");
Adafruit_MQTT_Publish Light3 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Light3");
Adafruit_MQTT_Publish Light4 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Light4");
Adafruit_MQTT_Publish Light5 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Light5");
Adafruit_MQTT_Publish Light6 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Light6");
Adafruit_MQTT_Publish Light7 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Light7");
Adafruit_MQTT_Publish Flag = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Flag");

// Subscriptions
Adafruit_MQTT_Subscribe onoffbutton1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Light1");
Adafruit_MQTT_Subscribe onoffbutton2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Light2");
Adafruit_MQTT_Subscribe onoffbutton3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Light3");
Adafruit_MQTT_Subscribe onoffbutton4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Light4");
Adafruit_MQTT_Subscribe onoffbutton5 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Light5");
Adafruit_MQTT_Subscribe onoffbutton6 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Light6");
Adafruit_MQTT_Subscribe onoffbutton7 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Light7");

// Function to connect and reconnect to MQTT
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");
  
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // Connect will return 0 for success
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // Wait for WDT reset
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}

void setup() {
  Serial.begin(9600);
  delay(10);
  dht.begin();

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Subscribe to MQTT feeds
  mqtt.subscribe(&onoffbutton1);
  mqtt.subscribe(&onoffbutton2);
  mqtt.subscribe(&onoffbutton3);
  mqtt.subscribe(&onoffbutton4);
  mqtt.subscribe(&onoffbutton5);
  mqtt.subscribe(&onoffbutton6);
  mqtt.subscribe(&onoffbutton7);

  // Pin setup
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, INPUT);
  pinMode(loff, INPUT);
  
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);
  digitalWrite(led6, LOW);
  digitalWrite(led7, LOW);
}

void loop() {
  // Connect to MQTT server if not connected
  MQTT_connect();

  // Handle incoming MQTT messages
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &onoffbutton1) handleLightControl(led1, onoffbutton1);
    if (subscription == &onoffbutton2) handleLightControl(led2, onoffbutton2);
    if (subscription == &onoffbutton3) handleLightControl(led3, onoffbutton3);
    if (subscription == &onoffbutton4) handleLightControl(led4, onoffbutton4);
    if (subscription == &onoffbutton5) handleLightControl(led5, onoffbutton5);
    if (subscription == &onoffbutton6) handleLightControl(led6, onoffbutton6);
    if (subscription == &onoffbutton7) handleLightControl(led7, onoffbutton7);
  }

  // Publish sensor data
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (!Temperature.publish(t)) {
    Serial.println(F("Failed to send temperature"));
  }
  
  if (!Humidity.publish(h)) {
    Serial.println(F("Failed to send humidity"));
  }

  // Handle lights
  handleLights();

  delay(1000);  // Delay before next loop iteration
}

void handleLightControl(int ledPin, Adafruit_MQTT_Subscribe& button) {
  Serial.print(F("Got: "));
  Serial.println((char *)button.lastread);
  if (!strcmp((char*)button.lastread, "ON")) {
    digitalWrite(ledPin, HIGH);
  } else if (!strcmp((char*)button.lastread, "OFF")) {
    digitalWrite(ledPin, LOW);
  } else {
    button.publish("ERROR");
  }
}

void handleLights() {
  if (digitalRead(loff) == 1) {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
    digitalWrite(led7, LOW);
  }

  if (digitalRead(led7) == 1) {
    digitalWrite(led6, HIGH);
    Light7.publish("ON");
    delay(3000);
    digitalWrite(led6, LOW);
  }
}
