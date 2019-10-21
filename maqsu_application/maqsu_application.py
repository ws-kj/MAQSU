import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import matplotlib.ticker as plticker
import matplotlib.gridspec as  gridspec
from matplotlib import style

#open serial port for reciever
ser = serial.Serial('/dev/ttyACM0', 9600)

#set up figure for plotting
gs = gridspec.GridSpec(2, 2)

fig = plt.figure()
mqax = plt.subplot(gs[0, 0:])
dhtax = plt.subplot(gs[1, 0])
mplax = plt.subplot(gs[1, 1])
#set up lines
tplus = []

mq135 = []
mq7   = []
mq5   = []
temp  = []
hum   = []
alt   = []

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
def animate(i, tplus, mq135, mq7, mq5, temp, hum, alt):
    #get data from data_processor
    packet = readSerial()
    tplus.append(packet[0])
    mq135.append(packet[1])
    mq7.append(packet[2])
    mq5.append(packet[3])
    temp.append(packet[4])
    hum.append(packet[5])
    alt.append(packet[6])

    #plot data
    mqax.clear()
    dhtax.clear()
    mplax.clear()

    #plot lines
    mqax.plot(tplus, mq135, label="MQ-135")
    mqax.plot(tplus, mq7, label="MQ-7")
    mqax.plot(tplus, mq5, label="MQ-5")

    dhtax.plot(tplus, temp, label="Temp (F)")
    dhtax.plot(tplus, hum, label="Humidity")

    mplax.plot(tplus, alt, label="Altitude (Metres)")

    #add legend
    mqax.legend(loc='upper left')
    dhtax.legend(loc='upper left')
    mplax.legend(loc='upper left')

    #limit displayed range to static value
    mqax.set_ylim(ymin=0.0, ymax=2.0)
    dhtax.set_ylim(ymin=0.0, ymax=100.0)

    #set ticker range
    dhtloc = plticker.MultipleLocator(5.0)
    dhtax.yaxis.set_major_locator(dhtloc)

    mplloc = plticker.MultipleLocator(1.0)
    mplax.yaxis.set_major_locator(dhtloc)

    #set labels
    mqax.set_ylabel("Pollutant Sensor Relative Value")
    mqax.set_xlabel("Time (s)")

    dhtax.set_ylabel("Weather Data")
    dhtax.set_xlabel("Time (s)")

    mplax.set_ylabel("Altitude (Metres)")
    mplax.set_xlabel("Time (s)")

anim = animation.FuncAnimation(fig, animate,
        fargs=(tplus, mq135, mq7, mq5, temp, hum, alt), interval=2000)
plt.show()




