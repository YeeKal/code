#coding:utf-8
#only cookie
import re
from urllib import request,error

url='https://www.taobao.com/'
cookie='thw=cn; _med=dw:1366&dh:768&pw:1366&ph:768&ist:0; v=0; _tb_token_=76bb37eef5ef8; uc1=cookie14=UoW%2Buf8n%2FFqKcA%3D%3D&lng=zh_CN&cookie16=VFC%2FuZ9az08KUQ56dCrZDlbNdA%3D%3D&existShop=false&cookie21=UtASsssmeW6lpyd%2BB%2B3t&tag=7&cookie15=WqG3DMC9VAQiUQ%3D%3D&pas=0; uc3=sg2=BdMx12kLGop8ChucVDtNcDj5F5JhR6x302B5%2BEYT8dY%3D&nk2=GgoRmt%2FuN94OcfcP%2FkaFMg%3D%3D&id2=UojRYhEekR33pg%3D%3D&vt3=F8dARVMZq6GJzonCMO8%3D&lg2=Vq8l%2BKCLz3%2F65A%3D%3D; existShop=MTQ5MTc0NDgxMQ%3D%3D; uss=UNN0necr0EZc%2FS1MJCknQPyRcP3Lfhk3rrggxD6CL9592TkFupYRvtX5RA%3D%3D; lgc=yanggang521fhuan; tracknick=yanggang521fhuan; cookie2=191aafbdd5908c7af3e55d159e106e91; sg=n9f; mt=np=; cookie1=BxFjUA8%2FP12kvKHH4SoYKA1cHaAWjIocuxq8UNkTeRg%3D; unb=1952483809; skt=0aa170404e07cd75; t=1e82adff60bc5e1c014f2be50a9b5db5; _cc_=Vq8l%2BKCLiw%3D%3D; tg=0; _l_g_=Ug%3D%3D; _nk_=yanggang521fhuan; cookie17=UojRYhEekR33pg%3D%3D; l=AlBQDX4FtB2s84yT8dMU8RZeoJSiXDRj; isg=AtPTBr-CjxApZ0PKllLVB9AeYlcFzWdKFVigvIXwavIpBPGmHFj3mjFUSMOQ; cna=Y51gESjn1lICAbSpPxGYC5n5'
headers={
    'cookie':cookie
    }
req=request.Request(url,headers=headers)
try:
    res=request.urlopen(req)
except error.URLError as e:
    print(e.reason)
finally:
    print('that is fine')


text=res.read().decode('utf-8')
pattern=(r'<title>(.*?)</title>')
nickname=re.search(pattern,text,re.S)
print(nickname.group(1))
print(len(text))
