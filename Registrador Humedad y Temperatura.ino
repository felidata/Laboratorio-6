#include <SD.h> // librería para la SD
#include <DHT.h> // librería para el sensor de Humedad y Temperatura DHT22
#include <Wire.h>    // incluye libreria para interfaz I2C
#include <RTClib.h>   // libreria para el manejo del modulo RTC

#define DHTPIN D2 // El pin de datos del DHT será conectado con el pin D2 del ESP
#define DHTTYPE DHT22 // Tipo de sensor
#define CS_PIN  D8 //Pin conectado a CS del módulo SD

RTC_DS3231 RTC;     // crea objeto del tipo RTC_DS3231
DHT dht(DHTPIN, DHTTYPE); // crea el objeto para la comunicacion con el sensor dht

void setup()
{
  Serial.begin(9600);
  Serial.print("Inicializando la tarjeta SD");

  //Inicializa el objeto para la comunicación con el sensor DHT
  dht.begin();

  // Verifica si la tarjeta SD está conectada y si está funcionando
  if (!SD.begin(CS_PIN)) {
    Serial.println("Error, verifique si la memoria SD está conectada.");
    return;
  }

  Serial.println("SD inicializada");

  File dataFile = SD.open("ensayoPVA-SinCeldas-12-12-14-55.txt", FILE_WRITE);
  // Si el archivo fue abierto correctamente, escribe una línea de ceros.
  if (dataFile) {
    Serial.println("El archivo fue abierto correctamente");
  }
  // Si el archivo no pudo ser abierto, los datos no serán guardados.
  else {
    Serial.println("Error al abrir el archivo");
  }

  RTC.begin();  // Init RTC
  RTC.adjust(DateTime(__DATE__, __TIME__));  // Time and date is expanded to date and time on your computer at compiletime
  Serial.print("Time and date set");


}

void loop()
{
  //Lectura de humedad
  float humedad = dht.readHumidity();
  Serial.print("Humedad: ");
  Serial.println(humedad);
  //lectura de temperatura
  float temperatura = dht.readTemperature();
  Serial.print("Temperatura: ");
  Serial.println(temperatura);

  File dataFile = SD.open("ensayoPVA-SinCeldas-12-12-14-55.txt", FILE_WRITE);
  DateTime fecha = RTC.now();      // funcion que devuelve fecha y horario en formato
  // DateTime y asigna a variable fecha

  if (dataFile) { // Si el archivo fue abierto correctamente, se guardarán los datos.
    Serial.println("El archivo fue abierto correctamente");

    Serial.print(fecha.day());     // funcion que obtiene el dia de la fecha completa
    Serial.print("/");       // caracter barra como separador
    Serial.print(fecha.month());     // funcion que obtiene el mes de la fecha completa
    Serial.print("/");       // caracter barra como separador
    Serial.print(fecha.year());      // funcion que obtiene el año de la fecha completa
    Serial.print(" ");       // caracter espacio en blanco como separador
    Serial.print(fecha.hour());      // funcion que obtiene la hora de la fecha completa
    Serial.print(":");       // caracter dos puntos como separador
    Serial.print(fecha.minute());      // funcion que obtiene los minutos de la fecha completa
    Serial.print(":");       // caracter dos puntos como separador
    Serial.println(fecha.second());    // funcion que obtiene los segundos de la fecha completa

    //Escribo los datos en el archivo con el formato csv (humedad,temperatura,dia,mes,año,hora,minutos,segundos).
    dataFile.print(temperatura);
    dataFile.print(",");
    dataFile.println(humedad);

    /*
      dataFile.print(",");
      dataFile.print(fecha.day());     // funcion que obtiene el dia de la fecha completa
      dataFile.print(",");
      dataFile.print(fecha.month());     // funcion que obtiene el mes de la fecha completa
      dataFile.print(",");
      dataFile.print(fecha.year());      // funcion que obtiene el año de la fecha completa
      dataFile.print(",");
      dataFile.print(fecha.hour());      // funcion que obtiene la hora de la fecha completa
      dataFile.print(",");
      dataFile.print(fecha.minute());      // funcion que obtiene los minutos de la fecha completa
      dataFile.print(",");
      dataFile.println(fecha.second());    // funcion que obtiene los segundos de la fecha completa
    */

    //Cierro el txt
    dataFile.close();
  }
  else {
    Serial.println("Error al abrir el archivo txt");
  }

  //intervalo de espera para un nuevo registro de datos (ms).
  delay(300000);

}
