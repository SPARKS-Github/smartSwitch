#include <PubSubClient.h>
#include <stdlib.h>
#include <ESP8266WiFi.h>

// wifi connection
const char* ssid = "ABCDEF";
const char* password = "9535109631";
const char* mqtt_server = "broker.mqtt-dashboard.com";

char pub_str[100];

WiFiClient espClient;
PubSubClient client(espClient);

// Function to connect to wifi network
void setup_wifi()
{
  delay(10);
  Serial.print("connecting to");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("-");
  }
  Serial.println();
  Serial.println("WiFi Connected");
  Serial.println("WiFi got IP");
  Serial.println();
  Serial.println(WiFi.localIP());
}

//function to print message after subscription
void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.println("Message arrived : ");
  Serial.print(topic);
  Serial.print(" : ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.print("\n");
  if ((char)payload[0] == 'o' && (char)payload[1] == 'n'){
    digitalWrite(2, HIGH);
    strncpy(pub_str, "on", sizeof(pub_str));
    }
  else if ((char)payload[0] == 'o' && (char)payload[1] == 'f' && (char)payload[2] == 'f' ) {
    digitalWrite(2, LOW);
    strncpy(pub_str, "off", sizeof(pub_str));
    }
}

void reconnect()
{
  while(!client.connected()){
    Serial.println("Attempting MQTT connection");
  if(client.connect("clientId-bsXUrjUkJD"))
  {
    Serial.println("Connected");
    client.publish("protium/switchStatus/led1","Connected!");
    client.subscribe("protium/changeState/led1");
    Serial.print("subscribed!");
  }
  else
  {
    Serial.print("Failed, rc = ");
    Serial.print(client.state());
    Serial.println("Waiting for 5 seconds to try again");
    delay(5000);
  }
  }
}

void setup()
{
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  reconnect();
}

void loop()
{

if(!client.connected())
{
reconnect();
Serial.print("disconnected");
}
//sprintf(pub_str,"%f", tmp);
//Serial.println(pub_str);
client.publish("protium/switchStatus/led1",pub_str);
delay(1000);

client.loop();
}
