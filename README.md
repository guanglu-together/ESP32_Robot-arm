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

## ⚖️ License & Disclaimer
This project is open-sourced strictly for **educational, personal learning, and research purposes**.
It strictly adheres to the original author's copyright requirements. Commercial use is prohibited. For commercial applications, please contact the original author, Tech Talkies, for independent authorization.
