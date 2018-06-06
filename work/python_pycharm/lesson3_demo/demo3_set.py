'''
集合set
'''
# student_set = {"zhangsan","lisi","wangwu"}
# print(type(student_set))
# print(len(student_set))
# print(student_set)
# id_list = ["id1","i、d2","id3","id1","id2"]
# distinct_set=set(id_list)
# print(distinct_set)#无序
# string_set=set("hello")#{'e', 'o', 'l', 'h'}
# print(string_set)
#创建一个新集合
# none_set = set()#空集合
# none_dict={}#创建一个空字典
# user_id="id5"
# if user_id in distinct_set:
#     print(user_id)
#
# if user_id not in distinct_set:
#     print("{}不存在".format(user_id))
#add添加元素到集合
# name_set ={"zhangsan","lisi"}
# name_set.add("wangwu")
# print(name_set)
#updata(序列) 把一个序列中每一个元素拆分，然后添加到集合里面
# name_set.update(["悟空","八戒"])
# print(name_set)
# name_set.update(["悟空","八戒"],["张飞","李逵"])
# print(name_set)
#remover（元素）
# name_set.remove("悟空")
# print(name_set)
#使用remove删除一个不存在的元素会报错
# name_set.remove("西游记")
# print(name_set)
#discard（元素)，删除一个不存在的元素不会报错
# name_set.discard("西游记")
#pop()随机删除集合中的某个元素，同时并返回删除的元素
# name = name_set.pop()
# print(name_set)
# print(name)
#交集
num_set1={1,2,4,7}
num_set2={2,5,8,9}
# inter_set1=num_set1 & num_set2
# inter_set2=num_set1.intersection(num_set2)
# print(inter_set1)
# print(inter_set2)
#并集
# union_set1=num_set1|num_set2
# union_set2=num_set1.union(num_set2)
# print(union_set1)
# print(union_set2)
#差集
# diff_set1=num_set1-num_set2
# diff_set2=num_set1.difference(num_set2)
# print(diff_set1)
# print(diff_set2)
#对称差集
sym_diff_set1 =num_set1^num_set2
sym_diff_set2 = num_set1.symmetric_difference(num_set2)
print(sym_diff_set1)
print(sym_diff_set2)

