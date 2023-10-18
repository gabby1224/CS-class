import numpy as np
import matplotlib.pyplot as plt
import random
import os
name=os.path.basename(__file__).split(".")[0]
picnum=0
def generate_data(num_samples, num_clusters):
    np.random.seed(666)
    points = []
    labels = []
    for i in range(num_clusters):
        mean = np.random.uniform(-100, 100, 2)
        cov = np.random.uniform(-2, 2, (2, 2))
        samples = np.random.multivariate_normal(mean, cov, num_samples // num_clusters)
        label = [i for j in range(num_samples // num_clusters)]
        points.extend(samples)
        labels.extend(label)
    return np.array(points) , np.array(labels)

def visualize_data(points, labels=None):
    global picnum
    plt.scatter(points[:, 0], points[:, 1], c=labels)
    plt.title('Data')
    plt.xlabel('X')
    plt.ylabel('Y')
    picnum=picnum+1
    plt.savefig(name+'-'+str(picnum)+'.jpg', bbox_inches='tight')
    plt.show()

def k_means(points, num_clusters, num_iterations=100):
    centroids = points[np.random.choice(len(points), num_clusters, replace=False)]
    for _ in range(num_iterations):
        distances = np.linalg.norm(points[:, np.newaxis] - centroids, axis=2)
        labels = np.argmin(distances, axis=1)
        new_centroids = np.array([points[labels == k].mean(axis=0) for k in range(num_clusters)])
        if np.allclose(centroids, new_centroids):
            break
        centroids = new_centroids
    return labels

# 生成数据
num_samples = 2000
num_clusters = 100
data,labels = generate_data(num_samples, num_clusters)

# 可视化原始数据
visualize_data(data,labels)

# 使用k-means聚类数据
labels = k_means(data, num_clusters)

# 可视化聚类结果
visualize_data(data, labels)
