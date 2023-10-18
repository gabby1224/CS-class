import numpy as np

# 定义转移矩阵 P 和初始状态分布 p0
a = 1/2
b = 1/3
P = np.array([[a, 1-a], [1-b, b]])
p0 = np.array([1/2, 1/2])

# 模拟马尔可夫链
num_steps = 1000
current_state = np.random.choice([0, 1], p=p0)  # 初始状态
state_counts = [0, 0]

for _ in range(num_steps):
    state_counts[current_state] += 1
    current_state = np.random.choice([0, 1], p=P[current_state])

simulated_distribution = np.array(state_counts) / num_steps
theoretical_distribution = np.array([(1-b)/(2-a-b), (1-a)/(2-a-b)])

print(f"Simulated Distribution: {simulated_distribution}")
print(f"Theoretical Distribution: {theoretical_distribution}")
