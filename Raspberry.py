# importamos las librerias que utilizamos
import sys
import time
import spidev
from Adafruit_IO import MQTTClient
# inicializamos SPI
spi = spidev.SpiDev()
#especificamos a qie slave nos vamos a comunicar
spi.open(0,0)
# especificamos velocidad de comunicacion
spi.speed_max_hz = 1953000
#Definimos el modo del SPI
spi.mode = 0b11
#Lee la cantidad de bytes que queremos
while(1):
    Luz = spi.read()
    time.sleep(.1)
    byte1 = bin(Luz)[2:].rjust(8,'0')
    Temp = spi.read()
    time.sleep(.1)
    byte2 = bin(Temp)[2:].rjust(8,'0')
    Parq = spi.read()
    byte3 = bin(Parq)[2:].rjust(8,'0')
    time.sleep(0.1)

    print(byte1)
    print(byte2)
    print(byte3)
