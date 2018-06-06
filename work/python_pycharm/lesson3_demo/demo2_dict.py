'''
字典
'''
#列表
user_info_list=["悟空",100,"male","取经"]
#字典
user_info_dict={"name":"悟空","age":100,"gender":"male","job":"取经"}
# user_info_dict["job"]="取经|偷桃"
# print(user_info_dict)
#字典不支持下标查询值的，
# print("%s的年龄是：%d，性别是：%s，工作内容:%s"%(user_info_dict["name"],user_info_dict["age"],user_info_dict["gender"],user_info_dict["job"]))
#且key不能重复出现，后出现的值会覆盖前面的值
# user_info_dict={"name":"悟空","age":100,"gender":"male","job":"取经","job":"偷桃"}
# print(user_info_dict)
#字典支持增删改查
user_info_dict["tel"]="13831073111"
# print(user_info_dict)
#len查看字典的有多少个key
# print(len(user_info_dict))
#修改
# user_info_dict["tel"]="13888888888"
# print(user_info_dict)
#删除
# del user_info_dict["tel"]
# print(user_info_dict)
# del user_info_dict["tel"]
# print(user_info_dict["tel"])
# 为了防止出现异常导致程序崩溃，通常会对要判查询的key在字典中是否存在做一个判断
# if "tel" in user_info_dict:
#     print(user_info_dict["tel"])
# else:
#     print("tel不存在")
#使用内置的get函数，查找一个不存在的键
# print(user_info_dict.get("tel"))
# print(user_info_dict.get("name"))
#使用get查询比if in更简洁
# print(user_info_dict.get("tel","10010"))
#第二个值为默认值，如果键不存在，我们就给他设置一个默认值，
# 如果不设置默认值就会返回空类型
#常出现在数据处理过程中，字段没有值时，给予一个默认值
#字典里内置的key方法,即查询key，key获取字典中所有的key
# for key in user_info_dict.keys():
#     print("{}:{}".format(key,user_info_dict[key]))
#获取value的方法，value获取字典中所有的values
# for value in user_info_dict.values():
#     print(value)
#items方法使用
# for item in user_info_dict.items():
#     print(type(item))
#     print(item)
#     print(item[0])#键key
#     print(item[1])#value
# for key,value in user_info_dict.items():
#     print(key)
#     print(value)
#clear() 清空字典
# user_info_dict.clear()
# print(user_info_dict)
