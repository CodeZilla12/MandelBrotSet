import matplotlib.pyplot as plt
import numpy as np


with open("mbdata.txt") as f:
    data = f.read()
    lines = [list(map(int,i.split(","))) for i in data.split("\n")[1:] if i] #parses all lines from file and cast as int

rmin,rmax,imin,imax = [float(i) for i in data.split("\n")[0].split(",")]

plt.xlabel("Real")
plt.ylabel("Imag")
plt.imshow(np.rot90(lines),extent=[rmin,rmax,imin,imax], aspect = 1, vmin = 0, vmax = 80)
plt.show()
