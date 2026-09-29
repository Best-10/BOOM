# 💣BOOM BOOM BASS💣
💥เกมปลดระเบิดโดยใช้ ESP32 เป็นเกมปลดระเบิดที่จะได้ฝึกทักษะการแปลงรหัส ASCII เป็น Binary โดยตัวเกม **"BOOM BOOM BASS"** เป็นเกม Interactive ที่ผู้เล่นต้องสวมบทบาทเป็นผู้กู้ระเบิด ทำการถอดรหัสตัวอักษร ASCII ออกมาเป็นลำดับบิต 8 บิต (MSB หรือ LSB) ภายในเวลาที่กำหนด 60 วินาที โดยหน้าจอ OLED จะทำการสุ่มโจทย์มา แล้วให้ผู้เล่นทำการใส่รหัสผ่านโดยการหมุน Potentiometer เพื่อเลือกบิต (0 หรือ 1) ตัว KY-002 จะใช้จับแรงสั่นสะเทือนของกล่องระเบิดถ้าโดนเวลาจะถูกลดลงไป 5 วินาที ปุ่ม BUTTON กดเมื่อยืนยันรหัส หากกู้สำเร็จเวลาจะหยุด Servo จะยังคงล็อกกล่องระเบิดไว้และ LED สีเขียวจะขึ้นเพื่อให้รู้ว่าปลดระเบิดสำเร็จ แต่ถ้าหากคำตอบผิดผู้เล่นจะต้องทำการใส่รหัสให้ถูกจนกว่าเวลาจะหมด ในกรณีที่ผู้เล่นไม่สามารถกู้รหัสได้ Servo จะทำการปลดล็อกกล่องระเบิด กล่องระเบิดจะทำงาน LED สีแดงจะขึ้น ผู้เล่นจะแพ้ทันที

## 🛠️ อุปกรณ์ที่ต้องใช้ (Hardware Requirements)

### 🧠 ไมโครคอนโทรลเลอร์ (Controller)
* 🧠 **ESP32 DOIT DevKit V1:** หน่วยประมวลผลหลัก

### 📥 อุปกรณ์อินพุต (Input Devices)
* 🎛️ **Potentiometer (10kΩ):** ใช้ในการหมุนเพื่อเลือกรหัส โดยเลือกบิท (`0` หรือ `1`) 
* 🔘 **Push Button:** ปุ่มกด ใช้ในการกดเริ่มเกม ยืนยันรหัส และกดรีเซ็ตเกม
* 📳 **Vibration Sensor (KY-002):** เซนเซอร์ตรวจจับการสั่นสะเทือน จะทำการหักเวลาเมื่อมีการโดนหรือเขย่ากล่องระเบิด

### 📤 อุปกรณ์เอาต์พุตและแสดงผล (Output & Display Devices)
* 🖥️ **0.96" OLED Display (SSD1306, I2C):** หน้าจอจะแสดงโจทย์ เวลานับถอยหลัง และสถานะบิตในการเลือกรหัสผ่าน
* ⚙️ **Servo Motor (SG90):** มอเตอร์ควบคุมสลักล็อกระเบิด (0° = ล็อก / 90° = จะปลดล็อกเมื่อระเบิดทำงาน)
* 💡 **LED (Red / Green):** หลอดไฟแสดงสถานะ (สีเขียว = กู้สำเร็จ / สีแดง = เตือนเวลาใกล้หมด/ระเบิดทำงาน)
* 🔊 **Buzzer:** ใช้ในการส่งสัญญาณเสียงเตือนและเสียงเอฟเฟกต์ประกอบการเล่น

##  Block Diagram

## ผังงานการทำงาน (Flowchart)

## อุปกรณ์ที่ต้องใช้ (Hardware Requirements)

## :memo:รายการเอกสารทางเทคนิค (Component Datasheets)
|อุปกรณ์ (Component)	| เอกสารอ้างอิง (Datasheet) | 
|---|---|
| OLED SDA / SCL |  | 
| Potentiometer |  |
| Push Button |  | 
| Vibration (KY-002) |  | 
| Servo Motor |  |
| LED | | 
| Buzzer |  | 

## :electric_plug:การต่อสาย (Pin Configuration)
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

## แผนภาพการต่อวงจร (Circuit Diagram)
<img src="https://github.com/user-attachments/assets/2b745aee-4b72-4df5-bde4-c917c992ea83" width="500">

## 
