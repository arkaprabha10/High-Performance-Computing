import matplotlib.pyplot as plt
import math
# %matplotlib inline
file1 = open('results.txt', 'r') 
# file1 = open('data_28_new.txt', 'r')
# file1 = open('data_28_new.txt', 'r')
file2=open('add.txt','r')
file3=open('div.txt','r')
count = 0
a=[]
# a=""
y_axis=[]
x_axis=[]
y_axis_a=[]
x_axis_a=[]
y_axis_d=[]
x_axis_d=[]
# while count<21: 
#     count += 1 
line = file1.readline() 
    # a.append(line.strip())
temp=line.strip()
    # b,a=temp.split(',')
a=temp.split(',')
# print(a)
    # x_axis.append(math.log(int(b),2))
for i in range(0,len(a)-1):
    y_axis.append(float(a[i]))
    # y_axis.append(float(a))
x_final=[i for i in range(8,29)]
    # print(a,b)
    
    # if line is empty 
    # end of file is reached 
    # if not line: 
    #     break
    # # # print("Line{}: {}".format(count, line.strip())) 
# count=0
# while count<13:
#      count += 1
     
#      line2 = file2.readline()
#      line3 = file3.readline()
#      temp2=line2.strip()
#      temp3=line3.strip()
#     #  print(temp2)

#      a,b=temp2.split(',')
#      c,d=temp3.split(',')

#      x_axis_a.append(math.log(int(b),2))
#      y_axis_a.append(float(a))
#      y_axis_d.append(float(c))

#      if not line2: 
#         break

file1.close()
# file2.close()
# file3.close() 
# print(x_axis)
# print(y_axis)
plt.figure(1)
plt.plot(x_final,y_axis,'orange', marker="x")
plt.xlabel('Log2 (Loop size)')
plt.ylabel('Compute throughput in Flops/s')
# plt.plot(x_axis,y_axis,'r',marker="x")
plt.grid()
plt.show()

# plt.figure(2)
# plt.plot(x_axis_a,y_axis_a,'r')
# plt.plot(x_axis_a,y_axis_d,'g')
# plt.grid()
# plt.show()
# # print(a.split(','))