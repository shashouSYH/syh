import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter

# 设置中文字体，防止图表中的中文显示异常
# Windows 通常使用 'SimHei'，macOS 可以使用 'Arial Unicode MS'
plt.rcParams['font.sans-serif'] = ['SimHei', 'Arial Unicode MS']
plt.rcParams['axes.unicode_minus'] = False

# 1. 准备数据
# 频率 (Hz)
frequencies = [10, 50, 100, 200, 400, 1000, 2000]

# 表1：1uF单电容RC全波整流滤波电路 - 纹波系数 (%)
ripple_single = [64.92, 58.42, 49.87, 36.51, 22.95, 10.91, 5.84]

# 表2：1uF双电容pi型全波整流滤波电路 - 纹波系数 (%)
ripple_pi = [60.63, 39.22, 26.50, 12.57, 4.62, 0.97, 0.28]

# 2. 创建图表
plt.figure(figsize=(10, 7))

# 绘制表1数据 (蓝色实线，圆点标记)
plt.plot(frequencies, ripple_single, marker='o', linestyle='-', color='blue', 
         linewidth=2, markersize=7, label='1$\mu$F单电容RC滤波')

# 绘制表2数据 (红色实线，方块标记)
plt.plot(frequencies, ripple_pi, marker='s', linestyle='-', color='red', 
         linewidth=2, markersize=7, label='1$\mu$F双电容$\pi$型滤波')

# 3. 设置图表格式
# x轴使用对数刻度，以适应 10 到 2000 的大跨度
plt.xscale('log')

# --- 改进：让 x 轴刻度更明确 ---
plt.xticks(frequencies, frequencies) # 将实验频率点直接设为刻度标签
plt.gca().xaxis.set_major_formatter(ScalarFormatter()) # 禁用科学计数法，显示普通数字

plt.title('不同滤波电路纹波系数随信号源频率变化趋势对比', fontsize=16, fontweight='bold', pad=20)
plt.xlabel('信号源频率 (Hz) [对数坐标]', fontsize=12)
plt.ylabel('纹波系数 (%)', fontsize=12)

# 显示网格线 (主网格和次网格)
plt.grid(True, which="major", linestyle='-', alpha=0.6)
plt.grid(True, which="minor", linestyle='--', alpha=0.3)

# 显示图例
plt.legend(fontsize=12, loc='upper right')

# 4. 在数据点附近添加数值标签
# --- 改进：放大纹波系数数据字体 ---
for i, (freq, val) in enumerate(zip(frequencies, ripple_single)):
    plt.annotate(f"{val}%", (freq, val), textcoords="offset points", 
                 xytext=(0, 10), ha='center', fontsize=12, fontweight='bold', color='blue')

for i, (freq, val) in enumerate(zip(frequencies, ripple_pi)):
    # 稍微向下偏移以防文本重叠，低频段向下，高频段根据空间微调
    offset = -18 if val > 5 else -15
    plt.annotate(f"{val}%", (freq, val), textcoords="offset points", 
                 xytext=(0, offset), ha='center', fontsize=11, fontweight='bold', color='red')

# 5. 调整布局，保存并展示图表
plt.tight_layout()
plt.savefig('ripple_coefficient_trend_improved.png', dpi=300)
plt.show()