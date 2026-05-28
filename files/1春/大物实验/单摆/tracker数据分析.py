import xml.etree.ElementTree as ET
import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import find_peaks
import re

# 设置绘图字体
plt.rcParams['font.sans-serif'] = ['SimHei', 'Microsoft YaHei']
plt.rcParams['axes.unicode_minus'] = False

def process_tracker_xml(file_path, L=0.6788, fps=30):
    """
    专门针对嵌套 FrameData 结构的 Tracker 文件解析与处理
    """
    # 1. 解决编码与路径中文字符问题
    with open(file_path, 'rb') as f:
        raw_data = f.read()
    xml_str = raw_data.decode('utf-8', errors='ignore')
    xml_str = re.sub(r'<\?xml.*?\?>', '', xml_str)

    try:
        root = ET.fromstring(xml_str)
    except ET.ParseError as e:
        print(f"XML 解析失败: {e}")
        return

    # 2. 深度解析：提取嵌套的 x, y 坐标和帧号
    frames = []
    x_coords = []
    y_coords = []

    # 寻找所有的 PointMass 对象
    for pm in root.iter('object'):
        if pm.get('class') == 'org.opensourcephysics.cabrillo.tracker.PointMass':
            # 在质点下寻找 framedata 数组
            for fd_prop in pm.iter('property'):
                if fd_prop.get('name') == 'framedata':
                    # 遍历每一帧的数据标签 [3], [4], [5] ...
                    for frame_item in fd_prop.findall('property'):
                        frame_name = frame_item.get('name') # 获取 "[3]" 这种字符串
                        match = re.search(r'\[(\d+)\]', frame_name)
                        if match:
                            frame_idx = int(match.group(1))
                            obj = frame_item.find('object')
                            if obj is not None:
                                x_val = None
                                y_val = None
                                for p in obj.findall('property'):
                                    if p.get('name') == 'x': x_val = float(p.text)
                                    if p.get('name') == 'y': y_val = float(p.text)
                                
                                if x_val is not None and y_val is not None:
                                    frames.append(frame_idx)
                                    x_coords.append(x_val)
                                    y_coords.append(y_val)

    if not frames:
        print("未能提取到有效坐标数据，请检查 Tracker 文件是否已完成打点。")
        return

    # 转换为 numpy 数组并按时间排序（防止帧号乱序）
    sort_idx = np.argsort(frames)
    frames = np.array(frames)[sort_idx]
    x = np.array(x_coords)[sort_idx]
    y = np.array(y_coords)[sort_idx]
    
    # 转换为时间轴 (t = 帧号 / FPS)
    t = frames / fps

    # 3. 自动判定主振动方向并进行中心化 (减去均值)
    # 计算方差，选择摆动幅度更大的轴
    disp = y if np.var(y) > np.var(x) else x
    disp_centered = disp - np.mean(disp)

    # 4. 取绝对值提取包络点 (实现样本点翻倍)
    disp_abs = np.abs(disp_centered)
    
    # 基于摆长 L 计算理论半周期，优化寻峰间距 (约 20 帧左右)
    # distance=15 可以在 30fps 下稳定抓取每个半周期的极值
    peaks_idx, _ = find_peaks(disp_abs, distance=15)
    t_peaks = t[peaks_idx]
    amp_peaks = disp_abs[peaks_idx]

    # 清洗：移除振幅为 0 或 NaN 的点防止 log 报错
    valid = (amp_peaks > 0) & (~np.isnan(amp_peaks))
    t_peaks, amp_peaks = t_peaks[valid], amp_peaks[valid]

    # 5. 物理计算与对数线性拟合
    # 实验周期与重力加速度
    T_exp = 2 * np.mean(np.diff(t_peaks))
    g_exp = (4 * np.pi**2 * L) / (T_exp**2)

    ln_amp = np.log(amp_peaks)
    slope, intercept = np.polyfit(t_peaks, ln_amp, 1)
    fit_line = slope * t_peaks + intercept

    # 6. 绘图
    fig, axes = plt.subplots(3, 1, figsize=(10, 12))
    
    axes[0].plot(t, disp_centered, label='中心化位移')
    axes[0].set_title("图1：摆动波形 (已强制 0 轴对称)")
    axes[0].grid(True, alpha=0.3)
    
    axes[1].plot(t, disp_abs, color='gray', alpha=0.5)
    axes[1].scatter(t_peaks, amp_peaks, color='red', label=f'捕获极值点 (n={len(t_peaks)})')
    axes[1].set_title(f"图2：绝对值翻折与极值提取 (实验周期 T={T_exp:.3f}s)")
    axes[1].legend()

    axes[2].scatter(t_peaks, ln_amp, color='red')
    axes[2].plot(t_peaks, fit_line, 'g-', label=f'拟合直线: slope={slope:.4f}')
    axes[2].set_title(f"图3：对数包络线拟合 (测得 g={g_exp:.3f} m/s²)")
    axes[2].set_xlabel("时间 (s)")
    axes[2].set_ylabel("ln(Amplitude)")
    axes[2].legend()

    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    # 使用你的绝对路径
    path = r'C:\Users\SYH\Desktop\大物实验\单摆\单摆.trk'
    process_tracker_xml(path, L=0.6788, fps=30)