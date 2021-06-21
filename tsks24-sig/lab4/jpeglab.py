# JPEG lab module

import numpy as np
import sys
def rgb2ycbcr(r, g=0, b=0):
    if r.ndim == 3:
        g = r[:,:,1]
        b = r[:,:,2]
        r = r[:,:,0]
    y = 16. + (65.481*r + 128.553*g + 24.966*b)/255.
    cb = 128. + (-37.797*r - 74.203*g + 112.*b)/255.
    cr = 128. + (112.*r - 93.786*g - 18.214*b)/255.
    return (y, cb, cr)

def ycbcr2rgb(y, cb, cr):
    r = 1.164*y + 1.596*cr - 0.874
    g = 1.164*y - 0.392*cb - 0.813*cr + 0.532
    b = 1.164*y + 2.017*cb - 1.086
    return (r, g, b)

def im2col_distinct(A, blocksize):
	nrows, ncols = blocksize
	nele = nrows*ncols
	nrowspad = A.shape[0]//nrows+(A.shape[0]%nrows>0)
	ncolspad = A.shape[1]//ncols+(A.shape[1]%ncols>0)	
	A1 = np.zeros((nrows*nrowspad, ncols*ncolspad))
	A1[:A.shape[0], :A.shape[1]] = A
	t = np.swapaxes(A1.reshape(nrowspad,nrows,ncolspad,ncols),0,3)
	out = t.reshape(nele,-1)
	return (out)	
	
def col2im_distinct(A, blocksize, shp):
	nrows, ncols = blocksize
	nele = nrows*ncols
	nrowspad = shp[0]//nrows+(shp[0]%nrows>0)
	ncolspad = shp[1]//ncols+(shp[1]%ncols>0)	
	t = np.swapaxes(A.reshape(ncols,nrows,ncolspad,nrowspad),0,3)
	out = t.reshape(nrows*nrowspad, ncols*ncolspad)[:shp[0],:shp[1]]
	return (out)
	
def dct2basemx(block):			#Make a 2D DCT transform matrix
    m, n = block
    N = m*n
    X = np.arange(m)[np.newaxis,np.newaxis,:,np.newaxis]
    Y = np.arange(n)[np.newaxis,np.newaxis,np.newaxis,:]
    I = np.arange(m)[:,np.newaxis,np.newaxis,np.newaxis]
    J = np.arange(n)[np.newaxis,:,np.newaxis,np.newaxis]
    #Form from here: https://www.math.cuhk.edu.hk/~lmlui/dct.pdf, I've used numpys internal broadcasting to skip using meshgrids since they can be error-prone
    DCT_tensor = (2/np.sqrt(n*m)*np.sqrt(0.5)**(I==0) *np.sqrt(0.5)**(J==0)*(np.cos((2*X+1)*I*np.pi/(2*n))*np.cos((2*Y+1)*J*np.pi/(2*m))))
    DCT_matrix = DCT_tensor.reshape(n*m,n*m)#The tensor has shape i,j,x,y we reshape it to i*j,x*y
    return DCT_matrix
	
def bdct(im, M):				#bdct - block dct of image
    if isinstance(M,int):
        M =(M,M)
    dct_base_matrix = dct2basemx(M)
    column_im = im2col_distinct(im, M)
    t = np.dot(dct_base_matrix,column_im)

    return (t)

def ibdct(t, M, shp):				#bdct - block dct of image
	if isinstance(M,int):
		M =(M,M)
	if isinstance(shp,int):
		shp = (shp,shp)
	if M[0]*M[1] != t.shape[0]:
		sys.exit('The blocksize does not fit the transform image.')
	im = col2im_distinct(np.dot(dct2basemx(M).T,t), M, shp)
	return (im)

def bquant(t, qmtx):
	if isinstance(qmtx, int):
		tq = np.round(t/np.float(qmtx))# Only one quantization value
	else:
		qmtx = qmtx.reshape(-1,1)
		if t.shape[0] != qmtx.shape[0]:
			sys.exit('Wrong number of quantization values.')
		tq = np.round(t/np.kron(np.ones((1,t.shape[1])),qmtx.astype(float)))
	return (tq)
	
def brec(tq, qmtx):
	if isinstance(qmtx, int):
		t = tq*qmtx				# Only one quantization value
	else:
		qmtx = qmtx.reshape(-1,1)
		if tq.shape[0] != qmtx.shape[0]:
			sys.exit('Wrong number of quantization values.')
		t = tq*np.kron(np.ones((1,tq.shape[1])),qmtx.astype(float))
	return (t)
	
def jpgqmtx():
	Q=np.array([[ 16],
       [ 12],
       [ 14],
       [ 14],
       [ 18],
       [ 24],
       [ 49],
       [ 72],
       [ 11],
       [ 12],
       [ 13],
       [ 17],
       [ 22],
       [ 35],
       [ 64],
       [ 92],
       [ 10],
       [ 14],
       [ 16],
       [ 22],
       [ 37],
       [ 55],
       [ 78],
       [ 95],
       [ 16],
       [ 19],
       [ 24],
       [ 29],
       [ 56],
       [ 64],
       [ 87],
       [ 98],
       [ 24],
       [ 26],
       [ 40],
       [ 51],
       [ 68],
       [ 81],
       [103],
       [112],
       [ 40],
       [ 58],
       [ 57],
       [ 87],
       [109],
       [104],
       [121],
       [100],
       [ 51],
       [ 60],
       [ 69],
       [ 80],
       [103],
       [113],
       [120],
       [103],
       [ 61],
       [ 55],
       [ 56],
       [ 62],
       [ 77],
       [ 92],
       [101],
       [ 99]])
	return (Q)

def zigzag_indices(R=8,C=8):
    inds = []
    done = False
    row,col = 0,0
    direction = "down"
    while not done:
        inds.append(row*C+col)
        if row == 0:
            if col % 2 == 0:
                row,col = row+0,col+1
            else:
                row,col = row+1,col-1
                direction = "down"
        elif row == R-1:
            if col == C-1:
                done = True
            elif col % 2 == 0:
                row,col = row+0,col+1
            else:
                row,col = row-1,col+1
                direction = "up"
        elif col == 0:
            if row % 2 == 1:
                row,col = row+1,col+0
            else:
                row,col = row-1,col+1
                direction = "up"
        elif col == C-1:
            if row % 2 == 1:
                row,col = row+1,col+0
            else:
                row,col = row+1,col-1
                direction = "down"
        else:
            if direction == "down":
                row,col = row+1,col-1
            else:
                row,col = row-1,col+1
    return inds

def zigzag_matrix():
    Z = np.zeros((64,64))
    zigzag = zigzag_indices()
    Z[np.arange(64),zigzag] = 1
    return Z

def plot_zigzag_scan(R=8,C=8):
    import matplotlib.pyplot as plt
    inds = zigzag_indices()
    y = [(ind//C) for ind in inds]
    x = [ind%C for ind in inds]
    plt.plot(x,y)
    plt.ylim(R-0.5, -0.5)
    plt.show()
