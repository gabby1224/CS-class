import cv2
import numpy as np
from skimage.metrics import structural_similarity as ssim
from skimage.metrics import peak_signal_noise_ratio as psnr

def add_gaussian_noise(image, mean, sigma):
    noise = np.random.normal(mean, sigma, image.shape).astype('uint8')
    noisy_image = cv2.add(image, noise)
    return noisy_image

def estimate_noise_parameters(noisy_image):
    mean = np.mean(noisy_image)
    variance = np.var(noisy_image)
    return mean, np.sqrt(variance)

def main():
    # 读取图像
    original_image = cv2.imread('original_image.bmp', cv2.IMREAD_COLOR)

    # 添加高斯噪声
    mean = 0  # 均值
    sigma = 25  # 标准差
    noisy_image = add_gaussian_noise(original_image, mean, sigma)

    # 计算PSNR和SSIM
    psnr_value = psnr(original_image, noisy_image)
    ssim_value = ssim(original_image, noisy_image, multichannel=True)

    print(f'PSNR: {psnr_value:.2f}')
    print(f'SSIM: {ssim_value:.4f}')

    # 估计噪声参数
    estimated_mean, estimated_sigma = estimate_noise_parameters(noisy_image)

    print(f'Estimated Mean: {estimated_mean:.2f}')
    print(f'Estimated Standard Deviation: {estimated_sigma:.2f}')

if __name__ == "__main__":
    main()