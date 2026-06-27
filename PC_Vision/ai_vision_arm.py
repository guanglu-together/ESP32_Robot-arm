import cv2
import numpy as np
import time
import socket 
from ultralytics import YOLO

# ESP32 的真实 IP
ESP32_IP = "" 
UDP_PORT = 8888

# UDP Socket (SOCK_DGRAM 代表 UDP)
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

print("正在加载 YOLOv8 AI 模型...")
model = YOLO('yolov8n.pt') 

cap = cv2.VideoCapture(0)

print("AI 视觉系统启动！RTX 4050 正在接管计算...")
print("已切换至 UDP 极速无连接模式！告别超时与卡顿！")
print("按 'q' 键退出。")

prev_time = time.time()

while True:
    ret, frame = cap.read()
    if not ret:
        break
        
    frame = cv2.resize(frame, (640, 480))
    H, W = frame.shape[:2]

    # RTX 4050 CUDA 
    results = model(frame, stream=True, verbose=False, device='0', imgsz=480)

    for result in results:
        boxes = result.boxes
        for box in boxes:
            cls_id = int(box.cls[0])
            if cls_id == 0: 
                x1, y1, x2, y2 = map(int, box.xyxy[0])
                conf = float(box.conf[0])
                
                cx = int((x1 + x2) / 2)
                cy = int((y1 + y2) / 2)
                
                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
                cv2.circle(frame, (cx, cy), 10, (0, 0, 255), -1)
                cv2.putText(frame, f"Person {conf:.2f}", (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

                lr_angle = int(np.interp(cx, [0, W], [30, 150]))
                fb_angle = int(np.interp(cy, [0, H], [140, 60]))
                
                # UDP 极速发送：不需要 connect，不需要 close，不需要 sleep！
                # 拼装成 "lr,fb" 格式直接扔给 ESP32
                msg = f"{lr_angle},{fb_angle}"
                udp_socket.sendto(msg.encode('utf-8'), (ESP32_IP, UDP_PORT))

    # 计算并显示 FPS
    curr_time = time.time()
    fps = 1.0 / (curr_time - prev_time + 0.0001)
    prev_time = curr_time
    fps_text = f"FPS: {fps:.1f}"
    cv2.putText(frame, fps_text, (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 0), 2)

    cv2.imshow("RTX 4050 AI Vision (YOLOv8 - UDP)", frame)
    
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()