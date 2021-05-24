import paho.mqtt.client as mqtt
import time

class pub_mqtt():
    """
    This class is used to publish a message on a topic.
    input: topic.
    """
    
    def __init__(self, topic):
        """
        topic - topic name to publish message.
        """
        self.broker_url = "broker.mqttdashboard.com"
        self.broker_port = 1883
        self.pub_message = ""
        self.pub_topic = topic
        
        self.pub_client = mqtt.Client()
        self.pub_client.on_publish = self.callback_on_publish
        self.pub_client.connect(self.broker_url, self.broker_port)
        
        
        
    def callback_on_publish(self, client, userdata, mid):
        print("--- Publisher ---")
        print("[INFO] Topic: {}".format(self.pub_topic)) 
        print("[INFO] Message Published: {}".format(self.pub_message)) 
        print("------------")
    
    def publish_msg(self, message):
        """
        This function will publish theessage to the topic.
        input: message (string)
        """
        self.pub_message = message
        self.pub_client.publish(topic=self.pub_topic, payload=message, qos=0, retain=False)
        time.sleep(1)



class sub_mqtt():
    """
    This function is used to subscribe the topic.
    input: topic
    """
    def __init__(self, topic):
        """
        topic: topic name to subscribe the message.
        """
        
        # define some parameters
        self.broker_url = "broker.mqttdashboard.com" 
        self.broker_port = 1883
        self.sub_topic = topic
        
        # create object og mqtt client
        self.sub_client = mqtt.Client() 
        
        # create callback function
        self.sub_client.on_connect = self.callback_on_connect 
        self.sub_client.on_message = self.callback_on_message
        
        # initiate the connection and subscribe to topic
        self.sub_client.connect(self.broker_url, self.broker_port)
        self.sub_client.subscribe(self.sub_topic, qos=0) 
        self.sub_client.loop_forever()
    
    def callback_on_connect(self, client, userdata, flags, rc):
        """
        Callback funtion that responds when the cliend succefully
        connects to the mqtt server
        """
        print("[INFO] Connected With Result Code: " + str(rc))
    
    def callback_on_message(self, client, userdata, message):
        """
        Callback function that prints the message published
        on the topic.
        """
        print("--- Subscriber ---")
        print("[INFO] Topic: {}".format(message.topic) )
        print("[INFO] Message Recieved: {}".format(message.payload.decode()))
        print("------------")

        
obj_publisher = pub_mqtt("protium/switchStatus/led1")




