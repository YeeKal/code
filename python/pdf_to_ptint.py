#！/usr/bin/python3
#_*_ coding: utf-8 _*_

'''
author: yee
support: python-3.6
modules: PyPDF2
crteate: 2017-5-11
update: 2018-4-30

Rearray pdf pages to make it printable as double-2 print to make a book.
Add blank pages to make sure the pdf pages num must be 4-multiple.
Before execution, revise the pdf path.
'''

from PyPDF2 import PdfFileWriter,PdfFileReader

def toPrint(outPdf,inPdf):
    outFile=PdfFileWriter()
    #open source
    print("file loading......")
    inputStream=open(inPdf,"rb")
    inFile=PdfFileReader(open(inPdf,"rb"))
    
    #page number
    pageNum=inFile.getNumPages()
    quarNum=int(pageNum/4)
    halfNum=int(pageNum/2)
    print("the num of this file is:%d" %pageNum)

    #page transfer
    pages=inFile.pages
    for i in range(quarNum):
        print("the %d cycles"%i)
        outFile.addPage(pages[(quarNum-i-1)*2+halfNum+1])
        outFile.addPage(pages[i*2+0])
        outFile.addPage(pages[i*2+1])
        outFile.addPage(pages[(quarNum-i-1)*2+halfNum])

    #page output
    print("all files added")
    outputStream=open(outPdf,"wb")
    outFile.write(outputStream)
    outputStream.close() 
    inputStream.close()

#change the file path before execution
pre_path="preArrange.pdf"
aft_path="aftArrange.pdf"

toPrint(pre_path,aft_path) 
    
    
