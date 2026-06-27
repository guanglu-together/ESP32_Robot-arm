import cv2
import numpy as np
import requests
import time

# ESP32 IP
ESP32_IP = "" 
URL = f"http://{ESP32_IP}/servo" 

cap = cv2.VideoCapture(0)

last_send_time = 0
print("视觉系统启动中... (已开启画面缩放优化)")
print("警告：请务必关闭浏览器中的机械臂控制网页！")
print("按 'q' 键退出。")

while True:
    ret, frame = cap.read()
    if not ret:
        break
        
    # 核心优化：将 4K 画面强制缩小到 800x600
    # 1. 窗口大小完美适配屏幕 
    # 2. 极大降低 CPU 负担，让追踪更流畅
    frame = cv2.resize(frame, (800, 600))
    
    # 获取缩放后的真实宽高，用于后续的坐标映射
    H, W = frame.shape[:2] 

    # 将图像转换为 HSV 色彩空间
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    
    # 定义红色的 HSV 阈值
    lower_red1 = np.array([0, 100, 100])
    upper_red1 = np.array([10, 255, 255])
    lower_red2 = np.array([160, 100, 100])
    upper_red2 = np.array([180, 255, 255])
    
    mask1 = cv2.inRange(hsv, lower_red1, upper_red1)
    mask2 = cv2.inRange(hsv, lower_red2, upper_red2)
    mask = mask1 + mask2
    
    mask = cv2.erode(mask, None, iterations=2)
    mask = cv2.dilate(mask, None, iterations=2)
    
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    
    if len(contours) > 0:
        c = max(contours, key=cv2.contourArea)
        
        if cv2.contourArea(c) > 500: 
            M = cv2.moments(c)
            if M["m00"] != 0:
                cx = int(M["m10"] / M["m00"])
                cy = int(M["m01"] / M["m00"])
                
                # 画十字准星
                cv2.circle(frame, (cx, cy), 15, (0, 255, 0), 2)
                cv2.line(frame, (cx - 20, cy), (cx + 20, cy), (0, 255, 0), 2)
                cv2.line(frame, (cx, cy - 20), (cx, cy + 20), (0, 255, 0), 2)
                
                # 根据缩放后的 W 和 H 动态映射为舵机角度
                lr_angle = int(np.interp(cx, [0, W], [30, 150]))
                fb_angle = int(np.interp(cy, [0, H], [140, 60]))
                
                current_time = time.time()
                if current_time - last_send_time > 0.15: 
                    try:
                        requests.get(f"{URL}?lr={lr_angle}&fb={fb_angle}&ud=90&grip=0", timeout=0.5)
                        last_send_time = current_time
                        # 减少控制台打印频率，让画面更流畅
                        # print(f" Sent -> LR: {lr_angle}, FB: {fb_angle}") 
                    except requests.exceptions.Timeout:
                        pass # 静默处理超时
                    except requests.exceptions.ConnectionError:
                        print(f"找不到 ESP32！请检查 IP ({ESP32_IP})。")
                    except Exception as e:
                        print(f"网络错误: {e}") 

    # 显示画面 (现在它们会是完美的 800x600 大小)
    cv2.imshow("Robot Vision (Press 'q' to quit)", frame)
    cv2.imshow("Red Mask", mask)
    
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()