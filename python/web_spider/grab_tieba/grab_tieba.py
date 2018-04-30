# coding:utf-8
import re
from urllib import request,error

class Bdtb:
    def __init__(self,baseUrl,idcode,pattern):
        self.baseUrl=baseUrl
        self.idcode=idcode
        self.pagenum=1
        self.pattern=pattern
        self.pagelen=1

    def getPage(self):
        url=self.baseUrl+self.idcode+str(self.pagenum)
        try:
            response=request.urlopen(url)
            pages=response.read().decode('utf-8')
            return pages
        except error.URLErroe as e:
            print("connection failed.%s"%e.reason)
            return None
        
    def getPagelen(self,pages):
         pagelen=re.search(r'<li class="l_reply_num".*?<span.*?<span.*?>(\d+?)</',pages,re.S)
         return int(pagelen.group(1))
    def getComment(self,pages):
        comments=re.findall(self.pattern,pages,re.S)
        return comments
    def start(self):
        print('量子力学吧，按任意键查看新评论，Q退出······')
        self.enable=True   
        while (self.pagenum<=self.pagelen)&( self.enable):
            pages=self.getPage()
            self.pagelen=self.getPagelen(pages)
            comments=self.getComment(pages)
            print('第%d页  评论：%d'%(self.pagenum,len(comments)))
            printNum=input('是否保存本页（是，press y）:')
            if printNum==('y'or'Y'):
                f=open('page%d.txt'%self.pagenum,'w+')
                f.write('第%d页  评论：%d\n'%(self.pagenum,len(comments)))
                for i in comments:
                    f.write("\n发布人：%10s%16s%8s\n%s\n"%(i[2],i[0],i[1],(self.replaceTool(i[3]) if self.replaceTool(i[3])!='' else '[img]')) )
                f.close()
                print('save successfully!')
            for i in comments:
                command=input()
                if command=='Q':
                    self.enable=False
                    return
                print("发布人：%10s%16s%8s\n%s\n"%(i[2],i[0],i[1],(self.replaceTool(i[3]) if self.replaceTool(i[3])!='' else '[img]')) )
            self.pagenum+=1
    def replaceTool(self,x):
        removeImg=re.compile('<img.*?>')
        removeAddr = re.compile('<a.*?>|</a>')
        #把换行的标签换为\n
        replaceLine = re.compile('<tr>|<div>|</div>|</p>')
        #将表格制表<td>替换为\t
        replaceTD= re.compile('<td>')
        #把段落开头换为\n加空两格
        replacePara = re.compile('<p.*?>')
        #将换行符或双换行符替换为\n
        replaceBR = re.compile('<br><br>|<br>')
        #将其余标签剔除
        removeExtraTag = re.compile('<.*?>')
        x = re.sub(removeImg,"",x)
        x = re.sub(removeAddr,"",x)
        x = re.sub(replaceLine,"\n",x)
        x = re.sub(replaceTD,"\t",x)
        x = re.sub(replacePara,"\n    ",x)
        x = re.sub(replaceBR,"\n",x)
        x = re.sub(removeExtraTag,"",x)
        #strip()将前后多余内容删除
        return x.strip()

#初始化参量
baseUrl='http://tieba.baidu.com/p/4430659730'
idcode='?pn='
pattern=(r'date&quot;:&quot;(\d{4}-\d{2}-\d{2}).*?(\d{2}:\d{2})&quot.*?alog-group="p_author".*?blank">(.*?)</.*?id="post_content.*?d_post_content .*?j_d_post_content.*?clearfix">(.*?)</')
#初始化实例
quantum=Bdtb(baseUrl,idcode,pattern)
quantum.start()

