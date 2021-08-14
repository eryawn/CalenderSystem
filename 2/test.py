import datetime
import time

cday = datetime.datetime.strptime('2015-6-1', '%Y-%m-%d')
print(cday)
a1 = datetime.datetime(1,1,1)

print(a1.strftime('%A   %B'))
#print(datetime.datetime.strftime('2018-11-09 14:42',' %B %A, %d %H:%M'))
a2 = datetime.datetime(2,2,2)
b1 = [a1,0]
b2 = [a2,0]
li = [b2,b1]
print(li)
li.sort()
print(li)
