import time
import RPi.GPIO as GPIO
import wiringpi as gpio
import math

out_pins=[3,5,7,8,10,12,11,13,15,16]
pwm_pin=3
dir_pin=5
ena_pins=[7,8,10,12]
relay_pins=[11,13,15]
enaqie_pin=16
read_pin=18

#variant
stepx_to_pulse=0.13125#step length each pulse 
stepy_to_pulse=0.2625
stepz_to_pulse=0.015625
stepjia_to_pulse=0.00625
#define min_steps 300


class Raspberry(object):
    def __init__(self):
        GPIO.setmode(GPIO.BOARD)
        GPIO.setup(out_pins,GPIO.OUT,initial=GPIO.HIGH)
        GPIO.setup(read_pin,GPIO.IN,pull_up_down=GPIO.PUD_DOWN)
        self.start_time=0
        self.min_steps=300
        self.min_fre=100
        self.constant_k=4
        self.periods=[0]*300
        self.dir=True
    def moveCom(self,command):
        item=command[0]
        disp=int(command[1:])
        if(command[0]=='x'):
            GPIO.output(ena_pins[0],GPIO.LOW)
            GPIO.output(relay_pins[0],GPIO.LOW)
            self.stepper(1600,disp,stepx_to_pulse)
            GPIO.output(relay_pins[0],GPIO.HIGH)
            GPIO.output(ena_pins[0],GPIO.HIGH)
        if(command[0]=='y'):
            GPIO.output(ena_pins[1],GPIO.LOW)
            GPIO.output(relay_pins[1],GPIO.LOW)
            self.stepper(500,disp,stepx_to_pulse)
            GPIO.output(relay_pins[1],GPIO.HIGH)
            GPIO.output(ena_pins[1],GPIO.HIGH)
        if(command[0]=='z'):
            GPIO.output(ena_pins[2],GPIO.LOW)
            GPIO.output(relay_pins[2],GPIO.LOW)
            self.stepper(2700,disp,stepx_to_pulse)
            GPIO.output(relay_pins[2],GPIO.HIGH)
            GPIO.output(ena_pins[2],GPIO.HIGH)
        if(command[0]=='j'):
            GPIO.output(ena_pins[3],GPIO.LOW)
            self.stepper(1600,disp,stepx_to_pulse)
            GPIO.output(ena_pins[3],GPIO.HIGH)
        if(command[0]=='q'):
            self.moveQie()
    def stepper(self,fre,disp,step_to_pulse):
        self.smodeLine(self.min_fre,fre,self.min_steps,self.constant_k)
        if disp<0:
            self.dir=not self.dir
        GPIO.output(dir_pin,self.dir)
        steps=disp/step_to_pulse
        if steps<=2*self.min_steps:
            start_steps=int(steps/2)
            finish_steps=int(steps-start_steps)
            for i in range(start_steps):
                GPIO.output(pwm_pin,GPIO.LOW)
                self.delayTime(periods[i])
                GPIO.output(pwm_pin,GPIO.HIGH)
                self.delayTime(periods[i])
            for i in range(finish_steps):
                GPIO.output(pwm_pin,GPIO.LOW)
                self.delayTime(periods[i])
                GPIO.output(pwm_pin,GPIO.HIGH)
                self.delayTime(periods[i])
        else:
            for i in range( self.min_steps):
                GPIO.output(pwm_pin,GPIO.LOW)
                self.delayTime(periods[i])
                GPIO.output(pwm_pin,GPIO.HIGH)
                self.delayTime(periods[i])
            time_delay=1000000/(2*fre)
            for i in range(int(steps-2*self.min_steps)):
                GPIO.output(pwm_pin,GPIO.LOW)
                self.delayTime(time_delay)
                GPIO.output(pwm_pin,GPIO.HIGH)
                self.delayTime(time_delay)
            for i in range( self.min_steps):
                GPIO.output(pwm_pin,GPIO.LOW)
                self.delayTime(periods[self.min_steps-1-i])
                GPIO.output(pwm_pin,GPIO.HIGH)
                self.delayTime(periods[self.min_steps-1-i])
            dir =True

    def moveQie(self):
        if self.judge():
            GPIO.output(enaqie_pin,GPIO.LOW)
            while self.judge():
                pass
            GPIO.output(enaqie_pin,GPIO.HIGH)
        elif not self.judge():
            GPIO.output(enaqie_pin,GPIO.LOW)
            while not self.judge():
                pass
            GPIO.output(enaqie_pin,GPIO.HIGH)
    def smodeLine(self,min_fre,max_fre,len,k):
        for i in range(len):
            deno=min_fre+(max_fre-min_fre)/(1+math.exp(-k*(i-len/2)/(len/2)))
            self.periods[i]=1000000/deno

    def delayTime(self,micros):
        while (gpio.micros()-self.start_time)<micros:
            pass
    def judge(self):
        if GPIO.input(read_pin):
            time.sleep(0.0005)
            if GPIO.input(read_pin):
                return True
        elif not GPIO.input(read_pin):
            time.sleep(0.0005)
            if not GPIO.input(read_pin):
                return False
    




raspberry=Raspberry()
while(True):
    com=input("cmd:")
    raspberry.moveCom(com)
    