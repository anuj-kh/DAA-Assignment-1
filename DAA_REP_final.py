import numpy as np
import matplotlib.pyplot as plt
f = open("./rectangle_file.txt", "r")
c=int(f.readline())
for i in range(c):
    index=0
    x=np.array(np.zeros(5))
    y=np.array(np.zeros(5))
    l = f.readline().split()
    xl=float(l[0])
    yb=float(l[1])
    l = f.readline().split()
    xr=float(l[0])
    yt=float(l[1])
    x[index]=xl
    y[index]=yb
    index=index+1
    x[index]=xl
    y[index]=yt
    index=index+1
    x[index]=xr
    y[index]=yt
    index=index+1
    x[index]=xr
    y[index]=yb
    index=index+1
    x[index]=xl
    y[index]=yb
    plt.scatter(x,y)
    plt.plot(x,y,color='red')
#plt.show()
f = open("./contour_file.txt", "r")
c=int(f.readline())
for i in range(c):
    a = f.readline().split()
    x = [a[0] ,a[2]]
    y = [a[1] , a[3]]
    plt.scatter(x,y)
    plt.plot(x,y,color='green')
plt.show()