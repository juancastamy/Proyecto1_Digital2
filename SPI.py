 # Import standard python modules.
import spidev
import sys
import time

# Import Adafruit IO MQTT client.
from Adafruit_IO import MQTTClient

# inicializamos SPI
spi = spidev.SpiDev()
#especificamos a qie slave nos vamos a comunicar
spi.open(0,0)
# especificamos velocidad de comunicacion
spi.max_speed_hz = 7629
spi.mode = 0b00

while True:
    temp = spi.xfer([0x00])
   # time.sleep(.5)
    parq = spi.xfer([0x01])
   # time.sleep(.5)
    luz = spi.xfer([0x02])
   # time.sleep(.5)
    presion = spi.xfer([0x03])
   # time.sleep(.5)
    ultra = spi.xfer([0x04])
   # time.sleep(.5) 

    print('ultrasonico',temp)
    print('temperatura',parq)
    print('parqueos',luz)
    print('luz',presion)
    print('presion',ultra)
