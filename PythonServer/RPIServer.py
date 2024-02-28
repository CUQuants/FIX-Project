import socket
import time
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)

bufferSize=1024
ServerIP='128.138.51.253'
ServerPort=2222
RPIServer=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)#sets up raspberry pi server
RPIServer.bind((ServerIP,ServerPort))#bind server
print('Server Up and Listening . . .')
while True:
    cmd,address=RPIServer.recvfrom(bufferSize)
    cmd=cmd.decode('utf-8')
    print(cmd)
    print('Client Address', address[0])
    if cmd=='GO':
        #he reads from sensors here that I honestly couldn't give a shit about
        data = "result:good"
        data = data.encode('utf-8')
        RPIServer.sendto(data, address)
    else:
        data = 'Invalid Request'
        data=data.encode('utf-8')
        RPIServer.sendto(data,address)
        
        