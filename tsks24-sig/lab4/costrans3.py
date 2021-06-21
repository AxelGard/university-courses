#-----------------------------------------------------------------------
# Program for studying the relation between the discrete cosine
# transform and the discrete fourier transform.
# Written by Maria Magnusson 2003-05-04
# Updated by Maria Magnusson 2007-04-30
# Ported to Python by Michael Felsberg 2016-03-01
#-----------------------------------------------------------------------

import numpy as np
from matplotlib import pyplot as plt
import cv2
a = np.load('images/goldhill128.npy')			# inimage
N = 128						# inimage size
b = np.vstack((np.hstack((a, a[:,::-1])), np.hstack((a[::-1,:], a[::-1,::-1]))))
# mirror inimage
u, v = np.meshgrid(np.arange(-N,N),np.arange(-N,N))
# (u,v) coordinate system

# factors to compensate for translation in the spatial domain
Ev = np.exp(-0.5j*v*np.pi/N)
Eu = np.exp(-0.5j*u*np.pi/N)

A = cv2.dct(a)					# discrete cosine transform

AA = np.fft.fftshift(np.fft.fft2(a))		# FFT
AA = AA/N;                              	# extra scaling

B = np.fft.fftshift(np.fft.fft2(b))		# fft + weighting
B = B*Ev*Eu
B[N,:] = B[N,:]/np.sqrt(2)
B[:,N] = B[:,N]/np.sqrt(2)
B = B/(2*N)
imagB = np.max(np.imag(B))			# check that imag part is 0
B = np.real(B)

# image limits
IMLIM = (0, np.max(a))
lim = np.max(A)/100
FLIM = (0, lim)

# show inimage, its FFT and its DCT
plt.figure(8)
plt.subplot(221), plt.imshow(a, 'gray', clim=IMLIM)
plt.title('original image, a(x,y)'), plt.colorbar()
plt.subplot(222), plt.imshow(np.abs(AA), 'gray', clim=FLIM)
plt.title('AA(u,v) = abs(FFT[a(x,y)])'), plt.colorbar()
plt.subplot(223), plt.imshow(np.abs(A), 'gray', clim=FLIM)
plt.title('A(u,v) = abs(DCT[a(x,y)])'), plt.colorbar()

# show inimage and mirrored inimage
# show fft + weighting of mirrored inimage and show lower right part
plt.figure(9)
plt.subplot(221), plt.imshow(a, 'gray', clim=IMLIM)
plt.title('original image, a(x,y)'), plt.colorbar()
plt.subplot(222), plt.imshow(b, 'gray', clim=IMLIM)
plt.title('mirrored image, b(x,y)'), plt.colorbar()
plt.subplot(223), plt.imshow(np.abs(B), 'gray', clim=FLIM)
plt.title('B(u,v) = almost abs(FFT[b(x,y)])'), plt.colorbar()
BPART = B[N:,N:]
plt.subplot(224), plt.imshow(np.abs(BPART), 'gray', clim=FLIM)
plt.title('lower right part of B(u,v)'), plt.colorbar()
plt.show()

print(f"Maximum difference: {np.max(A-BPART)}")	    	   	 # check that they are equal
