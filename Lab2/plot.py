import matplotlib.pyplot as plt
import numpy as np

# Int value with blocking
blocking_ijk=[0.030000 ,0.140000 ,0.630000 ,5.620000 ,46.980000]
blocking_ikj=[0.030000 ,0.100000 ,0.540000 ,4.390000 ,36.280000]
blocking_jik=[0.030000 ,0.150000 ,0.650000 ,5.850000 ,67.930000]
blocking_jki=[0.010000 ,0.090000 ,0.960000 ,9.360000 ,228.500000]
blocking_kij=[0.030000 ,0.110000 ,0.580000 ,4.770000 ,50.840000]
blocking_kji=[0.010000 ,0.090000 ,0.960000 ,9.760000 ,229.350000]

x_axis=[pow(2,i) for i in range (7,11)]
# print(x_axis[:-1])
plt.plot(x_axis,blocking_ijk[:-1],color='red',marker='o')
plt.plot(x_axis,blocking_ikj[:-1],color='orange',marker='o')
plt.plot(x_axis,blocking_jik[:-1],color='black',marker='o')
plt.plot(x_axis,blocking_jki[:-1],color='blue',marker='o')
plt.plot(x_axis,blocking_kij[:-1],color='pink',marker='o')
plt.plot(x_axis,blocking_kji[:-1],color='green',marker='o')
plt.grid(b = True, color ='grey', linestyle ='-.', linewidth = 0.5,
        alpha = 0.6)
plt.legend(['ijk','ikj','jik','jki','kij','kji',])
plt.xlabel('Log (Matrix Dimension)')
plt.ylabel('Time (in s)')
plt.show()