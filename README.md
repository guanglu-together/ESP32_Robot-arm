<div align="center">

# 🤖 ESP32 AI Vision Robot Arm

**A dual-brain robotic arm project using ESP32-S3 and PC RTX 4050 (YOLOv8) with UDP real-time control.**

**English** | 🌐 [简体中文](./README_CN.md)

</div>

---

## ✨ Features

- **Dual-Brain Architecture**: PC (RTX 4050) handles high-FPS YOLOv8 inference, while ESP32-S3 manages real-time kinematics.
- **Ultra-Fast UDP Communication**: Ditched blocking TCP/HTTP for raw UDP broadcast, achieving <10ms ultra-low latency.
- **Smooth Interpolation**: ESP32 implements non-linear smoothing to protect MG90S servos from sudden signal spikes.
- **Dual-Mode Control**: Seamless switching between LAN Web UI manual control and AI Vision auto-tracking.





## 🙏 Credits & Acknowledgments

The **base kinematics algorithm and initial Web UI framework** of this project originate from the excellent open-source creator **Tech Talkies**. Huge thanks for the solid foundation!
- 📺 **Original YouTube Channel**: [Tech Talkies](https://www.youtube.com/@techtalkies1)
- 💻 **Original GitHub Repo**: [TechTalkies/YouTube](https://github.com/TechTalkies/YouTube)



### 🛠️ Derivative Work & Enhancements
I performed deep architectural upgrades and secondary development on the original framework. The **original increments** include:
1. **AI Vision Loop**: Integrated PC-side RTX 4050 running YOLOv8 for real-time object detection.
2. **Protocol Refactoring**: Replaced HTTP blocking with a custom UDP broadcast protocol for ultra-low latency.
3. **IoT Integration**: Added mDNS LAN discovery and ArduinoOTA wireless firmware updates.
4. **State Machine**: Implemented keyframe-based action recording and playback.



## 📂 Project Directory Structure

```text
ESP32_Robot-arm/
│
├── 📁 ESP32_Firmware/             # ⚙️ Edge Firmware (Compiled and flashed using Arduino IDE)
│   ├── main.ino                   # Core control logic (Wi-Fi STA, UDP/TCP dual-stack, state machine, kinematic interpolation)
│   └── web.h                      # 🌐 LAN Web UI frontend source code (HTML/CSS/JS control sliders)
│
├── 📁 PC_Vision/                  # 👁️ Vision Scripts (Run using Python 3.11)
│   └── ai_vision_arm.py           # YOLOv8 inference, OpenCV image processing, and UDP high-speed sending script
│
├── requirements.txt               # 📦 Python application-level dependency list
├── .gitignore                     # 🛡️ Git ignore rules (Blocks venv, *.pt, compiled temp files)
└── README.md                      # 📖 Project description and architecture documentation
```



## 🛠️ Environment Setup Guide

To fully leverage the CUDA computing power of NVIDIA GPUs (such as the RTX 4050), **please strictly follow the installation order below**.

⚠️ **Critical Warnings**:

1. **DO NOT** directly run `pip install -r requirements.txt`. This will install the CPU-only version of PyTorch, resulting in the loss of AI hardware acceleration (`cuda.is_available()` will output `False` during verification).
2. **DO NOT** directly append a domestic mirror source (like the `-i` parameter) to the `torch` installation command. Because the PyTorch CUDA version is massive (>2GB), domestic mirrors are highly likely to "swap it out" and downgrade it to the CPU version, or cause the installation to crash due to network timeouts.
3. **Correct Strategy**: First, use a domestic mirror to "instantly install" underlying minor dependencies (to prevent subsequent timeouts), and then use the official source to "purely" install the PyTorch CUDA core package.

### 1. Create and Activate a Python 3.11 Virtual Environment (Recommended)

```bash
# Create virtual environment
python -m venv venv

# Activate virtual environment
# Windows (CMD/PowerShell):
venv\Scripts\activate
# Linux / macOS:
source venv/bin/activate
```

### 2. Install PyTorch with CUDA 12.1 Support (Official Source)

```bash
pip install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu121
```

### 3. Install Other Project Dependencies

```bash
pip install -r requirements.txt
```

### 4. Verify if CUDA is Successfully Activated

```bash
import torch
print(torch.cuda.is_available())  # Must output True
print(torch.cuda.get_device_name(0))  # Should output your GPU model, e.g., NVIDIA GeForce RTX 4050 Laptop GPU
```



## 🚀 Future Roadmap

This project is continuously evolving. Here are some exciting features planned for future updates:

- [ ] **✋ Gesture Control (MediaPipe)**: Move beyond color/object tracking. Integrate Google's MediaPipe to recognize hand landmarks (e.g., pinch to grip, open palm to release) for touchless robotic control.
- [ ] **📐 Inverse Kinematics (IK) & 3D Grasping**: Upgrade from 2D radar tracking to true 3D spatial awareness. Implement IK algorithms to calculate joint angles based on physical coordinates (X, Y, Z), allowing the arm to reach out and grab objects in 3D space.
- [ ] **🗣️ Voice Command & LLM Integration**: Connect to Large Language Models (LLMs) via API. Command the robot with natural language like *"Pick up the red apple"* or *"Scan the room"*, letting the AI plan the vision and motion path autonomously.
- [ ] **📦 Multi-Object Intelligent Sorting**: Utilize YOLOv8's multi-class detection to recognize various items on the desk (e.g., batteries, cans, blocks) and program the arm to sort them into different designated zones automatically.
- [ ] **🎥 AI Vlog Auto-Tracking Gimbal**: Repurpose the rig as a smart desktop camera gimbal. Use face-tracking to keep the user perfectly centered in the frame while recording videos or streaming.

*Feel free to open an Issue or submit a Pull Request if you want to contribute to any of these features!*



## ⚖️ License & Disclaimer
This project is open-sourced strictly for **educational, personal learning, and research purposes**.
It strictly adheres to the original author's copyright requirements. Commercial use is prohibited. For commercial applications, please contact the original author, Tech Talkies, for independent authorization.





