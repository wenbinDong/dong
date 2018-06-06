'''
元组
'''
#定义元组
db_info =("192.168.10.1",3306,"root","root123")
# ip=db_info[0]
# port = db_info[1]
# print("ip={},port={}".format(ip,port))
#元组是不可修改的 db_info[1]=3308
#元组是不能够删除的
# del db_info[1]
#定义一个只有一个元素的元组
one_tuple = ("zhangsan",)
none_tuple =()#空元组
#使用循环遍历元组
# for item in db_info:
#     print(item)
i=0
while i<len(db_info):
    print(db_info[i])
    i+=1
#元组只支持查询