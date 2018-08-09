#!/usr/bin/python3
# _*_ coding: UTF-8 _*_
import serial
import sys

serial_name='/dev/ttyUSB0'
if(len(sys.argv)==2):
    serial_name=sys.argv[1]
ser=serial.Serial(serial_name,9600,timeout=1);
min_coord=[-50,-50,0,-10]
max_coord=[2000,50,290,95]

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
	coord_file=open("coord.txt",'r')
	coord_lines=coord_file.readlines()
	coord_file.close()

	if(content[0]=='x'):
		content,coord_lines[0]= update(content,0,coord_lines)
	elif(content[0]=='y'):
		content,coord_lines[1]= update(content,1,coord_lines)
	elif(content[0]=='z'):
		content,coord_lines[2]= update(content,2,coord_lines)
	elif(content[0]=='j'):
		content,coord_lines[3]= update(content,3,coord_lines)
	elif(content[0]=='q'):
		content='q0'
	else:
		print("invalid command.")
	print("x:%d y:%d z:%d j:%d"%(int(coord_lines[0][:-1]),
									int(coord_lines[1][:-1]),
									int(coord_lines[2][:-1]),
									int(coord_lines[3][:-1])))
	#unicode to utf-8
	ser.write(content.encode(encoding = "utf-8"))
	# response=ser.readall().decode(encoding="utf-8")
	# print ("I heared:%s"%(response),end='')
	# if(response[-4:-2]=="sh"):
	# 	pass
	# else:
	response=""
	response=ser.readall().decode(encoding="utf-8")
	while(response==""):
		response=ser.readall().decode(encoding="utf-8")
	if(response[0:3]=="fin"):
		print("finish")
	else:
		sys.exit(1)

	coord_file=open("coord.txt",'w')
	coord_file.writelines(coord_lines)
	coord_file.close()

commands=[
	#"mz0020",
	#"mx-020",
	#0/0/30/95
	"x0175",
	"z0200",
	"x-040",
	"j-90",  #夹菜
	"z-230", #抬升
	"x0940", #移向切刀
	"z0290", #向下
	"q",
	"z-260",
	"q",
	"x0600",
	"j090",
	"x-1675"
]
	# "x0375",
	# "z0200",
	# "x-040",
	# "j-90",  #夹菜
	# "z-230", #抬升
	# "x0740", #移向切刀
	# "z0290", #向下
	# "q",
	# "z-260",
	# "q",
	# "x0600",
	# "j090",
	# "x-1675"

# while(True):
# 	command=input("enter:")
# 	moveOneStep(command)
for i in range(len(commands)):
	moveOneStep(commands[i])
print("bye")
ser.close()
