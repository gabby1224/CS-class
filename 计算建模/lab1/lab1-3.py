import numpy as np
from sklearn.mixture import GaussianMixture
import matplotlib.pyplot as plt

def main(file_path):
    array = myloadtxt(file_path)

    # 使用库里有的GMM进行分类
    k = 3  # 类别数
    gmm = GaussianMixture(n_components=k, random_state=0)
    gmm.fit(array)
    labels = gmm.predict(array)

    plt.scatter(array[:, 0], array[:, 1], c=labels, cmap='viridis')
    plt.xlabel('Density')
    plt.ylabel('Sugar')
    plt.title('Class')
    plt.savefig("./fruit_classify.png")


def myloadtxt(file_path):
    array = np.loadtxt(file_path, delimiter=',')
    print(array)
    return array


if __name__ == "__main__":
    main("data.txt")