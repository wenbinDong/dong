'''
函数使用
'''
'''
#局部变量
# def set_name():
#     name = "zhangsan"
#     return  name
# #如何让一个函数使用另一个函数的局部变量
# def get_name(name):
#     # name ="liosi"
#     print(name)
#
# nm=set_name()
# get_name(nm)
'''
'''
#全局变量
# name="zhangsan"
# def get_name():
#     print(name)
# def get_name2():
#     print(name)
#     
# get_name()
# get_name2()
'''

'''
#修改全局变量的值
#函数体内修改全局变量：使用global关键字
age = 20
def change_age():
    # age = 25
    global age
    age = 25
    print("函数体内age=%d"%age)

change_age()
print("函数体外age=%d"%age)
'''

'''
#函数使用的变量在函数调用之前定义就行
g_num1 = 100
def print_global_num():
    print("g_numl:%d"%g_num1)
    print("g_num2:%d"%g_num2)
    print("g_num3:%d"%g_num3)
g_num2 = 200
print_global_num()
g_num3=300
'''
'''
#字典，列表作为全局变量，在函数内修改这种类型的全局变量中的元素
#不需要使用global关键字，可直接修改
g_num_list =[1,2,3]
g_info_dict = {"name":"zhangsan","age":20}
def update_info():
    g_num_list.append(4)
    g_info_dict["gender"]= "male"

update_info()
print(g_info_dict)
print(g_num_list)
'''
#缺省参数,缺省参数一定在固定参数之后
def x_y_sum(x,y=20):
    print("x=%d"%x)
    print("y=%d"%y)
    return x+y
#错误的缺省定义函数
'''
# def x_y_sum(x=10,y):
#     return x+y'''

# rsl = x_y_sum(10)
# print(rsl)
rsl = x_y_sum(10,30)
print(rsl)
