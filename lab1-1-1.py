import random
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from tqdm import tqdm, trange
from PIL import Image
import cv2
import os
name=os.path.basename(__file__).split(".")[0]
def _int32(x):
    return int(0xFFFFFFFF & x)
class MT19937:
    def __init__(self, seed):
        self.mt = [0] * 624
        self.mt[0] = seed
        for i in range(1, 624):
            self.mt[i] = _int32(1812433253 * (self.mt[i - 1] ^ self.mt[i - 1] >> 30) + i)
    def extract_number(self):
        self.twist()
        y = self.mt[0]
        y = y ^ y >> 11
        y = y ^ y << 7 & 2636928640
        y = y ^ y << 15 & 4022730752
        y = y ^ y >> 18
        return _int32(y)
    def twist(self):
        for i in range(0, 624):
            y = _int32((self.mt[i] & 0x80000000) + (self.mt[(i + 1) % 624] & 0x7fffffff))
            self.mt[i] = y ^ self.mt[(i + 397) % 624] >> 1
            if y % 2 != 0:
                self.mt[i] = self.mt[i] ^ 0x9908b0df

imgsize=1000
record=[]
matrix=np.zeros(imgsize*imgsize*3)
rnd=MT19937(seed=666)
for i in tqdm(range(imgsize*imgsize*3)):
    # number=random.randint(0,256)
    number=rnd.extract_number()%256
    record.append(number)
    matrix[i]=number
array=[0 for i in range(0,257)]
for i in tqdm(range(len(record))):
    array[int(record[i])]+=1
matrix=matrix.reshape(imgsize,imgsize,3)
plt.plot(array)
plt.savefig(name+'freq.jpg', bbox_inches='tight')
plt.show()
cv2.imwrite(name+'vis.jpg',matrix)
s = cv2.imread(name+'.jpg')
cv2.imshow(name+'.jpg',s)
cv2.waitKey(0)
