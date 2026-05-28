import pandas as pd
import matplotlib.pyplot as plt

# 1. 提取并准备表格数据
data = {
    '滤波电路类型': [
        '1μF 单电容全波',
        '1μF 双电容π型',
        '10μF 单电容全波',
        '10μF 双电容π型'
    ],
    '交流电压 Vac (mV)': [550.878, 64.015, 76.880, 0.943],
    '直流电压 Vdc (mV)': [2404.69, 1376.55, 2731.41, 1498.01],
    '纹波系数 Ku (%)': [22.91, 4.65, 2.81, 0.063] 
}

df = pd.DataFrame(data)

# 2. 设置 Matplotlib 参数以支持中文显示
plt.rcParams['font.sans-serif'] = ['SimHei']  # Windows系统使用黑体
plt.rcParams['axes.unicode_minus'] = False     # 正常显示负号

# 3. 绘制图表
fig, axes = plt.subplots(1, 3, figsize=(16, 6))

# 使用四种不同的蓝色色号 (从浅到深)
blue_colors = ['#08519C', '#3182BD', '#6BAED6', '#9ECAE1']
# 缩短柱体宽度
bar_width = 0.4

# 图表 1：交流电压 Vac 对比
axes[0].bar(df['滤波电路类型'], df['交流电压 Vac (mV)'], color=blue_colors, width=bar_width)
axes[0].set_title('交流电压 $V_{AC}$ 对比 (越低越好)', fontsize=14)
axes[0].set_ylabel('电压 (mV)', fontsize=12)
axes[0].tick_params(axis='x', rotation=45)
for i, v in enumerate(df['交流电压 Vac (mV)']):
    axes[0].text(i, v + 5, str(v), ha='center', va='bottom')

# 图表 2：直流电压 Vdc 对比
axes[1].bar(df['滤波电路类型'], df['直流电压 Vdc (mV)'], color=blue_colors, width=bar_width)
axes[1].set_title('直流电压 $V_{DC}$ 对比', fontsize=14)
axes[1].set_ylabel('电压 (mV)', fontsize=12)
axes[1].tick_params(axis='x', rotation=45)
for i, v in enumerate(df['直流电压 Vdc (mV)']):
    axes[1].text(i, v + 20, str(v), ha='center', va='bottom')

# 图表 3：纹波系数 Ku 对比
axes[2].bar(df['滤波电路类型'], df['纹波系数 Ku (%)'], color=blue_colors, width=bar_width)
axes[2].set_title('纹波系数 $K_u$ 对比 (越低越好)', fontsize=14)
axes[2].set_ylabel('纹波系数 (%)', fontsize=12)
axes[2].tick_params(axis='x', rotation=45)
for i, v in enumerate(df['纹波系数 Ku (%)']):
    axes[2].text(i, v + 0.5, f"{v}%", ha='center', va='bottom')

# 4. 调整布局并保存
plt.suptitle('不同电容与滤波电路效果对比分析', fontsize=18, y=1.05)
plt.tight_layout()
plt.savefig('updated_bar_chart.png')