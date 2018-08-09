import serial
import sys
import copy
import time

class Arduino(object):
	
	def __init__(self):
		self.ser=serial.Serial('/dev/ttyUSB0',9600,timeout=1);
		self.coord_file_dir="coord.txt"
		self.min_coord=[0,-50,0,0]
		self.max_coord=[2000,50,290,90]
		self.running=False 
		self.running_error=False  
		self.allow_pick_rows=True   
		self.coords=[0]*4
		self.states=0
		self.row_num=0      #0 means not picking
		self.rows_all=2
		self.start_row=[
			{"x":175},
			{"z":230},
			{"x":135},
			{"j":0},
			{"z":0},
			{"x":1075},
			{"z":290},
			{"q":0},
			{"z":30},
			{"q":0},
			{"x":1675},
			{"j":90},
			{"x":0}
		]
		self.row_states=[1,1,2,2,3,4,4,5,6,6,7,8,9]
		self.row_states_fake=self.row_states#=[1,1,2,2,3,4,4,6,7,8,9]
		self.readCoord()
	   
	def runCommand(self,command):
		if(command[0:5]=="Start"):
			print("Start")
			self.reset()
			self.pickRows()
		elif(command[0:3]=="End"):
			print("End")
			self.reset()
		elif command[0:2]=="MQ":
			cmd="q0"
			self.moveOneStep(cmd)
		elif(command[1]=="X"):
			cmd="x"+command[2:7]
			self.moveCoordStep(cmd)
		elif(command[1]=="Y"):
			cmd="y"+command[2:7]
			self.moveCoordStep(cmd)
		elif(command[1]=="Z"):
			cmd="z"+command[2:7]
			self.moveCoordStep(cmd)
		elif(command[1]=="H"):
			cmd="j"+command[2:7]
			self.moveCoordStep(cmd)
		else:
			print("nothing to do.")
	def moveCoordStep(self,command):
		self.states=11
		if self.running:
			return
		self.running=True
		self.moveOneStep(command)
		self.running=False
		self.states=0
	def reset(self):
		#these steps has to be executed
		self.states=10
		self.allow_pick_rows=False  #end start
		while self.running:			#wait for each step to stop
			pass
		self.running=True
		commands=[{"y":0},{"z":30},{"x":0},{"j":90}]
		for i in range(len(commands)):
			self.moveToCoord(commands[i])
		self.allow_pick_rows=True
		self.running=False
		self.states=0
	def pickRows(self):
		if self.running:
			return
		self.running=True
		row_width=100
		row_commands=[]
		for i in range(self.rows_all):
			row_command=copy.deepcopy(self.start_row)
			row_command[0]["x"]=row_command[0]["x"]+i*row_width
			row_command[2]["x"]=row_command[2]["x"]+i*row_width
			print("%d  %d"%(i,row_command[0]["x"]))
			row_commands.append(row_command)
		for i in range(len(row_commands)):
			self.row_num=i+1
			for j in range(len(self.start_row)):
				if not self.allow_pick_rows:
					self.row_num=0
					self.running=False
					return
				self.states=self.row_states_fake[j]
				self.moveToCoord(row_commands[i][j])
		
		self.row_num=0
		self.running=False
		self.states=0
	def moveToCoord(self,coord_dict):
		p=list(coord_dict.keys())[0]
		num=self.getNum(p)
		if num=="":
			if p=='q':
				content="q0"
			else:
				return
		else:
			content=p+str(list(coord_dict.values())[0]-self.coords[num])
		self.moveOneStep(content)
			
	def getNum(self,coord_name):
		if coord_name=="x":
			return 0
		elif coord_name=="y":
			return 1
		elif coord_name=="z":
			return 2
		elif coord_name=="j":
			return 3
		else:
			return ""
		
	def update(self,content,num):
		pos=self.coords[num]
		new_coords=self.coords
		move_i=int(content[1:])  ##add judge
		if((pos+move_i)<self.min_coord[num]):
			print("Warning: out of range.")
			move_i=self.min_coord[num]-pos
			pos=self.min_coord[num]
			content=content[0]+str(move_i)+"."
		elif((pos+move_i)>self.max_coord[num]):
			print("<Warning: out of range>")
			move_i=self.max_coord[num]-pos
			pos=self.max_coord[num]
			content=content[0]+str(move_i)+"."
		else:
			pos +=move_i
			content=content+"."
		print("move:%d"%(move_i))
		new_coords[num]=pos
		return content,new_coords

	def moveOneStep(self,content):
		if self.running_error:
			print("runniing going error")
			return
		self.readCoord()

		num=self.getNum(content[0])
		if num=="":
			if content[0]=='q':
				content="q111."
				new_coords=self.coords
			else:
				return
				print("invalid command.")
		else:
			content,new_coords= self.update(content,num)
	   
			
		print("x:%d y:%d z:%d j:%d"%(new_coords[0],
									new_coords[1],
									new_coords[2],
									new_coords[3]))
		#unicode to utf-8
		
		if(int(content[1:-1])==0):
			print("finish")
			return
		self.ser.flushInput()
		self.ser.flushOutput()
		self.ser.write(content.encode(encoding = "ascii"))
		print("command:%s"%content)
		response=""
		try:
			while(response=="" ):

				#k=self.ser.readline()
				k=self.ser.readline()
				# k.replace(b'\xbb','')
				# k.replace(b'\xfb','')
				# k.replace(b'\xef','')
				response=k[0:3].decode(encoding="ascii")
				finish_time=time.clock()
			self.ser.flushInput()
			self.ser.flushOutput()
		except UnicodeDecodeError as e:
			print("error:%s"%str(e))
			self.coords=new_coords
			self.writeCoord()
		finally:
			print("k:%s"%k)
		if(response[0:3]=="111"):
			self.coords=new_coords
			self.writeCoord()
			print("finish")
		elif response[0:2]=='he':#未执行
			print("not executed:%s"%response)
			self.moveOneStep(content)
		else:  #不可抵抗的错误
			print(response)
			# self.coords=new_coords
			# self.writeCoord()
			print("我是一个神奇的错误")
			#self.running_error=True
	def readCoord(self):
		coord_file=open(self.coord_file_dir,'r')
		for i in range(4):
			self.coords[i]=int(coord_file.readline()[:-1])
		coord_file.close()
	def writeCoord(self):
		coord_file=open(self.coord_file_dir,'w')
		coord_lines=[]
		for i in range(4):
			coord_lines.append(str(self.coords[i])+"\n")
		coord_file.writelines(coord_lines)
		coord_file.close()

	def closeSerial(self):
		self.ser.close()

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




	
	
	