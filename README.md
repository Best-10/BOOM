# 💣 BOOM BOOM BASS 💣

💥 เกมปลดระเบิดโดยใช้ ESP32 เป็นเกมปลดระเบิดที่จะได้ฝึกทักษะการแปลงรหัส ASCII เป็น Binary โดยตัวเกม BOOM BOOM BASS เป็นเกม Interactive ที่ผู้เล่นต้องสวมบทบาทเป็นผู้กู้ระเบิด ทำการถอดรหัสตัวอักษร ASCII ออกมาเป็นลำดับบิต 8 บิต (MSB หรือ LSB) ภายในเวลาที่กำหนด 60 วินาที โดยหน้าจอ OLED จะทำการสุ่มโจทย์มา แล้วให้ผู้เล่นทำการใส่รหัสผ่านโดยการหมุน Potentiometer เพื่อเลือกบิต (0 หรือ 1) ตัว KY-002 จะใช้จับแรงสั่นสะเทือนของกล่องระเบิดถ้าโดนเวลาจะถูกลดลงไป 5 วินาที ปุ่ม BUTTON กดเมื่อยืนยันรหัส หากกู้สำเร็จเวลาจะหยุด Servo จะยังคงล็อกกล่องระเบิดไว้และ LED สีเขียวจะขึ้นเพื่อให้รู้ว่าปลดระเบิดสำเร็จ แต่ถ้าหากคำตอบผิดผู้เล่นจะต้องทำการใส่รหัสให้ถูกจนกว่าเวลาจะหมด ในกรณีที่ผู้เล่นไม่สามารถกู้รหัสได้ Servo จะทำการปลดล็อกกล่องระเบิด กล่องระเบิดจะทำงาน LED สีแดงจะขึ้น ผู้เล่นจะแพ้ทันที

---

## 🛠️ อุปกรณ์ที่ต้องใช้ (Hardware Requirements)

### 🧠 ไมโครคอนโทรลเลอร์ (Controller)
* **ESP32 DOIT DevKit V1**: หน่วยประมวลผลหลัก

### 📟 หน้าจอและตัวแสดงผล (Display & Indicators)
* **OLED Display (SSD1306 0.96" I2C)**: หน้าจอจะแสดงผลสถานะ คำถาม/โจทย์ ASCII และเวลานับถอยหลัง
* **Green LED**: หลอดไฟแสดงสถานะการกู้ระเบิดสำเร็จ
* **Red LED**: หลอดไฟแสดงสถานะ (เตือนเวลาใกล้หมด/ระเบิดทำงาน)
* **Buzzer**: ใช้ในการส่งสัญญาณเสียงเตือนและเสียงเอฟเฟกต์ประกอบการเล่น

### 🎛️ เซนเซอร์และอินพุต (Sensors & Inputs)
* **Potentiometer**: ใช้ในการหมุนเพื่อเลือกรหัส โดยเลือกบิท (0 หรือ 1)
* **Push Button**: ปุ่มกด ใช้ในการกดเริ่มเกม ยืนยันรหัส และกดรีเซ็ตเกม
* **KY-002 Vibration Sensor**: เซนเซอร์ตรวจจับการสั่นสะเทือน จะทำการหักเวลาเมื่อมีการโดนหรือเขย่ากล่องระเบิด

### 🔒 แอคชูเอเตอร์ (Actuator)
* **Servo Motor**: มอเตอร์ควบคุมสลักล็อกระเบิด (0° = ล็อก / 90° = จะปลดล็อกเมื่อระเบิดทำงาน)

---

## 📌 Block Diagram
<img width="582" height="916" alt="image" src="https://github.com/user-attachments/assets/9162e0ed-3015-4182-9a98-07161492b31c" />

---

## 🔄 ผังงานการทำงาน (Flowchart)
<img width="1740" height="1026" alt="image" src="https://github.com/user-attachments/assets/f99ed22c-225e-4c07-bef7-6a1ba70602b1" />

---

## 📝 รายการเอกสารทางเทคนิค (Component Datasheets)
| อุปกรณ์ (Component) | เอกสารอ้างอิง (Datasheet) |
|---|---|
| ESP32 DOIT DevKit V1 | |
| Potentiometer | |
| 0.96" OLED Display | |
| Buzzer | |
| Vibration (KY-002) | [Datasheet](https://drive.google.com/file/d/1RX5c7ZS-NEaHhLapKe-5Rhf1wcc78Wmq/view?usp=drive_link) |

---

## 🔌 การต่อสาย (Pin Configuration)
| Component | ESP32 Pin | Note |
|---|---|---|
| OLED SDA / SCL | GPIO 21 / GPIO 22 | Hardware I2C |
| Potentiometer | GPIO 34 | Analog Input (ADC1) |
| Push Button | GPIO 25 | Input Pull-up |
| Vibration (KY-002) | GPIO 15 | Digital Input |
| Servo Motor | GPIO 26 | PWM Control |
| LED(Red) | GPIO 14 | Digital Output |
| LED(Green)| GPIO 12 | Digital Output |
| Buzzer | GPIO 13 | Digital Output |

---

## ⚡ แผนภาพการต่อวงจร (Circuit Diagram)
<img width="1683" height="1475" alt="image" src="https://github.com/user-attachments/assets/db7ee5f1-5c6e-4e19-ade3-95ecb1e02fb3" />

---

## 💻 ซอฟต์แวร์และไลบรารี (Software & Libraries)

### ⚙️ Development Environment
* **Arduino IDE** (หรือ VS Code + PlatformIO)

### 📚 Required Libraries
* **Adafruit_GFX Library** (สำหรับจัดการกราฟิกบนหน้าจอ)
* **Adafruit_SSD1306** (สำหรับควบคุมจอ OLED SSD1306 ผ่าน I2C)
* **ESP32Servo** (สำหรับควบคุมมิวสิกสัญญาณ PWM ไปยัง Servo Motor บน ESP32)
* **Wire** (Built-in I2C Library)

---

## 💣 คู่มือและขั้นตอนการใช้งาน (How to Use & Quick Start)
1. เปิดอุปกรณ์ ระบบจะแสดง BOMB GAME
2. กด Push Button เพื่อเริ่มเกม
3. ระบบ Countdown 3 → 2 → 1
4. ระบบเริ่มนับเวลา 60 วินาที
5. ดูโจทย์บน OLED
6. หมุน Potentiometer เพื่อเลือกคำตอบ 0–1
7. กด Push Button เพื่อยืนยัน
8. หากตอบผิด สามารถเลือกคำตอบใหม่และลองอีกครั้ง
9. หากเกิดการสั่น KY-002 จะลดเวลาลง 5 วินาที
10. ตอบถูก → DEFUSED และ Servo lock เหมือนเดิม
11. หากเวลาหมด → GAME OVER และ Servo Unlock

---

## ⚙️ ขั้นตอนการทำงานของระบบ (System Workflow)
1. **Start / Setup**: ตั้งค่า Pin, OLED, LED, Buzzer, Servo และอุปกรณ์รับข้อมูล
2. **Game Initialization**: กดปุ่มเริ่มเกมและนับถอยหลัง 3 → 2 → 1 ก่อนเริ่มจับเวลา
3. **Question & Answer**: แสดงโจทย์บน OLED และให้ผู้เล่นใช้ Potentiometer เลือกคำตอบ
4. **Answer Checking**: ตรวจสอบคำตอบ หากถูกไปข้อถัดไป หากผิดแจ้งเตือนผ่าน LED และ Buzzer
5. **System Loop**: วนตรวจสอบคำตอบ เวลา และการสั่นแบบเรียลไทม์ จนกว่าจะ DEFUSED หรือ GAME OVER

---

## 📷 ภาพชิ้นงานและการติดตั้งจริง (Actual Device & Implementation)
<p align="center">
  <img width="30%" alt="BOOM1" src="https://github.com/user-attachments/assets/ba808248-9f1d-4c55-a6d0-eb8fc2ea1c28">
  &nbsp;&nbsp;&nbsp;
  <img width="30%" alt="BOOM2" src="https://github.com/user-attachments/assets/e649b795-7265-4d22-a70c-68978129f793">
</p>

---

## 🎬 วิดีโอสาธิตการทำงาน (Video Demonstration)
https://drive.google.com/file/d/1as-Y47HOoNO7oKxYfR1Myw3j9TWumdXS/view?usp=drive_link

---

## 📄 เอกสาร (Documentation)
https://sway.cloud.microsoft/WJD0RTaiVszRxPoC?ref=Link
