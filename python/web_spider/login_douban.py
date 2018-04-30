#coding:utf-8
import re
from urllib import request,error

url='https://accounts.douban.com/login'
user_agent='''Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36\
        (KHTML, like Gecko) Chrome/57.0.2987.133 Safari/537.36'''
email='yanggang@sjtu.edu.cn'
password='521forever'
headers={
    'User-Agent':user_agent,
    'form_email':email,
    'form_password':password,
    'redir':'https://www.douban.com/people/98216703/',
    }

req=req=request.Request(url,headers=headers)
try:
    res=request.urlopen(req)
except error.URLError as e:
    print(e.reason)
finally:
    print('that is fine')
text=res.read().decode('utf-8')
print(len(text))
pattern=(r'<title>(.*?)</title>')
nickname=re.search(pattern,text,re.S)
print("welcome",nickname.group(1))

