import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style

#open serial port for reciever
ser = serial.Serial('/dev/ttyACM0', 9600)

#set up figure for plotting
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
xs = []
ys = []

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
def animate(i, xs, ys):
    #get data from data_processor
    packet = readSerial()

    print(packet)

    xs.append(packet[0])
    ys.append(packet[1])

    xs = xs[-50:]
    ys = ys[-50:]

    ax.clear()
    ax.plot(xs, ys)

    plt.xticks(rotation=45, ha='right')
    plt.title("Pollutant Sensor Relative Output")
    plt.ylabel("MQ-135")


anim = animation.FuncAnimation(fig, animate, fargs=(xs, ys), interval=2000)
plt.show()




