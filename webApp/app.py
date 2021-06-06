# import libraries
# import libraries
from flask import Flask, render_template, request, jsonify
import paho.mqtt.client as mqtt
import threading as t
import time


app = Flask(__name__)


# Global variables
json = {
    'led1' : 'OFF',
    'led2' : 'OFF',
    'fan' : 'OFF',
    'plug' : 'OFF'
}



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
        global json
        """
        Callback function that prints the message published
        on the topic.
        """
        print("--- Subscriber ---")
        print("[INFO] Topic: {}".format(message.topic) )
        print("[INFO] Message Recieved: {}".format(message.payload.decode()))
        print("------------")

        if (self.sub_topic == 'protium/led1/switchStatus') and (json['led1'] != message.payload.decode()):
            json['led1'] = message.payload.decode()
        elif (self.sub_topic == 'protium/led2/switchStatus') and (json['led2'] != message.payload.decode()):
            json['led2'] = message.payload.decode()
        elif (self.sub_topic == 'protium/fan/switchStatus') and (json['fan'] != message.payload.decode()):
            json['fan'] = message.payload.decode()
        elif (self.sub_topic == 'protium/plug/switchStatus') and (json['plug'] != message.payload.decode()):
            json['plug'] = message.payload.decode()

def establish_connection_led1():
    obj_sub_led1 = sub_mqtt('protium/led1/switchStatus')

def establish_connection_led2():
    obj_sub_led2 = sub_mqtt('protium/led2/switchStatus')

def establish_connection_fan():
    obj_sub_fan = sub_mqtt('protium/fan/switchStatus')

def establish_connection_plug():
    obj_sub_plug = sub_mqtt('protium/plug/switchStatus')


@app.route('/control', methods=['GET', 'POST'])
def control():
    global json
    # Communication code here...
    
    if request.method == 'GET':
        return jsonify(json)
    else:
        new_state = ""
        data = request.get_json()  # parse as JSON
        if json['led1'] != data['led1']:
            obj_pub_led1 = pub_mqtt('protium/led1/changeStatus')
            json['led1'] = data['led1']
            new_state = data['led1']
            obj_pub_led1.publish_msg(new_state)
            
        
        if json['led2'] != data['led2']:
            obj_pub_led2 = pub_mqtt('protium/led2/changeStatus')
            json['led2'] = data['led2']
            new_state = data['led2']
            obj_pub_led2.publish_msg(new_state)
        
        if json['fan'] != data['fan']:
            obj_pub_fan = pub_mqtt('protium/fan/changeStatus')
            json['fan'] = data['fan']
            new_state = data['fan']
            obj_pub_led1.publish_msg(new_state)
        
        if json['plug'] != data['plug']:
            obj_pub_plug = pub_mqtt('protium/plug/changeStatus')
            json['plug'] = data['plug']
            new_state = data['plug']
            obj_pub_plug.publish_msg(new_state)

        print(json)
        return 'OK', 200

@app.route('/protium')
def protium():
    thrd1 = t.Thread(target = establish_connection_led1)
    thrd2 = t.Thread(target = establish_connection_led2)
    thrd3 = t.Thread(target = establish_connection_fan)
    thrd4 = t.Thread(target = establish_connection_plug)
    thrd1.start()
    thrd2.start()
    thrd3.start()
    thrd4.start()
    # look inside template and serve index.html
    return render_template('index.html')

if __name__ == "__main__":
    app.run(debug = True)