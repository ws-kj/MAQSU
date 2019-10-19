import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style

#open serial port for reciever
ser = serial.Serial('/dev/ttyACM0', 9600)

#set up figure for plotting
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)

#set up lines
tplus = []

mq135 = []
mq7   = []
mq5   = []

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
def animate(i, tplus, mq135, mq7, mq5):
    #get data from data_processor
    packet = readSerial()
    print(packet)
    tplus.append(packet[0])
    mq135.append(packet[1])
    mq7.append(packet[2])
    mq5.append(packet[3])

    #plot data
    ax.clear()
    #ax.plot(tplus)
    ax.plot(tplus, mq135, label="MQ-135")
    ax.plot(tplus, mq7, label="MQ-7")
    ax.plot(tplus, mq5, label="MQ-5")

    #add legend
    plt.legend(loc='upper left')

    ax.set_ylim(ymin=0.0, ymax=2.0)
    plt.xticks(rotation=45, ha='right')
    plt.title("Sensor Output")
    plt.ylabel("MQ Sensor Relative Value")
    plt.xlabel("Time (s)")

anim = animation.FuncAnimation(fig, animate,
        fargs=(tplus, mq135, mq7, mq5), interval=2000)
plt.show()




