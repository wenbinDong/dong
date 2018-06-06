'''
字符串常用内置方法
'''
#find
line="hello world hello python"
#hello 第一次出现的脚标
# print(line.find("hello"))
#指定查找的起始脚标
# print(line.find("hello",6))
#不存在的子字符串返回-1
# print(line.find("java"))
#cound
# print(line.count("hello"))
# print(line.count("python"))
#replace
# new_line = line.replace("hello","hi",1)
# print(new_line)
# new_line = line.replace("hello","hi")
# print(new_line)

#split
# split_list = line.split(" ")
# print(split_list)
# print(line)
#只分割第一个
# split_list = line.split(" ",1)
# print(split_list)
# print(line)

#startswith、endswith
# print(line.startswith("hello"))
# print(line.endswith("python"))
#应用场景
# files = ["20171201.txt","20171201.log","20180101.txt","20180101.log"]
# for file in files:
#     if file.startswith("2018") and file.endswith("log"):
#         print("2018年待处理日志：%s"%file)

#upper和lower
if_continue = input("是否继续购物，输入yes或no")
if if_continue.lower() == "yes":
    print("继续购物")
else:
    print("拜拜")

