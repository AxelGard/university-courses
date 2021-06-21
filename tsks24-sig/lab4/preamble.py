import numpy as np
from scipy import signal, misc, ndimage
import cv2
from matplotlib import pyplot as plt
plt.rcParams['image.interpolation'] = 'nearest'
import jpeglab as jl

class Error: # Also exists in preamble
    def __init__(self, original, altered):
        self.mse = np.mean((original-altered)**2)
        self.psnr = 10*np.log10(255**2/self.mse) # peak signal to noise ratio
