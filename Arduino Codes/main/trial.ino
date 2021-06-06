#include <PubSubClient.h>
#include <stdlib.h>
#include <ESP8266WiFi.h>
const char* ssid = "ABCDEF";
const char* password = "9535109631";
const char* mqtt_server = "broker.mqtt-dashboard.com";

char pub_str[100];


WiFiClient espClient;
PubSubClient client(espClient);
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

void callback(char* topic, byte* payload, unsigned int length)
{

Serial.print("Message arrived : ");
 Serial.print(topic);
 Serial.print(" : ");
 for (int i = 0; i < length; i++)
 {
 Serial.println((char)payload[i]);
 }
 if ((char)payload[0] == 'o' && (char)payload[1] == 'n')
 {
 digitalWrite(2, LOW);
 }
 else if ((char)payload[0] == 'o' && (char)payload[1] == 'f' && (char)payload[2] == 'f' ) {
 digitalWrite(2, HIGH);
 }

}

void reconnect()
{

while(!client.connected()){
Serial.println("Attempting MQTT connection");
if(client.connect("...HiveMQ clientId..."))
{
Serial.println("Connected");
client.publish("...Your_topic_to_read_temp...","Connected!");
client.subscribe("...Your_topic_to_control_led...");
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
float tmp = gettemp();
//sprintf(pub_str,"%f", tmp);
dtostrf(tmp,2,6,pub_str);
Serial.print(pub_str);
Serial.println(tmp);
client.publish("...Your_topic_to_read_temp...",pub_str);
delay(1000);

client.loop();
}
