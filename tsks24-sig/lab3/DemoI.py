from preamble import *

Im = np.load('images/pattern.npy')

fig, axes = plt.subplots(nrows=2, ncols=2)
fig.tight_layout()

plt.subplot(2, 2, 1)
plt.imshow(Im, 'gray', clim=(-1,1))
plt.title('original image')

Imsample = Im[::2,::2]
plt.subplot(2, 2, 2)
plt.imshow(Imsample, 'gray', clim=(-1,1))
plt.title('downsampled image')

# QUESTION: 16
# Because of the high frequencies at the top of the image we get more aliasing there
# as the pixels change color fast. Meaning when we sample we get a blur.

b = np.array([0.5, 0.5])
b2 = np.convolve(b, b).reshape(1, 3)
aver = signal.convolve2d(b2, b2.T)

Imaver = Im
for _ in range(5):
    Imaver = signal.convolve2d(Imaver,aver,'same')

plt.subplot(2, 2, 3)
plt.imshow(Imaver, 'gray', clim=(-1,1))
plt.title('LP filtered image')

Imsample = Imaver[::2,::2]
plt.subplot(2, 2, 4)
plt.imshow(Imsample, 'gray', clim=(-1,1))
plt.title('LP-filtered & downsampled')
plt.show()


# QUESTION: 17
# Around 5 repeated filter application.
# Because the lines are thick, the first filtering only removes the outer layer
# of one line and we need to filter repeatedly to remove the lines completely.
