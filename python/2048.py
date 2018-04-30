#coding:utf-8

'''
author: yee
support: python-3.5
modules: tkinter
crteate: 2017-5-1
update: 2017-12-30

a little game 2048 
'''

from tkinter import *   # import tkinter
import tkinter.messagebox as messagebox
import random

class Application(Frame):       #inherit
    def __init__(self,master=None):
        Frame.__init__(self,master)
        self.pack()
        self.createWigets()

    def createWigets(self):
        self.unFrame=Frame(self,relief='sunken',padx=5,pady=5,height=400,width=400,bg='#FFFFFF',)
        self.labels=[0]*16
        self.textDisplay=[0]*16
        for i in range(16):
                self.textDisplay[i]=StringVar()
                self.labels[i]=Label(self.unFrame,relief='raised',height=4,width=8,\
                                     font='Verdana 12 bold',bg='#9e948a',fg='#FFFFFF')
                self.labels[i]['textvariable']=self.textDisplay[i]
                self.textDisplay[i].set(str(i+1))
                r=int(i/4)
                c=i%4
                self.labels[i].grid(row=r,column=c,padx=5,pady=5)
        self.unFrame.pack()
        self.bind_all('<Key-W>',self.printW)
        self.bind_all('<Key-A>',self.printA)
        self.bind_all('<Key-S>',self.printS)
        self.bind_all('<Key-D>',self.printD)
        self.bind_all('<Key-Return>',self.printkey)
    def printA(self,event):
        for j in range(4):
            a=['']*4
            for i in range(4):
                a[i]=self.textDisplay[j*4+i].get()
            self.moveEnd(a,0)
            self.plusNeibor(a)
            self.moveEnd(a,0)
            for i in range(4):
                self.textDisplay[j*4+i].set(a[i])
        self.randomCreate()
        pass 
        pass
    def printD(self,event):
        for j in range(4):
            a=['']*4
            for i in range(4):
                a[3-i]=self.textDisplay[j*4+i].get()
            self.moveEnd(a,0)
            self.plusNeibor(a)
            self.moveEnd(a,0)
            for i in range(4):
                self.textDisplay[j*4+i].set(a[3-i])
        self.randomCreate()
        pass
    def randomCreate(self):
        a=[self.textDisplay[i].get() for i in range(16) if (self.textDisplay[i].get()=='')]
        length=len(a)
        if length==0:
            messagebox.showinfo('Game over!')
            return      
        num=random.randint(1,length)
        b=0
        for i in range(16):
            if self.textDisplay[i].get()=='':
                b=b+1
                if b==num:
                    self.textDisplay[i].set(str(2))
                    
    def printS(self,event):
        for j in range(4):
            a=['']*4
            for i in range(4):
                a[3-i]=self.textDisplay[i*4+j].get()
            self.moveEnd(a,0)
            self.plusNeibor(a)
            self.moveEnd(a,0)
            for i in range(4):
                self.textDisplay[i*4+j].set(a[3-i])
        self.randomCreate()
        
    def printW(self,event):
        for j in range(4):
            a=['']*4
            for i in range(4):
                a[i]=self.textDisplay[i*4+j].get()
            self.moveEnd(a,0)
            self.plusNeibor(a)
            self.moveEnd(a,0)
            for i in range(4):
                self.textDisplay[i*4+j].set(a[i])
        self.randomCreate()
        pass 
    def printkey(self,event):
        messagebox.showinfo('Game Begin','Enter--> start \nW: up  A: left   S: down  D: right  ')
        num=random.randint(1,16)
        key=event.keysym
        for i in range(16):
                self.textDisplay[i].set('')
        self.textDisplay[num-1].set(str(2))
 
    def moveEnd(self,nums,s_num):
        #s_num,起始索引
        n=len(nums)
        if n-s_num==1:
            return
        str=''
        for i in range(s_num,n):
            str=str+nums[i]
        #judge if or not to go on
        if str=='' :
            return
        elif nums[s_num]!='' :
            self.moveEnd(nums,s_num+1)     
        else:
            self.moveNext(nums,s_num)
            self.moveEnd(nums,s_num)
            
#move to next
    def moveNext(self,nums,s_num):
        n=len(nums)
        for i in range(s_num,n-1):
            nums[i]=nums[i+1]
        nums[n-1]=''
    
    #plus
    def plusNeibor(self,nums):
        if nums[0]=='':
            return
        elif nums[0]==nums[1]:
            nums[0]=str(int(nums[0])*2)
            nums[1]=''
            if (nums[2]==nums[3])&(nums[2]!=''):
                nums[2]=str(int(nums[2])*2)
                nums[3]=''
        elif (nums[1]==nums[2])&(nums[1]!=''):
            nums[1]=str(int(nums[1])*2)
            nums[2]=''
        elif (nums[2]==nums[3])&(nums[2]!=''):
            nums[2]=str(int(nums[2])*2)
            nums[3]=''

app=Application()
app.master.title('Enjoy 2048')
app.master.geometry('500x400')
app.mainloop()

