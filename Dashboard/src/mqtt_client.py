import paho.mqtt.client as mqtt
from queue import Queue

# Define the queue to store received messages
message_queue = Queue()

# The callback for when the client receives a connection response from the server.
def on_connect(client, userdata, flags, rc):
    print(f"Connected with result code {rc}")
    client.subscribe("your/topic")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(f"Received message: {msg.payload.decode()}")
    message_queue.put(msg.payload.decode())

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("mqtt.eclipse.org", 1883, 60)

# Start the MQTT client in a separate thread
client.loop_start()
