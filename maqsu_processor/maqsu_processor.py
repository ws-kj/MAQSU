import serial

#open serial port for reciever
ser = serial.Serial('/dev/ttyACM0', 9600)

#poll serial process
def readSerial():
    tempPacket = {}
    values = []
    while 1:
        #convert bytes from serial to string
        ser_bytes = ser.readline()
        decoded_bytes = str(ser_bytes[0:len(ser_bytes)-2].decode("utf-8"))

        #split data into temporary packet with format {tplus : val, val, val, val, val}
        if(decoded_bytes == ""):
            tempPacket = {values[0] : values[1:]}
            processData(tempPacket)
            values = []
            tempPacket = {}
        else:
            values.append(decoded_bytes)

#process temporary packet
def processData(tempPacket):
    key = list(tempPacket.keys())[0]

    #check if tplus- if not, add to log
    if(key[0].isdigit()):
        packet = {float(key) : []}

        #turn strings into floats
        for v in list(tempPacket.values())[0]:
            list(packet.values())[0].append(float(v))
        print(packet)
    else:
        print(key[0])

readSerial()
