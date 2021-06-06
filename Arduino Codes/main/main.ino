// importing libraries
#include <PubSubClient.h>
#include <stdlib.h>
#include <ESP8266WiFi.h>

//global declarations
WiFiClient espClient;
PubSubClient client(espClient);


// essential classes
class Hardware
{
  private:
    String _ssid = "ABCDEF";
    String _password = "9535109631";
  
  public:
    void setup_wifi()
    {
      delay(10);
      Serial.print("connecting to");
      Serial.println(_ssid);
      WiFi.begin(_ssid, _password);
      
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
};


class PubMqtt
{
  public:

    const char* mqtt_server = "broker.mqtt-dashboard.com";
    String pub_topic;
    
    // constructor
    PubMqtt(String topic)
    {
      pub_topic = topic;
    }

    void publish_msg(String msg)
    {
      client.setServer(mqtt_server, 1883);
      while(!client.connected()){
        Serial.println("Attempting MQTT connection");
      if(client.connect("clientId-bsXUrjUkJD"))
      {
        Serial.println("Connected");
        client.publish(pub_topic,"Connected!");
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
};


class SubMqtt
{
  public:

    const char* mqtt_server = "broker.mqtt-dashboard.com";
    
    String sub_topic;
    
    // constructor
    SubMqtt(String topic)
    {
      sub_topic = topic;
    }

    void subscribe()
    {
      client.setServer(mqtt_server, 1883);
      client.setCallback(callback);
      while(!client.connected()){
        Serial.println("Attempting MQTT connection");
      if(client.connect("clientId-bsXUrjUkJD"))
      {
        client.subscribe(sub_topic);
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
};

PubMqtt pub_led1("protium/led1/switchStatus");

void setup()
{
  // setup code
}

void loop()
{
  // code in loop...
}
