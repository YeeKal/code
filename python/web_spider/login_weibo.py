# _*_ coding:utf-8 _*_
import re
from urllib import request,error

url='http://weibo.com/'


def login1():
    #use cookie
    user_agent='''Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36\
        (KHTML, like Gecko) Chrome/57.0.2987.133 Safari/537.36'''
    cookie='M_distinctid=15b13ab49d422-0098d78d68d3d1-b373f68-100200-15b13ab49d7a2; SINAGLOBAL=6648996588691.805.1490683650720; _s_tentry=cuiqingcai.com; Apache=4862140121036.172.1491731710966; ULV=1491731712994:5:4:1:4862140121036.172.1491731710966:1491621520892; _ga=GA1.2.315863807.1491735339; YF-Ugrow-G0=9642b0b34b4c0d569ed7a372f8823a8e; YF-V5-G0=b2423472d8aef313d052f5591c93cb75; YF-Page-G0=8eed071541083194c6ced89d2e32c24a; wb_publish_fist100_5211684676=1; login_sid_t=38524b7549d1a70dd48c87c173cd2ae5; WBStorage=02e13baf68409715|undefined; UOR=www.baidu.com,vdisk.weibo.com,login.sina.com.cn; SCF=Ah690Q0d_JBEwAymxCaLVzbwxJDYBPYIzidGRCGV40ICS0hI0gUrE_s7DhFhEnZNQhsAPdCWsicabofAY9pt2U8.; SUB=_2A2517lmXDeRhGeNM6lMX-CrKzDqIHXVWmsxfrDV8PUNbmtBeLXXQkW-J3LR743inwJ4h5pckycsIJzsDEw..; SUBP=0033WrSXqPxfM725Ws9jqgMF55529P9D9WWkyq6bqqvhhaQA_pQif6nW5JpX5K2hUgL.Fo-EeK2c1hBcS0q2dJLoIEXLxK-L12qLB--LxK-LB.BL1KeLxK-L1K2L1hqLxKMLB-zLBK-LxK.L1--L12zt; SUHB=0oUoNCqR0eXMFd; ALF=1523277122; SSOLoginState=1491741127; un=15121166382; wvr=6'
    #construct datas
    headers={
       # 'User-Agent':user_agent,
       # "Connection":'close',
       # 'Accept':'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8',
       #'Accept-Encoding':'gzip, deflate, sdch',
       # 'Accept-Language':'zh-TW,zh;q=0.8,en-US;q=0.6,en;q=0.4',
       # 'Host':'weibo.com',
        'cookie':cookie
        }
    req=request.Request(url,headers=headers)
    try:
        res=request.urlopen(req)
    except error.URLError as e:
        print(e.reason)
    finally:
        print('that is fine')
    return res

res=login1()
#res=login2()
text=res.read().decode('utf-8')
print(len(text))
pattern=(r'<title>(.*?)</title>.*?onick\'\]=\'(.*?)\'')
nickname=re.search(pattern,text,re.S)
print(nickname.group(1),'\n',nickname.group(2))



