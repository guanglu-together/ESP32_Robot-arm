<div align="center">
🤖 ESP32 4-DOF Robot Arm (Stable Base Version)
A foundational control project for a 4-DOF robotic arm based on the ESP32-S3 DevKitC-1, featuring stable LAN Web UI control and kinematic interpolation.
English | 🌐 [简体中文](./README_CN.md)
</div>

## ✨ Features
* **Successful Hardware Porting**: Perfectly ported the original code from XIAO ESP32S3 to the **ESP32-S3 DevKitC-1** development board.
* **Stable 4-Servo Drive**: Successfully adapted and stably drives 4 x MG90S servos, achieving smooth motion for the 4-DOF robotic arm.
* **Smooth Interpolation**: ESP32 implements non-linear smoothing to protect MG90S servos from sudden signal spikes, extending hardware lifespan.
* **LAN Web UI Control**: Built-in lightweight web server allows real-time control of the arm's posture via visual sliders on a phone or PC browser over LAN.

## 🙏 Credits & Acknowledgments
The base kinematics algorithm and initial Web UI framework of this project originate from the excellent open-source creator Tech Talkies. Huge thanks for the solid foundation!
* 📺 Original YouTube Channel: [Tech Talkies](https://www.youtube.com/@techtalkies1)
* 💻 Original GitHub Repo: [TechTalkies/YouTube](https://github.com/TechTalkies/YouTube)

## 🛠️ About the Stable Branch (Derivative Work)
This `stable` branch is a **clean, stable, and foundational hardware driver version**.
Building upon the original author's excellent framework, my primary work includes:
1. **MCU Porting**: Resolved pin definitions and hardware differences to successfully port the code from XIAO ESP32S3 to the more generic **ESP32-S3 DevKitC-1**.
2. **Focus on Core Control**: Removed experimental features like PC-side AI Vision (YOLOv8) and UDP communication to focus on polishing the stability of Web UI manual control and underlying servo driving.

*(Note: For the advanced version featuring AI Vision, Dual-Brain architecture, and UDP ultra-low latency, please check out the `main` branch.)*

## 📂 Project Directory Structure
```text
ESP32_Robot-arm/
│
├── 📁 ESP32_Firmware/             # ⚙️ Edge Firmware (Compiled and flashed using Arduino IDE)
│   ├── main.ino                   # Core control logic (Wi-Fi STA, Web Server, kinematic interpolation, 4-channel PWM)
│   └── web.h                      # 🌐 LAN Web UI frontend source code (HTML/CSS/JS control sliders)
│
├── .gitignore                     # 🛡️ Git ignore rules
├── README.md                      # 📖 English Project Description
└── README_CN.md                   # 📖 Chinese Project Description
```
## 🛠️ Environment Setup & Flashing Guide

This branch only contains ESP32 firmware code; no Python environment setup is required. Please use the **Arduino IDE** for compilation and flashing.

1. **Install ESP32 Board Support**:
    - In Arduino IDE, go to `File` -> `Preferences` -> `Additional boards manager URLs`, and add the Espressif official link: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
    - Search for and install `esp32` in the `Boards Manager`.
2. **Select the Correct Board**:
    - Tools -> Board -> Select `ESP32S3 Dev Module` (or the corresponding DevKitC-1 option).
3. **Configure Pins (Important)**:
    - Open `ESP32_Firmware/main.ino` and ensure the servo pin definitions match the actual GPIO pins connected to your ESP32-S3 DevKitC-1.
4. **Compile and Upload**:
    - Connect the board, select the correct COM port, and click Upload.
5. **Connect and Use**:
    - After flashing, the ESP32 will connect to your configured Wi-Fi.
    - Open the Serial Monitor (Baud rate 115200) to get the ESP32's LAN IP address.
    - Enter this IP address in a browser on your phone or PC to open the Web UI control interface.

## 🚀 Advanced Features (Head to the Main Branch)

If you want more than just basic manual control, the `main` branch of this project is actively developing the following "hardcore" advanced features:

- 👁️ **AI Vision Loop**: Integrating PC-side RTX 4050 running YOLOv8 for real-time object auto-tracking.
- ⚡ **Ultra-Fast UDP**: Ditched HTTP blocking for a custom UDP broadcast protocol achieving <10ms latency.
- ✋ **Gesture Control**, **Inverse Kinematics (IK) 3D Grasping**, **LLM Voice Integration**, and more.

## ⚖️ License & Disclaimer

This project is open-sourced strictly for educational, personal learning, and research purposes. It strictly adheres to the original author's copyright requirements. Commercial use is prohibited. For commercial applications, please contact the original author, Tech Talkies, for independent authorization.