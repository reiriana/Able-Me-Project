//ultrasonic
const int trigPin = 22;
const int echoPin = 23;

#define SOUND_SPEED 0.034

long duration;
float distanceCm;
float distanceInch;

//firebase & wifi config
#include <WiFi.h>
#include <FirebaseESP32.h>

#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

FirebaseData firebaseData;
FirebaseJson json;

#define DATABASE_URL "ableme-7bb3a-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define API_KEY "AIzaSyCoQCaN3NiSvSBsDPOr4BbkPAc-vBBdg5s"
#define WIFI_SSID "Auffs-2.4G@unifi"
#define WIFI_PASSWORD "0139745335abc"

//auth
#define USER_EMAIL "taoswsbandori@gmail.com"
#define USER_PASSWORD "FAREISHA"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting"); 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Firebase.setDoubleDigits(5);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}
 
void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_SPEED/2;
  
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  delay(100);
  json.set("/Data", distanceCm);
  Firebase.updateNode(firebaseData, "/Sensor",json);
}
