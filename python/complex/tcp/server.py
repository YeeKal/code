#!/usr/bin/env python3
#_*_ coding:utf-8 _*_
import sys
import threading
from socket import *
import time
import opera
import serial
import signal


class Server(object):
    def __init__(self,port):
        self.thread_list=[]
        self.buffsize=2048
        self.host=''
        self.port=port
        self.ss=socket(AF_INET,SOCK_STREAM) 
        self.arduino=opera.Arduino()
        self.initializeSocket()
    def initializeSocket(self):
        ADDR=(self.host,self.port)
        self.ss=socket(AF_INET,SOCK_STREAM)
        self.ss.bind(ADDR)
        self.ss.listen(3)
    def acceptSocket(self):
        while True:
            print("wait for connection ...")
            ss_recv,addr=self.ss.accept()
            print("connection from:",addr)
            thread_recv=threading.Thread(target=self.recvThread,args=(ss_recv,))
            self.thread_list.append(thread_recv)
            thread_recv.start()
            thread_send=threading.Thread(target=self.sendThread,args=(ss_recv,))
            self.thread_list.append(thread_send)
            thread_send.start()
    

    def sendThread(self,ss_recv):
        while True:
            data=chr(self.arduino.states)+   \
                "{:0>4d}".format(self.arduino.coords[0])+   \
                "{:0>4d}".format(self.arduino.coords[1])+   \
                "{:0>4d}".format(self.arduino.coords[2])+   \
                "{:0>4d}".format(self.arduino.coords[3])+   \
                "{:0>1d}".format(self.arduino.row_num)
            ss_recv.send(('%s' % (data,)).encode(encoding="utf-8"))
            time.sleep(0.5)

        ss_recv.close()

    def recvThread(self,ss_recv):
        while True:
            data = ss_recv.recv(self.buffsize).decode(encoding="utf-8")
            if not data:
                break
            else:
            
                thread_command=threading.Thread(target=self.commandThread,args=(data,))
                thread_command.start()


        ss_recv.close()
    def commandThread(self,command):
        self.arduino.runCommand(command)

# def exitServer(signum,frame):
#     for item in thread_list:
#         #item.stop()
#         pass
#     arduino.closeSerial()
#     sys.exit(0)
#     print("quit.")
    
# signal.signal(signal.SIGINT,exitServer)
# signal.signal(signal.SIGTERM,exitServer)

if __name__ == "__main__":
    port=2000
    if(len(sys.argv)==2):
        port=int(sys.argv[1])
    server=Server(port)
    server.acceptSocket()
#ss_recv.close()
