# coding:utf-8
import re
from urllib import request,error

url='http://www.qiushibaike.com/hot//page/2/?s=4961479'
user_agent='Mozilla/4.0 (compatible; MSIE 5.5; Windows NT)'
request_set=request.Request(url,headers={'User-Agent':user_agent})
pattern_String=r'''class="author clearfix">.*?title="(?P<author>.*?)">
.*?content.*?<span>(?P<content>.*?)</'''
pattern=re.compile(pattern_String)
try:
    response=request.urlopen(request_set)    
except error.HTTPError as e:
    print(e.reason,e.code,e.headers,sep='\n')
finally:
    print("that's fine")

text=response.read().decode('utf-8')
print(len(text))
items=re.findall(pattern_String,text,re.S)
print(len(items))
for i in items:
    print("author:%s\n%s\n"%(i[0],i[1]))

