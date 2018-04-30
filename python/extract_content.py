#!/usr/bin/python3
# coding:utf-8

'''
author: yee
support: python-3.5
create: 2018-4-24
update: 2018-4-24
function: extract the content in the directory and write it to a markdown file.
          this is initially encouraged by the README.md in github
'''

import os

FILTER_END_STRING=('.git','README.md') 

#the compare function in filter 
def cmp(*endString):
    def run(file):
        f=map(file.endswith,endString)
        if False in f:
            return file
    return run
#filter the file names
def filterFiles(files,*endString):
    f=cmp(FILTER_END_STRING)
    return filter(f,files)
#extract the content in a directory, return the markdown
def extractContent(prefix,file_path,depth):
    context=""
    files=os.listdir(file_path)
    files=filterFiles(files)   #filter the files 
    files.sort()                #sort the files by name
    
    #construct the hyperlink 
    if prefix=="":
        link=""
    else:
        link=prefix+"/"
    #space before the file name
    space="    "*depth
    for file in files:
        if os.path.isdir(file_path+"/"+file):
            context=context+space+"- [%s](%s)\n"%(file,link+file)+ \
                    extractContent(link+file,file_path+"/"+file,depth+1)
        elif os.path.isfile(file_path+"/"+file):
            context=context+space+"- [%s](%s)\n"%(file,link+file) #link
    return context


#base_path="/home/yee/Documents/github/learn_notes" #the abs-path
base_path=os.getcwd()           #get the current path
readme_file_name="README.md"    #file name to store the content
readme_file=open(base_path+"/"+readme_file_name,'w+')  #rewrite this file each time
readme_file.write(extractContent("",base_path,0))
readme_file.close()
print("content has been saved to:\n%s"%(base_path+"/"+readme_file_name))

'''
os.listdir(path)  #get all the files under the path and return a list
os.path.isdir(path)  #check if it is a directory or not
print("getcwd():%s"%os.getcwd())  #the path where this file is running
'''