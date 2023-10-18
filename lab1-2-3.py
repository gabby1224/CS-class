import numpy as np
import matplotlib.pyplot as plt

def main():
    # 设置参数
    aver_arrival = 4  # 平均到达率，即每分钟到达的坦克数
    total_time = 3  # 总时间间隔为3分钟
    total_tanks = []

    # 模拟在第1、2、3分钟内各到达几辆坦克
    for i in range(0, 100):
        tanks_first_minute = np.random.poisson(aver_arrival)
        tanks_second_minute = np.random.poisson(aver_arrival)
        tanks_third_minute = np.random.poisson(aver_arrival)
        t_tanks = tanks_first_minute + tanks_second_minute + tanks_third_minute
        print(f'Total tanks in 3 minutes: {t_tanks}')
        print(f'Tanks arrived in 1st minute: {tanks_first_minute}')
        print(f'Tanks arrived in 2nd minute: {tanks_second_minute}')
        print(f'Tanks arrived in 3rd minute: {tanks_third_minute}')

        total_tanks.append(t_tanks)

        # 下面是第二问
        arrival_rate = 1 / aver_arrival  # 到达率，即每辆坦克的到达间隔时间

        # 生成每辆坦克的到达时刻
        arrival_times = np.cumsum(np.random.exponential(scale=arrival_rate, size=t_tanks))

        print(f'Arrival times for each tank (in minutes): {arrival_times}')

    # 统计数字出现的次数
    counts = {}
    for num in total_tanks:
        if num in counts:
            counts[num] += 1
        else:
            counts[num] = 1

    # 提取数字和对应的出现次数
    numbers = list(counts.keys())
    occurrences = list(counts.values())

    # 绘制条形图
    plt.bar(numbers, occurrences, color='blue')

    # 添加标题和标签
    plt.title('Occurrences of Numbers')
    plt.xlabel('Numbers')
    plt.ylabel('Occurrences')
    plt.savefig('./lab2-2.png')




if __name__ == "__main__":
    main()