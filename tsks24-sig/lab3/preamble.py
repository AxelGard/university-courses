import numpy as np
from scipy import signal, misc
import imageio
from matplotlib import pyplot as plt
plt.rcParams['image.interpolation'] = 'nearest'

class Error:
    def __init__(self, original, altered):
        self.mse = np.mean((original-altered)**2)
        self.psnr = 10*np.log10(255**2/self.mse)
