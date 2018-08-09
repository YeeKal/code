#!/usr/bin/python3
# _*_ coding: UTF-8 _*_
import serial
import sys

serial_name='/dev/ttyUSB0'
if(len(sys.argv)==2):
    serial_name=sys.argv[1]
ser=serial.Serial(serial_name,9600,timeout=1);
min_coord=[-50,-50,0,-10]
max_coord=[1700,50,290,95]

def update(content,num,coord_lines):
	i=int(coord_lines[num])
	move_i=int(content[1:])
	if((i+move_i)<min_coord[num]):
		print("Warning: out of range.")
		move_i=min_coord[num]-i
		i=min_coord[num]
		content=content[0]+str(move_i)+"."
	elif((i+move_i)>max_coord[num]):
		print("<Warning: out of range>")
		move_i=max_coord[num]-i
		i=max_coord[num]
		content=content[0]+str(move_i)+"."
	else:
		i +=move_i
		content=content+"."
	print("move:%d"%(move_i))
	coord_line=str(i)+"\n"
	return content,coord_line

def moveOneStep(content):
	

	if(content[0]=='q'):
		content='q111'
	content=content+"."

	#unicode to utf-8
	ser.write(content.encode(encoding = "ascii"))
	# response=ser.readall().decode(encoding="utf-8")
	# print ("I heared:%s"%(response),end='')
	# if(response[-4:-2]=="sh"):
	# 	pass
	# else:
	response=""
	# # #response=ser.readall().decode(encoding="utf-8")
	# while(response==""):
	# 	response=ser.readall().decode(encoding="ascii")
	while response=="":
		response=ser.readline().decode(encoding="ascii")
	ser.flushInput()
	ser.flushOutput()
	
	if(response[0:2]=="11"):
		print("finish")
	else:
		print(response,end='')
		sys.exit(1)

commands=[
	#"mz0020",
	#"mx-020",
	#0/0/30/95
	"x0180",
	"z0200",
	"x-040",
	"j-90",  #夹菜
	"z-230", #抬升
	"x0960", #移向切刀
	"z0290", #向下
	"q",
	"z-260",
	"q",
	"x0600",
	"j090",
	"x-1600"
]

while(True):
	command=input("enter:")
	moveOneStep(command)
# for i in range(len(commands)):
# 	moveOneStep(commands[i])
print("bye")
ser.close()
