import numpy as np
import matplotlib.pyplot as plt
from scipy import stats

# 1. 输入实验数据
# T 数据 (单位: s^2)
T = np.array([47.10, 44.93, 47.44, 49.96, 51.22, 53.02])
T1 = T / 30.0
T2 = T1 ** (2)
# 摆长 l 数据 (单位: cm)
l_cm = np.array([60.68, 54.71, 61.85, 68.38, 71.56, 76.72])
# 将摆长转换为米 (单位: m) 用于计算重力加速度
l_m = l_cm / 100.0

# 2. 最小二乘法线性拟合
# 计算斜率、截距、相关系数、p值和斜率标准差 (使用国际标准单位 m)
slope, intercept, r_value, p_value, std_err = stats.linregress(T2, l_m)

# 3. 计算重力加速度 g 及其不确定度
g = 4 * np.pi**2 * slope
u_g = 4 * np.pi**2 * std_err

# 4. 打印计算结果
print("========== 数据处理结果 ==========")
print(f"拟合直线方程: l = {slope:.4f} * T^2 + {intercept:.4f}")
print(f"斜率 k: {slope:.4f} m/s^2")
print(f"斜率的标准不确定度 u_k: {std_err:.4f} m/s^2")
print(f"重力加速度测量值 g: {g:.3f} m/s^2")
print(f"重力加速度不确定度 u_g: {u_g:.3f} m/s^2")
print(f"决定系数 R^2: {r_value**2:.5f}")
print("==================================")

# 5. 绘制 l - T^2 关系图 (为了与实验报告图表一致，Y轴使用 cm)
# 重新计算以 cm 为单位的拟合参数用于绘图
slope_cm, intercept_cm, _, _, std_err_cm = stats.linregress(T2, l_cm)

plt.figure(figsize=(8, 6))

# 绘制实验数据点 (黑色方块)
plt.plot(T2, l_cm, 'ks', markersize=6, label='Experimental Data')

# 绘制线性拟合直线 (红色实线)
fit_line = intercept_cm + slope_cm * T2
plt.plot(T2, fit_line, 'r-', linewidth=1, label='Linear Fit')

# 设置图表标签和刻度字体大小
plt.xlabel('T$^2$ (s$^2$)', fontsize=14)
plt.ylabel('l (cm)', fontsize=14)
plt.xticks(fontsize=12)
plt.yticks(fontsize=12)

# 在图表中添加类似 Origin 风格的数据参数文本框
text_str = '\n'.join((
    'Equation         y = a + b*x',
    f'Intercept        {intercept_cm:.4f}',
    f'Slope            {slope_cm:.4f} ± {std_err_cm:.4f}',
    f'R-Square (COD)   {r_value**2:.5f}'
))

# 放置文本框
props = dict(boxstyle='square,pad=0.5', facecolor='white', edgecolor='black', alpha=1)
plt.text(0.05, 0.95, text_str, transform=plt.gca().transAxes, fontsize=11,
         verticalalignment='top', bbox=props, fontfamily='monospace')

# 调整图表布局并显示
plt.tight_layout()
plt.show()