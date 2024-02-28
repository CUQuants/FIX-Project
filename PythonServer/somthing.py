import socket
import time
serverAddress = ('128.138.51.253', 2222)
bufferSize=1024
UDPClient=socket.socket(socket.AF_INET, socket.SOCK_DGRAM)#uses udp, not what we want
#no bind on client side

# test = True
while True:
    t = time.localtime()
    current_time = time.strftime("%H:%M:%S", t)
    cmd=current_time
    cmd = cmd.encode('utf-8')
    UDPClient.sendto(cmd, serverAddress)
    data,address=UDPClient.recvfrom(bufferSize)
    data = data.decode('utf-8')
    #he parses the data, the world slept on
    print(data)
    
    # test = input("Keep testing?(Yes or any)")=="Yes"#I added this to be fun
    time.sleep(1)
