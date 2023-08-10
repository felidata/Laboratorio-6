#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <RTClib.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Configuración del sensor DHT22
#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// Configuración de la tarjeta SD
const int chipSelect = 15;

File dataFile;

// Configuración del reloj RTC
RTC_DS3231 rtc;

void setup() {
  Serial.begin(9600);

  dht.begin();

  if (!SD.begin(chipSelect)) {
    Serial.println("Error al inicializar la tarjeta SD.");
    return;
  }

  Serial.println("Tarjeta SD inicializada correctamente.");

  Wire.begin();
  rtc.begin();
  /*rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  /*Usar esta línea de código si se desconfigura la fecha y hora del Reloj RTC*/ 
  
  if (rtc.lostPower()) {
    Serial.println("Reloj RTC no encontrado.");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  } else {
    Serial.println("Reloj RTC encontrado.");
  }
}

void loop() {
  // Medir la temperatura y la humedad
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Obtener la fecha y hora actual del reloj RTC
  DateTime now = rtc.now();

  if (temperature != -50){
    // Guardar los datos en la tarjeta SD
    dataFile = SD.open("data.txt", FILE_WRITE);
    if (dataFile) {
  
      Serial.print(now.year());
      Serial.print("-");
      Serial.print(now.month());
      Serial.print("-");
      Serial.print(now.day());
      Serial.print(" ");
      Serial.print(now.hour());
      Serial.print(":");
      Serial.print(now.minute());
      Serial.print(":");
      Serial.print(now.second());
      Serial.print(" ");
      Serial.print(temperature);
      Serial.print("ºC");
      Serial.print(" y ");
      Serial.print(humidity);
      Serial.println(" %HR");
      
      dataFile.print(now.year());
      dataFile.print("-");
      dataFile.print(now.month());
      dataFile.print("-");
      dataFile.print(now.day());
      dataFile.print(" ");
      dataFile.print(now.hour());
      dataFile.print(":");
      dataFile.print(now.minute());
      dataFile.print(":");
      dataFile.print(now.second());
      dataFile.print(",");
      dataFile.print(temperature);
      dataFile.print(",");
      dataFile.println(humidity);
      dataFile.close();
      Serial.println("Datos guardados en la tarjeta SD.");
    } else {
      Serial.println("Error al abrir el archivo data.txt.");
    }
  }
  delay(10000);
}
