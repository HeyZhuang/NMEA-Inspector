# 🛰️ NMEA Inspector

> 🎯 **一款基于 Qt/C++ 的高性能 NMEA-0183 卫星导航数据分析与可视化工具**

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Qt](https://img.shields.io/badge/Qt-6.x-green.svg)](https://www.qt.io/)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)]()
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-orange.svg)]()

---

## 📖 项目简介 (Introduction)

**NMEA Inspector** 是一个跨平台的卫星导航数据分析工具，专为嵌入式开发者、导航算法工程师及相关专业学生设计。

本项目旨在解决 GNSS 开发过程中数据繁杂、难以直观分析的痛点。它能够实时解析标准的 **NMEA-0183** 协议数据（支持 **北斗 BDS**、GPS、GLONASS、Galileo 等主流卫星系统），并通过现代化的 UI 界面进行多维度可视化展示。

无论是串口实时调试，还是日志文件回放分析，NMEA Inspector 都能助你一臂之力。

## ✨ 核心功能 (Features)

* **🔌 多源数据接入**：
    * 支持 **串口 (Serial Port)** 实时数据读取，波特率自适应。
    * 支持本地 **NMEA 日志文件** 的导入与回放分析。
* **⚡ 高性能解析**：
    * 基于 C++ 的高效解析引擎，支持 `$GPGGA`, `$GPRMC`, `$GPGSV`, `$BDGSV` (北斗) 等常用语句。
    * 实时校验和 (Checksum) 检测，过滤错误数据。
* **📊 多维可视化 (Visualization)**：
    * **星空图 (Skyplot)**：直观展示卫星的方位角与仰角分布。
    * **信噪比图 (SNR)**：实时柱状图展示各颗卫星的信号强度。
    * **定位轨迹 (Track Map)**：基于 Canvas/地图组件绘制实时定位轨迹。
    * **仪表盘**：直观显示速度、航向、经纬度及定位状态 (Fix Status)。
* **🛠️ 开发者工具**：
    * 原始数据十六进制 (Hex) 查看。
    * 数据导出与保存。

## 📸 项目演示 (Demo)

*(此处建议放一张软件运行的主界面截图，或者一个简短的 GIF 动图)*

| 卫星星空图 | 信号强度分析 | 实时轨迹回放 |
| :---: | :---: | :---: |
| ![Skyplot Placeholder](https://via.placeholder.com/300x200?text=Skyplot+View) | ![SNR Placeholder](https://via.placeholder.com/300x200?text=SNR+Analysis) | ![Map Placeholder](https://via.placeholder.com/300x200?text=Track+Map) |

## 🚀 快速开始 (Getting Started)

### 开发环境要求
* **C++ Standard**: C++ 17 及以上
* **Qt Version**: Qt 6.2+ (推荐) 或 Qt 5.15
* **Compiler**: MSVC 2019+ / GCC / Clang
* **Build System**: CMake 或 QMake

### 安装与构建 (Build)

1.  **克隆仓库**
    ```bash
    git clone git@github.com:HeyZhuang/NMEA-Inspector.git
    cd NMEA-Inspector
    ```

2.  **使用 Qt Creator 构建 (推荐)**
    * 打开 `CMakeLists.txt` 或 `.pro` 文件。
    * 配置项目构建套件 (Kit)。
    * 点击 **运行 (Run)** 即可。

3.  **命令行构建 (CMake)**
    ```bash
    mkdir build && cd build
    cmake ..
    make
    ./NMEA-Inspector
    ```

## 📂 项目结构 (Structure)

```text
NMEA-Inspector/
├── src/                # 源代码
│   ├── core/           # NMEA 解析核心逻辑
│   ├── gui/            # Qt 界面文件 (.ui, .cpp, .h)
│   ├── utils/          # 工具类 (串口管理, 文件操作)
│   └── main.cpp        # 程序入口
├── resources/          # 图标与资源文件
├── tests/              # 单元测试
├── docs/               # 文档说明
└── README.md           # 你正在阅读的文档
