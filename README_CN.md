<div align="center">
🤖 ESP32 4-DOF 机械臂 (Stable 基础版)
一个基于 ESP32-S3 DevKitC-1 的四自由度机械臂基础控制项目，提供稳定的局域网 Web UI 控制与运动学插值。
🌐 [English](./README.md) | 简体中文
</div>

## ✨ 核心特性
* **硬件成功移植**：将原作者基于 XIAO ESP32S3 的代码完美移植至 **ESP32-S3 DevKitC-1** 开发板。
* **四舵机稳定驱动**：成功适配并稳定驱动 4 个 MG90S 舵机，实现四自由度 (4-DOF) 机械臂的平滑运动。
* **平滑插值算法**：ESP32 内部实现非线性平滑过渡，有效保护 MG90S 舵机免受突变信号冲击，延长硬件寿命。
* **局域网 Web UI 控制**：内置轻量级 Web 服务器，支持通过手机或电脑浏览器连接局域网，使用可视化滑块实时控制机械臂姿态。

## 🙏 致谢与版权声明
本项目的基础机械臂运动学算法与初始 Web UI 框架来源于极其优秀的开源创作者 Tech Talkies。感谢原作者提供的坚实基础！
* 📺 原作者 YouTube 频道: [Tech Talkies](https://www.youtube.com/@techtalkies1)
* 💻 原始 GitHub 仓库: [TechTalkies/YouTube](https://github.com/TechTalkies/YouTube)

## 🛠️ 关于 Stable 分支 (二次开发声明)
本 `stable` 分支是一个**纯净、稳定的基础硬件驱动版本**。
在原作者优秀的基础框架上，我主要进行了以下工作：
1. **主控芯片移植**：解决了引脚定义与硬件差异，将代码从 XIAO ESP32S3 成功移植到更通用的 **ESP32-S3 DevKitC-1**。
2. **聚焦核心控制**：移除了 PC 端 AI 视觉 (YOLOv8) 与 UDP 通信等实验性功能，专注于打磨 Web UI 手动控制与底层舵机驱动的稳定性。

*(注：包含 AI 视觉、双脑架构及 UDP 极速通信的进阶版本，请前往 `main` 分支查看。)*

## 📂 项目目录结构
```text
ESP32_Robot-arm/
│
├── 📁 ESP32_Firmware/             # ⚙️ 边缘端固件 (使用 Arduino IDE 编译烧录)
│   ├── main.ino                   # 核心主控逻辑 (Wi-Fi STA, Web Server, 运动学插值, 4路 PWM 控制)
│   └── web.h                      # 🌐 局域网 Web UI 前端源码 (HTML/CSS/JS 控制滑块)
│
├── .gitignore                     # 🛡️ Git 忽略规则
├── README.md                      # 📖 英文项目说明
└── README_CN.md                   # 📖 中文项目说明
```

## 🛠️ 环境配置与烧录指南

本分支仅包含 ESP32 固件代码，无需配置 Python 环境。请使用 **Arduino IDE** 进行编译和烧录。

1. **安装 ESP32 开发板支持**：
    - 在 Arduino IDE 中，进入 `文件` -> `首选项` -> `附加开发板管理器网址`，添加 Espressif 官方链接： `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
    - 在 `开发板管理器` 中搜索并安装 `esp32`。
2. **选择正确的开发板**：
    - 工具 -> 开发板 -> 选择 `ESP32S3 Dev Module` (或对应的 DevKitC-1 选项)。
3. **配置引脚 (重要)**：
    - 打开 `ESP32_Firmware/main.ino`，确保代码中的舵机引脚定义 (Servo Pins) 与你实际连接到 ESP32-S3 DevKitC-1 的 GPIO 引脚一致。
4. **编译与上传**：
    - 连接开发板，选择正确的 COM 端口，点击上传。
5. **连接与使用**：
    - 烧录完成后，ESP32 将连接到你配置的 Wi-Fi。
    - 打开串口监视器 (波特率 115200) 获取 ESP32 的局域网 IP 地址。
    - 在手机或电脑浏览器中输入该 IP 地址，即可打开 Web UI 控制界面。

## 🚀 进阶玩法 (前往 Main 分支)

如果你不满足于基础的手动控制，本项目的 `main` 分支正在开发以下“硬核”进阶功能：

- 👁️ **AI 视觉闭环**：引入 PC 端 RTX 4050 运行 YOLOv8 实现目标自动追踪。
- ⚡ **UDP 极速通信**：抛弃 HTTP 阻塞，手写 UDP 广播协议实现 <10ms 极低延迟控制。
- ✋ **手势隔空控制**、**逆运动学 (IK) 3D 抓取**、**大模型 (LLM) 语音接入**等。

## ⚖️ 许可与免责声明

本项目开源仅供个人学习、技术研究与教育目的。 严格遵守原作者的版权要求。任何人不得将本项目直接用于商业产品售卖。如需商业应用，请务必联系原作者 Tech Talkies 获取独立授权。
