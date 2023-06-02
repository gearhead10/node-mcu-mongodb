#include <DHT.h> //Libreria DHT
#include <ESP8266WiFi.h> //Libreria ESP
#include "LittleFS.h" //Libreria FileSystem
#include "ESPAsyncTCP.h" //Libreria ASYNC TCP
#include "ESPAsyncWebServer.h" //Libreria Servidor Web
#include <ArduinoJson.h> //LIbreria Arduino Json
#include <PubSubClient.h> //Libreria Cliente MQTT
#include <AsTime.h>
#include <AsLed.h>
#include <AsButton.h>
//#include <HttpClient.h>
#include <ESP8266HTTPClient.h>

#define _luz 2
#define _inteructor 0

//Creación de objeto Sensor DHT11
#define DHTPIN 12
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

AsLed luz(_luz, ACTIVE_LOW);

//Variables
static String cj;
static String sj;
static String pj;
static String msj;
static String muj;
static String mpj;
static String mpoj;
byte b = 200;
String dataFile;

//Conexión Wifi
const char* ssid = "<SSID>";
const char* password = "<password>";
WiFiEventHandler gotIp, disconnected;

//Conexión MQTT
WiFiClient espClient;
PubSubClient client(espClient);
String mqttServer = "broker.mqttdashboard.com";
String mqttPort = "1883";
String mqttUser = "";
String mqttPassword = "";
String mqttTopic = "Name/proyect";

//Variable de ArduinoJson
String payload = "";

//Creación de Objeto Timers
AsTime timeMqtt;
AsTime ts;

//Servidor Web Asyncrono
AsyncWebServer server(80);

//Procesor (Si no carga la pagina web correctamente)
String processor(const String& var) {
  if (var == "mac")return "mac";
  return String();
}

///////////*****************//////////////
//PRUEBA CONEXION MONGODB ATLAS

const char* mongodbServer = "xxx-xxx.mongodb.net/xxx"; // Reemplaza con la dirección de tu clúster
const int mongodbPort = 443; // Puerto para conexiones HTTPS
const char* mongodbDatabase = "database-name"; // Reemplaza con el nombre de tu base de datos
const String mongodbCollection = "collection-name"; // Reemplaza con el nombre de tu colección
const char* mongodbUsername = "user"; // Reemplaza con tu nombre de usuario de MongoDB Atlas
const char* mongodbPassword = "password"; //Reemplaza con tu contraseña de usuario MongoDB Atlas7
const String mongodbApiKey = "api key";

void sendToMongoDB(const String& json){
  HTTPClient https;
  String url = "https://" + String(mongodbServer) + ":" + String(mongodbPort) + "api/atlas/v1.0/clusters" + String(mongodbDatabase) +
    "/" + String(mongodbCollection) + "?apiKey=" + String(mongodbApiKey); // reemplazar este ultimo por la API KEY de mongoDB


  bool begin(WiFiClient &client, const String& url);
  https.addHeader("Content-Type", "application/json");
  https.setAuthorization(mongodbUsername, mongodbPassword);

  int httpsResponseCode = https.POST(json);
  Serial.print("-- Iniciando conexión HTTPs -> MongoDB Atlas en puerto --" + mongodbPort);

  if (httpsResponseCode > 0){
    Serial.print("Conexión establecida.");
    Serial.print("HTTP(s) Response code: ");
    Serial.println(httpsResponseCode);
    String response = https.getString();
    Serial.println(response);
  }else{
    Serial.print("Error en la solicitud HTTP(s): ");
    Serial.println(httpsResponseCode);
  }

  https.end();
}

//////////*****************////////////////
void setup() {
  dht.begin();
  Serial.begin(9600);
  client.setServer(mqttServer.c_str(), mqttPort.toInt());
  conexion();
  ts.run();
}

void loop() {
  client.loop();

  timeMqtt.setTimeout([]() {
    if (!client.connected()) {
      reconnect();
    }
    timeMqtt.run();
  }, 5000);

  ts.setTimeout([]() { // metodo setTimeout con funcion anonima
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    // Validar si el sensor está conectado
    if (isnan(t) || isnan(h)) {
      Serial.println(F("Error. No se detecta sensor."));
      return;
    }
    //Serializar valores de la variable
    StaticJsonDocument<90> jsonDoc;
    jsonDoc["Humedad"] = h;
    jsonDoc["Temperatura"] = t;

    /****/
    //VARIABLE JSON STRING
    //serializeJson1(jsonDoc, jsonStr);

    //PRUEBA DE ENVIO DE DATOS A MONGODB
    sendToMongoDB(payload);
    //delay(5000)
    /****/


    //SERIALIZE ORIGINAL
    serializeJson(jsonDoc, payload);

    client.publish((char*)mqttTopic.c_str(), (char*)payload.c_str());
    payload.clear();
    ts.run(); // Reinicio de sensor para tomar otra medida
  }, 10000);
}

void reconnect() {

  if (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqttUser.c_str(), mqttPassword.c_str())) {
      Serial.println("Connected.");
      client.subscribe(mqttTopic.c_str());
    } else {
      Serial.print("Error, rc= ");
      Serial.print(client.state());
      Serial.println("Retrying connection in 15 seconds ...");
    }
  }
}

void conexion() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  //Evento de conexión Wifi
  gotIp = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP& ) {
    Serial.print("Station attached, IP: ");
    Serial.println(WiFi.localIP());
    timeMqtt.run();
  });

  //Evento de desconexión Wifi
  disconnected = WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected& ) {
    Serial.println("Station desconnect");
  });
}
