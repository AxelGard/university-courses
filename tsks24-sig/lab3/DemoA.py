from preamble import *

Im = np.double(imageio.imread('images/baboon.tif'))

fig, axes = plt.subplots(nrows=2, ncols=2)
fig.tight_layout()

plt.subplot(2, 2, 1)
plt.imshow(Im, 'gray', clim=(0,255))
plt.title('original image')
plt.colorbar()

def on_press(event):
    x = int(round(event.xdata));
    y = int(round(event.ydata));
    print(f"{x},{y} = {Im[y, x]}")

fig = plt.gcf()
fig.canvas.mpl_connect('button_press_event', on_press)

plt.subplot(2, 2, 2)
plt.imshow(Im, 'gray', clim=(50, 200))
plt.title('contrast image')
plt.colorbar()

# QUESTION: 1
print(Im)
print("min =", np.min(Im)) # min =  2.0
print("max =", np.max(Im)) # max =  207.0


# QUESTION: 2
# (x,y) 62,18 = 99.0 (Index)

graycmap = plt.get_cmap('gray', 256)
gray_vals = graycmap(np.arange(256))
print(gray_vals)
print("\n\n")

# QUESTION: 3
# values are shown as floats instead of ints (so 2 in slides equals 2 / 255 here).
# Also there is a fourth column which is opacity (alpha channel)

gray_vals[200:] = [1, 0, 0, 1]
plt.register_cmap('ngray', graycmap.from_list('ngray', gray_vals))
print(gray_vals)

plt.subplot(2, 2, 3)
plt.imshow(Im, 'ngray', clim=(0, 255))
plt.title('red upper range image')
plt.colorbar()
plt.show()

# QUESTION: 4
# Every point that have a value over 200 becomes fully red. So the points
# that were very white in previous images become red.
