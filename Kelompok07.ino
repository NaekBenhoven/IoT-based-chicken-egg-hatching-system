// Kelompok 07
// 1. Albert Panggabean
// 2. Naek Butarbutar
// 3. Risda Br. Malau

#define BLYNK_TEMPLATE_ID "TMPL67o81TlmF"
#define BLYNK_TEMPLATE_NAME "monitoring suhu dan kelembapan inkubator telur"
#define BLYNK_AUTH_TOKEN "hwNN8apCtsqav8s2mCOaoaIox4Zt9F-8"
#define BLYNK_PRINT Serial
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

//Password dan Nama Hostpot
char ssid[] = "ronaldogoat"; //nama hostpot atau wifi
char pass[] = "rajaraja34"; //password hostpot atau wifi

#include <SoftwareSerial.h>
SoftwareSerial EspSerial(12, 13); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600
 
ESP8266 wifi(&EspSerial);

#include "DHT.h"
DHT dht(2, DHT11);

#define RELAY_ON 0
#define RELAY_OFF 1
#define RELAY_1  7   
#define RELAY_2  8
#define RELAY_3  9
#define RELAY_4  10

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); 

#define suhuon 5
#define suhuoff 4


void setup() {
  Serial.begin(9600); 
// Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass);

  lcd.init();
  lcd.backlight(); 

  Serial.begin(9600);
  Serial.println("DHTxx test!");
  dht.begin();
  
 // Set pin as output.
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(suhuon, OUTPUT);
  pinMode(suhuoff, OUTPUT);
   
 // Initialize relay one as off so that on reset it would be off by default
  digitalWrite(RELAY_1, RELAY_OFF);
  digitalWrite(RELAY_2, RELAY_OFF);
  digitalWrite(RELAY_3, RELAY_OFF);

  // lcd.begin(16, 2);
  // lcd.setCursor(3,0);
  // lcd.print("INCUBATOR");
  // lcd.setCursor(3,1);
  // lcd.print("TELUR AYAM");
  // delay(5000);
  // lcd.clear();

  // lcd.setCursor(2,0);
  // lcd.print("Di Buat Oleh");
  // lcd.setCursor(2,1);
  // lcd.print("Kelompok  07");
  // delay(5000);
  // lcd.clear();

  
}

void loop() {
  // Baca humidity dan temperature
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print("kelembaban: ");
  Serial.print(h);
  Serial.print(" ");
  Serial.print("suhu: ");
  Serial.println(t);

  // Cek hasil pembacaan, dan tampilkan bila ok
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
    return;
  }

  if (t<37.00)// ON
{
  digitalWrite(RELAY_1, RELAY_ON);
  digitalWrite(RELAY_2, RELAY_ON);
  digitalWrite(RELAY_3, RELAY_ON);
  digitalWrite(suhuon, HIGH);
  digitalWrite(suhuoff, LOW);
}
else if (t >= 37.00 && t <38.50)//relay 2 off
{
  digitalWrite(RELAY_1, RELAY_ON);
  digitalWrite(RELAY_2, RELAY_OFF);
  digitalWrite(RELAY_3, RELAY_ON);
  digitalWrite(suhuon, HIGH);
  digitalWrite(suhuoff, LOW);
}  
 else if (t>39.00)//OFF
{
  digitalWrite(RELAY_1, RELAY_OFF);
  digitalWrite(RELAY_3, RELAY_OFF);
  digitalWrite(suhuoff, HIGH); 
  digitalWrite(suhuon, LOW);
}
    

  

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Kelembapan: ");
  lcd.setCursor(11,0);
  lcd.print(h);

  lcd.setCursor(0,1);
  lcd.print("Suhu: ");
  lcd.setCursor(5,1);
  lcd.print(t);
  delay(1000);

  Blynk.virtualWrite(V0, t); // menset pin virtual (V3) agar dapat menampilkan suhu ke aplikasi blynk
  Blynk.virtualWrite(V1, h);

  Blynk.run();

  
}

