import numpy as np
import matplotlib.pyplot as plt

def My_gaussRamd(mean,var,size):
    std = np.sqrt(var)
    ramdom = []
    random = np.random.normal(mean, std, size)
    return random

def main():
    mean = 10
    var = 5
    estiMean = []
    estiVar = []
    sizearr = []
    for size in range(100,10000,100):
        random = My_gaussRamd(mean,var,size)
        e_mean,e_var = Estimate(random)
        estiMean.append(e_mean)
        estiVar.append(e_var)
        sizearr.append(size)
    plt.plot(sizearr,estiMean,marker = 'o')
    plt.xlabel("size")
    plt.ylabel("estimate_Mean")
    plt.savefig('./mean.png')
    plt.show()


    plt.plot(sizearr, estiVar, marker='o')
    plt.xlabel("size")
    plt.ylabel("estimate_Var")
    plt.savefig('./var.png')
    plt.show()


def Estimate(random):
    e_mean = np.mean(random)
    e_variance = np.var(random)
    return e_mean,e_variance


if __name__ == "__main__":
    main()