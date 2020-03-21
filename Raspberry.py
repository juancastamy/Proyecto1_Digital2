# importamos las librerias que utilizamos
import sys
import time
import spidev
from Adafruit_IO import MQTTClient

#iniciamos el spi
spi = spidev.SpiDev()
#especificamos a que slave hablaremos
spi.open(0,0)
#velocidad de comunicacion
spi.max_speed_hz = 7629
spi.mode = 0b00

# Set to your Adafruit IO key.
# Remember, your key is a secret,
# so make sure not to publish it when you publish this code!
ADAFRUIT_IO_KEY = 'aio_GNDT51vVrI4T8mp5tn2bu3QFV0LS'

# Set to your Adafruit IO username.
# (go to https://accounts.adafruit.com to find your username)
ADAFRUIT_IO_USERNAME = 'JuanDiego979'


# Define callback functions which will be called when certain events happen.
def connected(client):
    # Connected function will be called when the client is connected to Adafruit IO.
    # This is a good place to subscribe to feed changes.  The client parameter
    # passed to this function is the Adafruit IO MQTT client so you can make
    # calls against it easily.
    print('Connected to Adafruit IO!  Listening for DemoFeed changes...')
    # Subscribe to changes on a feed named DemoFeed.
    client.subscribe('temp')
    client.subscribe('parq')
    client.subscribe('LUZ')
    client.subscribe('PRESION')
    client.subscribe('ULTRASONICO')

def disconnected(client):
    # Disconnected function will be called when the client disconnects.
    print('Disconnected from Adafruit IO!')
    sys.exit(1)

def message(client, feed_id, payload):
    # Message function will be called when a subscribed feed has a new value.
    # The feed_id parameter identifies the feed, and the payload parameter has
    # the new value.
    print('Feed {0} received new value: {1}'.format(feed_id, payload))


# Create an MQTT client instance.
client = MQTTClient(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

# Setup the callback functions defined above.
client.on_connect    = connected
client.on_disconnect = disconnected
client.on_message    = message

# Connect to the Adafruit IO server.
client.connect()

# Now the program needs to use a client loop function to ensure messages are
# sent and received.  There are a few options for driving the message loop,
# depending on what your program needs to do.

# The first option is to run a thread in the background so you can continue
# doing things in your program.
client.loop_background()
# Now send new values every 10 seconds.
print('Publishing a new message every 10 seconds (press Ctrl-C to quit)...')
while True:
   # temp = random.randint(0, 100)
    #print('Publishing {0} to DemoFeed.'.format(value))
    ultra = spi.xfer([0x00])
    byte1 = bin(ultra[0])[2:].rjust(8,'0')
    decimal1 = int (byte1, 2)
    client.publish('ULTRASONICO', decimal1)
    time.sleep(10)

    #parq = random.randint(0, 100)
    #print('Publishing {0} to DemoFeed.'.format(value))
    temp = spi.xfer([0x01])
    byte2 = bin(temp[0])[2:].rjust(8,'0')
    decimal2 = int(byte2,2)
    client.publish('temp', decimal2)
    time.sleep(10)

    #luz = random.randint(0, 100)
    #print('Publishing {0} to DemoFeed.'.format(value))
    parq = spi.xfer([0x02])
    byte3 = bin(parq[0])[2:].rjust(8,'0')
    decimal3 = int(byte3,2)
    client.publish('parq', decimal3)
    time.sleep(10)

    #presion = random.randint(0, 100)
    #print('Publishing {0} to DemoFeed.'. format(value))
    luz = spi.xfer([0x03])
    byte4 = bin(luz[0])[2:].rjust(8,'0')
    decimal4 = int(byte4,2)
    client.publish('LUZ', decimal4)
    time.sleep(10)
    
    #ultra = random.randint(0, 100)
    #print('Publishing {0} to DemoFeed.'.format(value))
    presion = spi.xfer([0x04])
    byte5 = bin(presion[0])[2:].rjust(8,'0')
    decimal5 = int(byte5,2)
    client.publish('PRESION', decimal5)
    time.sleep(10)
