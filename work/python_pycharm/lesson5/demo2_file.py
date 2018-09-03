'''
文件操作
'''
import os
import shutil
# f=open("text.txt","r")#打开不存在的文件会报错
# f.close() #切记要关闭
'''
#先用写的模式创建一个文件//文件不存在则创建
f=open("text.txt","w",encoding="utf-8") #会覆盖已存在的内容。encoding="utf-8"解决中文乱码
f.write("你好")  #不指定编码格式，会输出乱码
f.close()
'''
'''
f = open("f://test.txt","w",encoding="utf-8")
f.write("你好 python")
f.close()
'''
#read读文件
'''
f = open("text.txt","r",encoding="utf-8")
print(f.read())
f.close()
'''
#
# f = open("text.txt","a",encoding="utf-8")
# f.write("我好")
# f.write("\n大家好")
# f.close()

#读多行的方法

#按行读取全部数据
#readlines 按行全部读取文件数据，返回一个文件数据列表，每一行是列表的一个元素
'''
f = open("text.txt","r",encoding="utf-8")
data = f.readlines()
print(data)
print("-----------")
i = 1
for line in data:
    # print("第{}行：{}".format(i,line))  #print 默认有换行符，两个换行符就多换了一行
    print("第{}行：{}".format(i, line),end="")
    i+=1
f.close()
'''
#readline
'''
f = open("text.txt","r",encoding="utf-8")
line1=f.readline()
print(line1,end="")
line2=f.readline()
print(line2,end="")
line3=f.readline()
print(line3,end="")
f.close()
'''
#writelines向文件写入一个字符串序列
'''
f = open("text.txt","w",encoding="utf-8")
f.writelines(["张三\n","李四\n","王五\n"])
f.close()
'''
#
#os.mkdir("f://testdir123")
#print(os.getcwd())#获取目前程序运行的目录
#print(os.listdir("f://"))#查看指定目录下的文件列表
#os.rmdir("f://testdir123")#删除一个文件夹,如果删除有文件的文件夹，会报错
#os.rmdir("f://text1")
#shutil.rmtree("f://text1")
'''
#getcwd获取当前路径也可以通过
path = os.getcwd()#程序运行的当前路径
print(path)
# os.chdir("../")#切换到上级目录
# path = os.getcwd()
# print(path)
os.chdir("f://")#切换到上级目录
path = os.getcwd()
print(path)
'''












