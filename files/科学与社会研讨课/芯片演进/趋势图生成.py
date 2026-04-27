import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import re
import textwrap
from adjustText import adjust_text

# 设置中文字体，确保图片中中文正常显示
plt.rcParams['font.sans-serif'] = ['Microsoft YaHei', 'SimHei', 'Arial Unicode MS']
plt.rcParams['axes.unicode_minus'] = False

# ==================== Intel 数据 ====================
intel_data = [
    [1971, "4004", "-", "10 µm"],
    [1972, "8008", "-", "10 µm"],
    [1974, "8080", "-", "6 µm"],
    [1976, "8085", "-", "3 µm"],
    [1978, "8086", "-", "3 µm"],
    [1982, "80286", "-", "1.5 µm"],
    [1985, "Intel 386", "-", "1 µm"],
    [1989, "i860", "-", "1 µm"],
    [1992, "OverDrive", "-", "-"],
    [1993, "Pentium (奔腾)", "P5", "800 nm"],
    [1995, "Pentium Pro", "P6", "500 nm"],
    [1998, "Pentium II Xeon / Celeron", "P6 / Covington", "250 nm"],
    [1998, "StrongARM 技术处理器", "-", "-"],
    [1999, "Pentium III & P3 Xeon", "Katmai", "250 nm"],
    [2000, "Pentium 4", "NetBurst", "180 nm"],
    [2001, "Itanium (安腾)", "IA-64", "180 nm"],
    [2003, "Centrino (迅驰)", "Banias", "130 nm"],
    [2006, "Core 2 Duo / Quad", "Core", "65 nm"],
    [2007, "Core 2 Quad", "Penryn", "45 nm"],
    [2008, "Intel Atom (凌动)", "Bonnell", "45 nm"],
    [2010, "第 1 代 Core", "Nehalem", "45 nm"],
    [2011, "第 2 代 Core", "Sandy Bridge", "32 nm"],
    [2012, "第 3 代 Core", "Ivy Bridge", "22 nm"],
    [2013, "第 4 代 Core", "Haswell", "22 nm"],
    [2015, "第 5 / 6 代 Core", "Broadwell / Skylake", "14 nm"],
    [2016, "第 7 代 Core", "Kaby Lake", "14 nm+"],
    [2017, "第 8 代 Core", "Kaby Lake R / Coffee", "14 nm++"],
    [2018, "第 9 代 Core", "Coffee Lake Refresh", "14 nm++"],
    [2019, "第 10 代 Core", "Ice Lake (移动端)", "10 nm"],
    [2020, "第 11 代 Core", "Tiger Lake", "10 nm SuperFin"],
    [2021, "第 12 代 Core", "Alder Lake", "Intel 7 (10nm)"],
    [2022, "第 13 代 Core", "Raptor Lake", "Intel 7"],
    [2023, "第 14 代 Core", "Raptor Lake Refresh", "Intel 7"],
    [2023, "第 1 代 Core Ultra", "Meteor Lake", "Intel 4 + TSMC"],
    [2024, "第 2 代 Core Ultra", "Lunar Lake / Arrow", "TSMC N3B 主导"],
    [2026, "第 3 代 Core Ultra", "Panther Lake", "Intel 18A"],
]

def process_to_nm(value):
    if value == "-" or pd.isna(value):
        return None
    value = str(value).strip()
    if "µm" in value:
        num = float(value.replace("µm", "").strip())
        return num * 1000
    elif "nm" in value:
        match = re.search(r'(\d+(?:\.\d+)?)', value)
        if match:
            return float(match.group(1))
    elif "Intel" in value:
        if "Intel 7" in value:
            return 10
        elif "Intel 4" in value:
            return 7
        elif "Intel 18A" in value:
            return 2
    elif "TSMC N3B" in value:
        return 3
    return None

# ==================== AMD 数据 ====================
amd_data = [
    [1991, "Am386", "-", "1.5 µm"],
    [1993, "Am486", "-", "500 nm"],
    [1996, "AMD K5", "K5", "500 nm"],
    [1997, "AMD K6", "K6", "350 nm"],
    [1999, "Athlon (速龙)", "K7", "250 nm"],
    [2003, "Athlon 64 / Opteron", "K8", "130 nm"],
    [2007, "Phenom (羿龙)", "K10", "65 nm"],
    [2011, "FX 系列", "Bulldozer (推土机)", "32 nm"],
    [2014, "A 系列 APU", "Steamroller (压路机) / Excavator", "28 nm"],
    [2017, "Ryzen (锐龙) 1000", "Zen 1", "14 nm"],
    [2019, "Ryzen 3000", "Zen 2", "7 nm (TSMC)"],
    [2020, "Ryzen 5000", "Zen 3", "7 nm"],
    [2022, "Ryzen 7000", "Zen 4", "5 nm (TSMC)"],
    [2023, "Ryzen 7000X3D", "Zen 4 (3D V-Cache)", "5 nm + 6 nm"],
    [2024, "Ryzen 9000 (桌面端) / Ryzen AI 300 (移动端)", "Zen 5", "4 nm / 3 nm (TSMC)"],
    [2026, "Next Gen (预计为 Ryzen 新一代)", "Zen 6 (预计)", "3 nm / 2 nm 级节点"],
]

def amd_process_to_nm(value):
    if value == "-" or pd.isna(value):
        return None
    value = str(value).strip()
    if "µm" in value:
        num = float(value.replace("µm", "").strip())
        return num * 1000
    elif "nm" in value:
        if "+" in value:
            match = re.search(r'(\d+(?:\.\d+)?)\s*nm', value)
            if match:
                return float(match.group(1))
        elif "/" in value:
            matches = re.findall(r'(\d+(?:\.\d+)?)\s*nm', value)
            if matches:
                return min([float(m) for m in matches])
        else:
            match = re.search(r'(\d+(?:\.\d+)?)', value)
            if match:
                return float(match.group(1))
    elif "级节点" in value:
        match = re.search(r'(\d+(?:\.\d+)?)\s*nm', value)
        if match:
            return float(match.group(1))
    return None

# ==================== 绘图函数 ====================
def create_process_trend_chart(df, title, output_path):
    years = df["年份"].values
    process_nm = df["工艺数值(nm)"].values
    
    start_year = int(years.min())
    start_nm = process_nm[0]
    
    fig, ax = plt.subplots(figsize=(20, 12)) # 进一步放大画布以容纳更多多行文本
    
    # 绘制主线条
    ax.semilogy(years, process_nm, 'o-', color='#0066CC', linewidth=2.5, 
                markersize=6, markerfacecolor='#0066CC', markeredgecolor='white', markeredgewidth=1)
    
    # 数据聚合：处理同一年份且工艺相同的节点，防止标签完全重合
    grouped = df.groupby(["年份", "工艺数值(nm)"]).agg({
        "芯片名称": lambda x: " / ".join([str(i) for i in pd.unique(x) if str(i) != "-" and pd.notna(i)]),
        "架构代号": lambda x: " / ".join([str(i) for i in pd.unique(x) if str(i) != "-" and pd.notna(i)]),
        "制造工艺": "first"
    }).reset_index()
    
    grouped = grouped.sort_values(by="年份")
    
    texts = []
    # 遍历聚合后的数据打标签
    for idx, row in grouped.iterrows():
        year = row["年份"]
        nm = row["工艺数值(nm)"]
        chip_name = row["芯片名称"]
        arch = row["架构代号"]
        process_str = row["制造工艺"]
        
        # 组装显示文本 (堆叠多维度)
        parts = []
        if chip_name.strip() != "":
            parts.append(textwrap.fill(chip_name, width=22))
        if arch.strip() != "":
            parts.append(textwrap.fill(arch, width=22))
        if process_str.strip() != "":
            parts.append(process_str)
        parts.append(str(int(year)))
        
        label_text = "\n".join(parts)
        
        # 记录需要生成的文本点
        t = ax.text(year, nm, label_text, fontsize=8, ha='center', va='center',
                    color='#222222',
                    bbox=dict(boxstyle='round,pad=0.3', facecolor='white', edgecolor='#0066CC', alpha=0.9, lw=0.6))
        texts.append(t)
        
    # 自动排版，防止互相重叠
    adjust_text(texts, ax=ax,
                arrowprops=dict(arrowstyle='-', color='#0066CC', alpha=0.6, lw=1.0),
                force_text=(1.2, 1.5),
                force_points=(1.2, 1.5),
                expand_text=(1.1, 1.25),
                expand_points=(1.25, 1.5),
                max_iterations=1500)
    
    # 设置图表基础属性
    ax.set_title(title, fontsize=18, fontweight='bold', pad=20)
    ax.set_xlabel('年份', fontsize=14, fontweight='bold')
    ax.set_ylabel('制造工艺 (nm, 对数刻度)', fontsize=14, fontweight='bold')
    
    ax.grid(True, which='both', linestyle='--', alpha=0.5)
    ax.set_axisbelow(True)
    
    max_nm = process_nm.max()
    if max_nm > 500:
        ax.set_yticks([10000, 5000, 2000, 1000, 500, 200, 100, 50, 20, 10, 5, 2, 1])
        ax.set_yticklabels(['10000', '5000', '2000', '1000', '500', '200', '100', '50', '20', '10', '5', '2', '1'])
        ax.set_ylim(1, 15000)
    else:
        ax.set_yticks([100, 50, 20, 10, 5, 2, 1])
        ax.set_yticklabels(['100', '50', '20', '10', '5', '2', '1'])
        ax.set_ylim(1, 150)
    ax.set_xlim(years.min() - 2, years.max() + 2)
    
    # 摩尔定律参考线
    moore_years = np.arange(start_year, int(years.max()) + 1, 2)
    moore_nm = start_nm * (0.7 ** ((moore_years - start_year) / 2))
    ax.plot(moore_years, moore_nm, '--', color='#999999', linewidth=1.5, alpha=0.7, label='摩尔定律参考线')
    
    ax.legend(loc='upper right', fontsize=11)
    
    plt.tight_layout()
    plt.savefig(output_path, dpi=200, bbox_inches='tight', facecolor='white', edgecolor='none')
    plt.close()
    
    print(f"图表已保存至: {output_path}")

# ==================== 生成并输出 ====================
df_intel = pd.DataFrame(intel_data, columns=["年份", "芯片名称", "架构代号", "制造工艺"])
df_intel["工艺数值(nm)"] = df_intel["制造工艺"].apply(process_to_nm)
df_intel_chart = df_intel[df_intel["工艺数值(nm)"].notna()].copy()

create_process_trend_chart(
    df_intel_chart, 
    'Intel 芯片架构与制造工艺演进全景 (1971-2026)', 
    'intel_process_trend_full.png'
)

df_amd = pd.DataFrame(amd_data, columns=["年份", "芯片名称", "架构代号", "制造工艺"])
df_amd["工艺数值(nm)"] = df_amd["制造工艺"].apply(amd_process_to_nm)
df_amd_chart = df_amd[df_amd["工艺数值(nm)"].notna()].copy()

create_process_trend_chart(
    df_amd_chart, 
    'AMD 芯片架构与制造工艺演进全景 (1991-2026)', 
    'amd_process_trend_full.png'
)

# ==================== Apple 数据 ====================
apple_data = [
    [2010, "Apple A4", "Cortex-A8 (Hummingbird)", "45nm"],
    [2011, "Apple A5", "Cortex-A9", "45/32nm"],
    [2012, "Apple A6 / A6X", "Swift (首款全面定制)", "32nm"],
    [2013, "Apple A7", "Cyclone (首款64位)", "28nm"],
    [2014, "Apple A8 / A8X", "Typhoon", "20nm"],
    [2015, "Apple A9 / A9X", "Twister", "14nm / 16nm"],
    [2016, "Apple A10 Fusion", "Hurricane & Zephyr", "16nm"],
    [2017, "Apple A11 Bionic", "Monsoon & Mistral", "10nm"],
    [2018, "Apple A12 Bionic / A12X", "Vortex & Tempest", "7nm"],
    [2019, "Apple A13 Bionic", "Lightning & Thunder", "7nm"],
    [2020, "Apple A14 Bionic / M1", "Firestorm & Icestorm", "5nm"],
    [2021, "Apple A15 Bionic / M1 Pro / M1 Max", "Avalanche & Blizzard", "5nm"],
    [2022, "Apple A16 Bionic / M2", "Everest & Sawtooth (A16) / Avalanche & Blizzard (M2)", "4nm (A16) / 5nm (M2)"],
    [2023, "Apple A17 Pro / M3", "Everest 2nd Gen & Sawtooth 2nd Gen", "3nm"],
    [2024, "Apple A18 (Pro) / M4 系列", "Everest 3rd Gen & Sawtooth 3rd Gen (引入SME指令集)", "3nm"],
    [2025, "Apple A19 (Pro) / M5 系列", "架构升级 (引入每核神经加速器 Neural Accelerator)", "3nm"]
]

# ==================== Qualcomm 数据 ====================
qualcomm_data = [
    [2007, "Snapdragon S1", "Scorpion (定制)", "65nm"],
    [2010, "Snapdragon S1 (升级版)", "Scorpion (定制)", "45/65nm"],
    [2012, "Snapdragon S4", "Krait", "28nm"],
    [2013, "Snapdragon 800", "Krait 400", "28nm"],
    [2014, "Snapdragon 801 / 805", "Krait 400 / 450", "28nm"],
    [2015, "Snapdragon 810", "Cortex-A57 & A53 (ARM公版)", "20nm"],
    [2016, "Snapdragon 820 / 821", "Kryo", "14nm"],
    [2017, "Snapdragon 835", "Kryo 280", "10nm"],
    [2018, "Snapdragon 845", "Kryo 385", "10nm"],
    [2019, "Snapdragon 855 / 855+", "Kryo 485", "7nm"],
    [2020, "Snapdragon 865 / 888", "Kryo 585 / Kryo 680", "7nm / 5nm"],
    [2021, "Snapdragon 8 Gen 1", "Kryo (基于 Cortex-X2/A710/A510)", "4nm"],
    [2022, "Snapdragon 8+ Gen 1 / 8 Gen 2", "Kryo (基于 Cortex-X3/A715/A510)", "4nm"],
    [2023, "Snapdragon 8 Gen 3 / X Elite", "Kryo (基于 Cortex-X4) / Oryon V1 (X Elite定制架构)", "4nm"],
    [2024, "Snapdragon 8 Elite / X Plus", "Oryon V2 / Oryon", "3nm / 4nm"],
    [2025, "Snapdragon 8 Elite Gen 5 / X2 Elite", "Oryon (面向移动端与PC端的全面定制化)", "3nm"]
]

def mobile_process_to_nm(value):
    if value == "-" or pd.isna(value):
        return None
    value = str(value).strip()
    # 过滤掉括号内的内容（如 (A16), (M2) 等），避免这些数字被正则提取为纳米数
    value_clean = re.sub(r'\(.*?\)', '', value)
    if "µm" in value_clean:
        num = float(value_clean.replace("µm", "").strip())
        return num * 1000
    elif "nm" in value_clean:
        matches = re.findall(r'(\d+(?:\.\d+)?)', value_clean)
        if matches:
            return min([float(m) for m in matches])
    return None

df_apple = pd.DataFrame(apple_data, columns=["年份", "芯片名称", "架构代号", "制造工艺"])
df_apple["工艺数值(nm)"] = df_apple["制造工艺"].apply(mobile_process_to_nm)
df_apple_chart = df_apple[df_apple["工艺数值(nm)"].notna()].copy()

create_process_trend_chart(
    df_apple_chart, 
    'Apple 芯片架构与制造工艺演进全景 (2010-2026)', 
    'apple_process_trend_full.png'
)

df_qualcomm = pd.DataFrame(qualcomm_data, columns=["年份", "芯片名称", "架构代号", "制造工艺"])
df_qualcomm["工艺数值(nm)"] = df_qualcomm["制造工艺"].apply(mobile_process_to_nm)
df_qualcomm_chart = df_qualcomm[df_qualcomm["工艺数值(nm)"].notna()].copy()

create_process_trend_chart(
    df_qualcomm_chart, 
    'Qualcomm 芯片架构与制造工艺演进全景 (2007-2026)', 
    'qualcomm_process_trend_full.png'
)