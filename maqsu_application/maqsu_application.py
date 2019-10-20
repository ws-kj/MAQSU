import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import matplotlib.ticker as plticker
from matplotlib import style

#open serial port for reciever
ser = serial.Serial('/dev/ttyACM0', 9600)

#set up figure for plotting
fig = plt.figure()
mqax = plt.subplot(2, 1, 1)
dhtax = plt.subplot(2, 1, 2)

#set up lines
tplus = []

mq135 = []
mq7   = []
mq5   = []
temp  = []
hum   = []

#turn serial input into usable floats
def readSerial():
    values = []
    packet = []

    while 1:
        #convert bytes from serial to string
        ser_bytes = ser.readline()
        decoded_bytes = str(ser_bytes[0:len(ser_bytes)-2].decode("utf-8"))
        if(decoded_bytes == ""):
            for d in values:
                packet.append(float(d))
            break
        else:
            values.append(decoded_bytes)

    return packet

#func to add data to graph
def animate(i, tplus, mq135, mq7, mq5, temp, hum):
    #get data from data_processor
    packet = readSerial()
    tplus.append(packet[0])
    mq135.append(packet[1])
    mq7.append(packet[2])
    mq5.append(packet[3])
    temp.append(packet[4])
    hum.append(packet[5])

    #plot data
    mqax.clear()
    dhtax.clear()

    #plot lines
    mqax.plot(tplus, mq135, label="MQ-135")
    mqax.plot(tplus, mq7, label="MQ-7")
    mqax.plot(tplus, mq5, label="MQ-5")

    dhtax.plot(tplus, temp, label="Temp (F)")
    dhtax.plot(tplus, hum, label="Humidity")

    #add legend
    mqax.legend(loc='upper left')
    dhtax.legend(loc='upper left')

    #limit displayed range to static value
    mqax.set_ylim(ymin=0.0, ymax=2.0)
    dhtax.set_ylim(ymin=0.0, ymax=100.0)

    #set ticker range
    dhtloc = plticker.MultipleLocator(5.0)
    dhtax.yaxis.set_major_locator(dhtloc)

    #set labels
    mqax.set_ylabel("Pollutant Sensor Relative Value")
    mqax.set_xlabel("Time (s)")

    dhtax.set_ylabel("Weather Data")
    dhtax.set_xlabel("Time (s)")

anim = animation.FuncAnimation(fig, animate,
        fargs=(tplus, mq135, mq7, mq5, temp, hum), interval=2000)
plt.show()




