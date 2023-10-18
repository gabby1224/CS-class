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

class myrnd01:
    def __init__(self, seed=666):
        self.rnd=MT19937(seed)
    def extract_number(self):
        a=self.rnd.extract_number()%400000000
        a/=400000000
        return a
    
myrnd=myrnd01(seed=666)
def generate_possion(lbd):
    randomnumber=myrnd.extract_number()
    k=0
    p=0
    k2=1
    p+=np.exp(-lbd)*(lbd**k)/(k2)
    while(p<randomnumber):
        k+=1
        k2*=k
        p+=np.exp(-lbd)*(lbd**k)/(k2)
    return k

def generate_gaussian(mean, std_dev):
    u1 = myrnd.extract_number()
    u2 = myrnd.extract_number()
    z0 = np.sqrt(-2 * np.log(u1)) * np.cos(2 * np.pi * u2)
    z1 = np.sqrt(-2 * np.log(u1)) * np.sin(2 * np.pi * u2)

    return z0 * std_dev + mean



testcount=int(1e4)

list1=np.zeros(400)
for i in tqdm(range(testcount)):
    k=generate_possion(3)
    # k=generate_gaussian(20,6)
    if k<400:    
        list1[k]+=1
list1/=testcount
xaxis=np.linspace(0,400,400)
plt.plot(xaxis,list1)
plt.savefig(name+'generate_possion.jpg', bbox_inches='tight')
plt.show()

list2=np.zeros(4000)
for i in tqdm(range(testcount)):
    # k=generate_possion(3)
    k=generate_gaussian(40,6)
    k=int(10*k)
    if k<4000:    
        list2[k]+=1
list2/=testcount
xaxis=np.linspace(0,400,4000)
plt.plot(xaxis,list2)
plt.savefig(name+'generate_gaussian.jpg', bbox_inches='tight')
plt.show()