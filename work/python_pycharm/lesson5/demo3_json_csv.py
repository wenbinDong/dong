'''
json和csv格式文件处理
'''
import json
import csv
#dumps和loads
json_dict = {"name":"zhangsan","age":20,"language":["python","java"],"study":{"AI":"python","bigdata":"hadoop"},"if_vip":True}
'''
json_str = json.dumps(json_dict)
print(json_str)
print(type(json_str))
python_data = json.loads(json_str)
print(python_data)
print(type(python_data))
'''
#dump和load
# with open("f://user_info.json","w") as f:  #文件操作完自动调用close方法关闭
#     json.dump(json_dict,f)
'''
with open("f://user_info.json","r") as f:
    user_info_data = json.load(f)
    print(user_info_data)
    print(type(user_info_data))
'''

#csv文件操作
#向csv文件写数据
'''

datas = [["name","age"],["zhangsan",20],["lisi",30]] #第一个列表元素表示csv文件的标题
with open("f://user_info_csv.csv","w",newline="",encoding="utf_8") as f:
    writer = csv.writer(f)
    # for row in datas:
        #一次写入一行
    #     writer.writerow(row)
        #一次写入多行
    #writer.writerows(datas)
'''
#从csc文件读数据
'''
with open("f://user_info_csv.csv","r",newline="",encoding="utf_8") as f:
    reader = csv.reader(f)
    header = next(reader)
    print(header)
    print("----------")
    for row in reader:
        print(row)
        print(row[0])
        print(row[1])
'''
header = ["name","age"] #数据会随着标题改变 key会与标题进行匹配
rows = [{"name":"zhangsan","age":20},{"name":"lisi","age":30},{"name":"wangwu","age":18}]
'''
with open("f://user_info_csv_dict.csv","w",newline="",encoding="utf_8") as f:
    writer = csv.DictWriter(f,header)
    writer.writeheader()
    writer.writerows(rows)
'''
with open("f://user_info_csv_dict.csv","r",newline="",encoding="utf_8") as f:
    reader = csv.DictReader(f)
    for row in reader:
        print(row)
        print("name:{},age:{}".format(row["name"],row["age"]))


















