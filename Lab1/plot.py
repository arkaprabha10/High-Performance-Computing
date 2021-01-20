import matplotlib.pyplot as plt
import math
# %matplotlib inline
file1 = open('data.txt', 'r') 
file2=open('add.txt','r')
file3=open('div.txt','r')
count = 0
# a=[]
a=""
y_axis=[]
x_axis=[]
y_axis_a=[]
x_axis_a=[]
y_axis_d=[]
x_axis_d=[]
while count<22: 
    count += 1
  
    # Get next line from file 
    line = file1.readline() 
    # a.append(line.strip())
    temp=line.strip()
    a,b=temp.split(',')
    x_axis.append(math.log(int(b),2))
    y_axis.append(float(a))

    # print(a,b)
    
    # if line is empty 
    # end of file is reached 
    if not line: 
        break
    # print("Line{}: {}".format(count, line.strip())) 
count=0
while count<13:
     count += 1
     
     line2 = file2.readline()
     line3 = file3.readline()
     temp2=line2.strip()
     temp3=line3.strip()
    #  print(temp2)

     a,b=temp2.split(',')
     c,d=temp3.split(',')

     x_axis_a.append(math.log(int(b),2))
     y_axis_a.append(float(a))
     y_axis_d.append(float(c))

     if not line2: 
        break

file1.close()
file2.close()
file3.close() 
# print(x_axis)
# print(y_axis)
# plt.figure(1)
# plt.plot(x_axis,y_axis)
# plt.grid()
# plt.show()

plt.figure(2)
plt.plot(x_axis_a,y_axis_a,'r')
plt.plot(x_axis_a,y_axis_d,'g')
plt.grid()
plt.show()
# print(a.split(','))