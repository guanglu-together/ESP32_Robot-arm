# ESP32 AI Vision Robot Arm
A dual-brain robotic arm project using ESP32-S3 and RTX 4050 (YOLOv8) with UDP real-time control.

## 🙏 致谢与版权声明 (Credits & Acknowledgments)

本项目的**基础机械臂运动学算法与初始 Web UI 框架**来源于极其优秀的开源创作者 **Tech Talkies**。感谢原作者提供的坚实基础！
- 📺 **原作者 YouTube 频道**: [Tech Talkies](https://www.youtube.com/@techtalkies1)
- 💻 **原始 GitHub 仓库**: [TechTalkies/YouTube](https://github.com/TechTalkies/YouTube)

### 🛠️ 二次开发声明 (Derivative Work)
本人在原作者优秀的基础框架上，进行了深度的架构升级与二次开发，主要**原创增量**包括：
1. **AI 视觉闭环**：引入 PC 端 RTX 4050 运行 YOLOv8，实现目标检测。
2. **通信协议重构**：抛弃 HTTP 阻塞，手写 UDP 广播协议实现 <10ms 极低延迟控制。
3. **物联网化**：加入 mDNS 局域网发现与 ArduinoOTA 无线固件升级。
4. **状态机系统**：实现基于关键帧的动作录制与回放。

**⚖️ 许可与免责声明 (License & Disclaimer)**
本项目开源仅供**个人学习、技术研究与教育目的 (Educational & Non-Commercial Purpose)**。
严格遵守原作者的版权要求。任何人不得将本项目直接用于商业产品售卖。如需商业应用，请务必联系原作者 Tech Talkies 获取独立授权。
