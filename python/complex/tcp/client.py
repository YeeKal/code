#!/usr/bin/env python3
#-*- coding:utf-8 -*-

#scp yee@192.168.1.103:~/Desktop/complex/tcp/opera.py  .
#scp yee@192.168.1.103:~/Desktop/complex/stepper_slide_speed/stepper_slide_speed.ino .
import sys
import time
import threading
from socket import *
import signal

HOST ='localhost'
PORT = 5001
if(len(sys.argv)==3):
    HOST=sys.argv[1]
    PORT=int(sys.argv[2])

BUFFSIZE=2048

ADDR = (HOST,PORT)

client = socket(AF_INET,SOCK_STREAM)

client.connect(ADDR)
def exitServer(signum,frame):
    sys.exit(0)
    print("quit.")
    

signal.signal(signal.SIGINT,exitServer)
signal.signal(signal.SIGTERM,exitServer)

def recvThread(ss_recv):
    while True:
        data = ss_recv.recv(BUFFSIZE).decode(encoding="utf-8")
        #print(data)
        if not data:
            break
        coord_file=open("temp.txt",'a+')
        coord_file.write(data+"\n")
        coord_file.close()
    ss_recv.close()
def sendThread(ss_recv):
    while True:
        data=input("com:")
        ss_recv.send(data.encode(encoding="utf-8"))
       # time.sleep(1)
    ss_recv.close()

thread_recv=threading.Thread(target=recvThread,args=(client,))
thread_recv.start()
thread_send=threading.Thread(target=sendThread,args=(client,))
thread_send.start()
# while True:
#     data = input(">")
#     if not data:
#         break
#     client.send(data.encode())
#     data = tctimeClient.recv(BUFFSIZE).decode()
#     if not data:
#         break
#     print(data)
# tctimeClient.close()