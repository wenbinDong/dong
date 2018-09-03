'''
日期和时间
'''

import  datetime
import  time

#获取当前时间戳，单位秒
#print(time.time())

#获取当前时间的时间元组
#print(time.localtime(time.time()))
#指定一个时间戳表示本地时间
#print(time.localtime(1522555932))
#格式化当前时间元组
#print(time.strftime("%Y-%m-%d %H:%M:%S"))
#格式化指定时间元组
#print(time.strftime("%Y-%m-%d %H:%M:%S",time.localtime()))
#日期字符串转换成时间元组
#print(time.strptime("2018-04-12 20:17:30","%Y-%m-%d %H:%M:%S"))
#将时间元组转化成时间戳
#print(time.mktime(time.strptime("2018-04-12 20:17:30","%Y-%m-%d %H:%M:%S")))
'''
#sleep（秒）程序睡眠时间
start_time = time.time()
time.sleep(5)
end_time = time.time()
print(end_time-start_time)
'''
#datetime模块
#获取当前时间
#print(datetime.datetime.now())
#日期时间格式化
#print(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
'''
#计算时间差值
start_time=datetime.datetime.now()
time.sleep(2)
end_time=datetime.datetime.now()
print(end_time-start_time)
print((end_time-start_time).seconds)
#测试程序性能时间长短方法
'''
# ts = time.time()#当前时间戳
# print(ts)
# print(datetime.datetime.fromtimestamp(ts))
#计算昨天的日期
#获取今天的日期和时间
today = datetime.datetime.today()
print(today.strftime("%Y-%m-%d %H:%M:%S"))
timedelta = datetime.timedelta(days=1)
yesterday = today -timedelta
print(yesterday.strftime("%Y-%m-%d %H:%M:%S"))
