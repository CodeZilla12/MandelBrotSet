import matplotlib.pyplot as plt
import numpy as np

path = "C:\\Users\\samgl\\source\\repos\\CodeZilla12\\MandelBrotSet\\"

with open(path + "mbdata.txt") as f:
    data = f.read()
    lines = [list(map(int,i.split(","))) for i in data.split("\n")[1:] if i] #parses all lines from file and cast as int

rmin,rmax,imin,imax,depth = [float(i) for i in data.split("\n")[0].split(",")]

aspect = (imax-imin)/(rmax-rmin)

img = np.rot90(lines)
plt.imsave(path + "MBImg.tiff",img)
plt.imshow(img,extent=[rmin,rmax,imin,imax], aspect = 1, vmin = 0, vmax = depth)
plt.xlabel("Real")
plt.ylabel("Imag")
plt.colorbar(label = "# Iterations")
plt.show()
