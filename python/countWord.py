#coding:utf-8
'''
author: yee
support: python-3.6
create: 2017-12-30
update: 2017-12-30

count word frequency
'''
import re
import collections
from string import punctuation
#pdf reader
from pdfminer.converter import PDFPageAggregator
from pdfminer.layout import LTTextBoxHorizontal, LAParams
from pdfminer.pdfinterp import PDFResourceManager, PDFPageInterpreter
from pdfminer.pdfinterp import PDFTextExtractionNotAllowed
from pdfminer.pdfparser import PDFParser, PDFDocument

def count_word(content):
    result={}
##    with open(path) as file:
##        content=file.read()
##        #all to lower
    content=content.lower()
    #replace special letter
    content=re.sub(r'[{}]'.format(punctuation),' ',content )

    for word in content.split():
        if word not in result:
            result[word]=0
        result[word] +=1
    result=collections.OrderedDict(sorted(result.items(), key = lambda t: -t[1]))
    return result

punctuation +='“”‘’'
path = 'contract/friendship.pdf'
file_name=open(path,"rb")
note=open("contract/word.txt",'w+')


#创建一个与文档相关联的解释器
parser=PDFParser(file_name)

#PDF文档对象
doc=PDFDocument()

#链接解释器和文档对象
parser.set_document(doc)
doc.set_parser(parser)
#初始化文档
doc.initialize()
#创建PDF资源管理器
if not doc.is_extractable:
        raise PDFTextExtractionNotAllowed
else:
    # 创建PDf资源管理器 来管理共享资源
    rsrcmgr = PDFResourceManager()

    # 创建一个PDF参数分析器
    laparams = LAParams()

    # 创建聚合器
    device = PDFPageAggregator(rsrcmgr, laparams=laparams)

    # 创建一个PDF页面解释器对象
    interpreter = PDFPageInterpreter(rsrcmgr, device)

    # 循环遍历列表，每次处理一页的内容
    # doc.get_pages() 获取page列表
    result={}
    for page in doc.get_pages():
        # 使用页面解释器来读取
        interpreter.process_page(page)

        # 使用聚合器获取内容
        layout = device.get_result()

        # 这里layout是一个LTPage对象 里面存放着 这个page解析出的各种对象 一般包括LTTextBox, LTFigure, LTImage, LTTextBoxHorizontal 等等 想要获取文本就获得对象的text属性，
        for out in layout:
            # 判断是否含有get_text()方法，图片之类的就没有
            # if hasattr(out,"get_text"):
            if isinstance(out, LTTextBoxHorizontal):

                page = out.get_text()
                #print("results: " + page)
                content=page.lower()
                #replace special letter
                content=re.sub(r'[{},{}]+'.format(r'\d',punctuation),' ',content )
                #replace numbers
                #content=re.sub('\d+?',' ',content)

                for word in content.split():
                    if word not in result:
                        result[word]=0
                    result[word] +=1
                result=collections.OrderedDict(sorted(result.items(), key = lambda t: -t[1]))

dword=result
for key,value in dword.items():
    note.write("%-15s%d\n" % (key,value))
    if value>5:
        print("%-15s%d\n" % (key,value))


file_name.close()
note.close()
