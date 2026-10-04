#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>

// กำหนดขาอุปกรณ์

#define POT_PIN 34
#define BUTTON_PIN 25
#define KY002_PIN 15

#define SERVO_PIN 26
#define RED_LED_PIN 14
#define GREEN_LED_PIN 12
#define BUZZER_PIN 13

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Servo LockServo;
int answerNumber = 0;
int questionNumber = 0;
int Time = 60;
bool Started = false;
bool Finished = false;
bool Shake = false;
unsigned long previousTime = 0;

// ประกาศ Function
void checkStart();
void countdownStart();
void Question();
void CheckAnswerButton();
void correctAnswerAction();
void wrongAnswerAction();
void checkShake();
void updateTime();
void readPotentiometer();
void updateOLED();
void defused();
void gameOver();

void setup()
{
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(KY002_PIN, INPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  LockServo.attach(SERVO_PIN);

  LockServo.write(0);

  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // เริ่มต้น OLED
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    // Serial.println("OLED failed");
    while (1)
      ;
  }

  oled.clearDisplay();
  oled.setTextColor(WHITE);

  oled.setTextSize(2);
  oled.setCursor(10, 12);
  oled.print("BOMB GAME");

  oled.setTextSize(1);
  oled.setCursor(28, 42);
  oled.print("PRESS BUTTON");

  oled.display();
}

// กด push button เพื่อเริ่มเกม (1)
void checkStart()
{
  if (digitalRead(BUTTON_PIN) == LOW)
  {
    delay(200);
    countdownStart();
    while (digitalRead(BUTTON_PIN) == LOW)
    {
      delay(10);
    }
  }
}

// นับถอยหลัง 3 วิ ก่อนเริ่มเกม (2)
void countdownStart()
{
  int i;
  for (i = 3; i >= 1; i--)
  {
    oled.clearDisplay();
    oled.setTextSize(2);
    oled.setCursor(55, 20);
    oled.println(i);
    oled.display();

    digitalWrite(BUZZER_PIN, HIGH);
    delay(150);
    digitalWrite(BUZZER_PIN, LOW);
    delay(850);
  }
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setCursor(35, 25);
  oled.println("START!");
  oled.display();

  questionNumber = 0;
  Time = 60;
  Started = true;
  Finished = false;
  Shake = false;
  previousTime = millis();

  Question();
  updateOLED();
}

// โครงสร้างข้อมูลโจทย์
struct QuestionItem
{
  String text;
  int bits[8]; // อาร์เรย์เก็บค่าบิต 8 บิต (0 หรือ 1)
};

// ประกาศโจทย์ 5 ข้อตามตาราง ASCII
QuestionItem questions[5] = {
    // ข้อ 1: หา 'g' (Hex: 0x67 -> Binary: 01100111) ส่งแบบ MSB
    {"FIND 'g' (MSB)", {0, 1, 1, 0, 0, 1, 1, 1}},

    // ข้อ 2: หา 'i' (Hex: 0x69 -> Binary: 01101001) ส่งแบบ LSB
    {"FIND 'i' (LSB)", {1, 0, 0, 1, 0, 1, 1, 0}},

    // ข้อ 3: หา 'H' (Hex: 0x48 -> Binary: 01001000) ส่งแบบ LSB
    {"FIND 'H' (LSB)", {0, 0, 0, 1, 0, 0, 1, 0}},

    // ข้อ 4: หา 'o' (Hex: 0x6F -> Binary: 01101111) ส่งแบบ MSB
    {"FIND 'o' (MSB)", {0, 1, 1, 0, 1, 1, 1, 1}},
  
    // ข้อ 5: หา 'N' (Hex: 0x4E -> Binary: 01001110) ส่งแบบ LSB
    {"FIND 'N' (LSB)", {0, 1, 1, 1, 0, 0, 1, 0}},   
};

int currentQuestionIdx = 0; // ข้อโจทย์ปัจจุบันที่สุ่มได้
int currentBitIndex = 0;    // ลำดับบิตปัจจุบันที่ผู้เล่นกำลังป้อน (0-7)
int userAnswers[8];         // อาร์เรย์เก็บคำตอบของผู้เล่น 8 บิต

// คำถาม
void Question()
{
  currentQuestionIdx = random(0, 5); // สุ่มโจทย์ข้อ 1-5
  currentBitIndex = 0;               // เริ่มป้อนที่บิตแรก (index 0)

  // รีเซ็ตคำตอบเดิม
  for (int i = 0; i < 8; i++)
  {
    userAnswers[i] = -1;
  }
}

// เช็คคำตอบตอนกด push button
void CheckAnswerButton()
{
  static bool lastBtnState = HIGH;
  bool currentBtnState = digitalRead(BUTTON_PIN);

  // ตรวจจับการกดปุ่ม
  if (lastBtnState == HIGH && currentBtnState == LOW)
  {
    delay(50);

    if (digitalRead(BUTTON_PIN) == LOW)
    {
      // บันทึกค่าบิตที่เลือก (0 หรือ 1)
      userAnswers[currentBitIndex] = answerNumber;
      currentBitIndex++;

      // เสียง Beep ตอบรับการกด
      digitalWrite(BUZZER_PIN, HIGH);
      delay(80);
      digitalWrite(BUZZER_PIN, LOW);

      // เมื่อผู้เล่นป้อนคำตอบครบทั้ง 8 บิต
      if (currentBitIndex >= 8)
      {
        bool isCorrect = true;

        // ตรวจสอบความถูกต้องกับเฉลยทีละบิต
        for (int i = 0; i < 8; i++)
        {
          if (userAnswers[i] != questions[currentQuestionIdx].bits[i])
          {
            isCorrect = false;
            break;
          }
        }

        // ตัดสินผลลัพธ์
        if (isCorrect)
        {
          correctAnswerAction();
        }
        else
        {
          wrongAnswerAction();
        }
      }
    }
  }
  lastBtnState = currentBtnState;
}

// เช็คคำตอบถูก
void correctAnswerAction()
{
  defused(); // กู้ระเบิดสำเร็จ

}

// เช็คคำตอบผิด
void wrongAnswerAction()
{
  // แจ้งเตือนไฟแดงและเสียงเตือน
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);

  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setCursor(25, 20);
  oled.println("WRONG!");
  oled.setTextSize(1);
  oled.setCursor(20, 45);
  oled.println("TRY AGAIN...");
  oled.display();

  delay(1000);
  
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // รีเซ็ตตำแหน่งบิตเพื่อเริ่มให้ผู้เล่นตอบข้อนี้ใหม่อีกครั้ง
  currentBitIndex = 0;
  for (int i = 0; i < 8; i++)
  {
    userAnswers[i] = -1;
  }
}

// ตรวจสอบการเขย่า
void checkShake()
{
  int sensorValue = digitalRead(KY002_PIN);
  // ถ้าเขย่า
  if (sensorValue == HIGH && Shake == false)
  {
    // ลดเวลาไป 5 วินาที
    Time = Time - 5;
    if (Time < 0)
    {
      Time = 0;
    }

    Shake = true;
    // Warning
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    oled.clearDisplay();
    oled.setTextSize(2);
    oled.setCursor(15, 10);
    oled.println("WARNING!");
    oled.setTextSize(1);
    oled.setCursor(15, 40);
    oled.println("SHAKE DETECTED");
    oled.setCursor(30, 52);
    oled.print("TIME -5 SEC");

    oled.display();
    delay(300);

    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }

  // รอจนกว่าสัญญาณจะกลับมาเป็น LOW
  if (sensorValue == LOW)
  {
    Shake = false;
  }
}

// UPDATE TIMER
void updateTime()
{
  unsigned long currentTime = millis();

  // นับถอยหลัง  1 วินาที
   if (currentTime - previousTime >= 1000)
  {
    previousTime = currentTime;
    Time--;

    // นับถอยหลัง 3 วินาทีสุดท้ายก่อนระเบิด
    if (Time <= 10 &&  Time > 0)
    {
     {
      // ส่งเสียงปี๊บเตือนจังหวะสั้นกระชับ
      digitalWrite(BUZZER_PIN, HIGH);
      digitalWrite(RED_LED_PIN, HIGH); // กระพริบไฟแดงพร้อมเสียง
      delay(150);
      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(RED_LED_PIN, LOW);
    }
    }
    if(Time <= 0)
    {
      Time = 0;
    } 
  }
}

// อ่านค่า Potentiometer
void readPotentiometer()
{
  int potValue = analogRead(POT_PIN);

  // answerNumber (0-9)
  answerNumber = map(potValue, 0, 4095, 0, 1);
}

// แสดงจอ OLED หลังจากเปิดเกม
void updateOLED()
{
  oled.clearDisplay();

  // Title
  oled.setTextSize(1);
  oled.setCursor(0, 0);
  oled.println("BOMB DEFUSAL");

  // Time
  oled.setCursor(85, 0);
  oled.print(Time);
  oled.println("s");

  // แสดงชื่อโจทย์ที่สุ่มได้
  oled.setCursor(0, 12);
  oled.print(questions[currentQuestionIdx].text);

  // แสดงบิตทั้ง 8 ช่องที่ผู้เล่นกำลังป้อนข้อมูล
  oled.setCursor(0, 26);
  oled.print("BIT ");
  oled.print(currentBitIndex + 1);
  oled.print("/8: ");
  for (int i = 0; i < 8; i++)
  {
    if (userAnswers[i] == -1)
    {
      oled.print("_"); // บิตที่ยังไม่ได้ใส่
    }
    else
    {
      oled.print(userAnswers[i]); // บิตที่ใส่แล้ว
    }
  }

  // แสดงค่าบิตปัจจุบันที่หมุนเลือกอยู่
  oled.setTextSize(2);
  oled.setCursor(55, 42);
  oled.print(answerNumber);

  oled.display();
}

// DEFUSED
void defused()
{
  Finished = true;
  Started = false;

  // Stop timer

  // Green LED ON
  digitalWrite(GREEN_LED_PIN, HIGH);
  // Red LED OFF
  digitalWrite(RED_LED_PIN, LOW);
  // Buzzer OFF
  digitalWrite(BUZZER_PIN, LOW);
  // Servo remains locked
  LockServo.write(0);

  // OLED
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setCursor(16, 5);
  oled.println("DEFUSED!");

  oled.setTextSize(1);
  oled.setCursor(25, 30);
  oled.print("TIME LEFT: ");
  oled.print(Time);
  oled.println("s");
  oled.setCursor(35, 48);
  oled.println("GOOD JOB!");
  oled.display();

  delay(10000);
}

// GAME OVER
void gameOver()
{
  Finished = true;
  Started = false;

  // Green OFF
  digitalWrite(GREEN_LED_PIN, LOW);
  // Red LED ON
  digitalWrite(RED_LED_PIN, HIGH);
  // Servo unlock
  LockServo.write(90);

  // OLED
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setCursor(20, 10);
  oled.println("BOOM!");
  oled.setTextSize(1);
  oled.setCursor(30, 40);
  oled.println("TIME OUT");
  oled.setCursor(25, 55);
  oled.println("GAME OVER");
  oled.display();

  // === เสียงระเบิดลากยาว (เมื่อหมดเวลา) ===
  digitalWrite(BUZZER_PIN, HIGH);  // เปิดเสียง Buzzer ค้างไว้
  digitalWrite(RED_LED_PIN, HIGH); // ไฟแดงติดค้าง
  delay(1500);                     // เสียงระเบิดลากยาว 1.5 วินาที (ปรับเพิ่ม/ลดได้ตรงนี้)
  digitalWrite(BUZZER_PIN, LOW);   // ปิดเสียง
  delay(200);

  // === เสียงไซเรนกระพริบหลังระเบิด 10 ครั้ง ===
  for (int i = 0; i < 10; i++)
  {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(RED_LED_PIN, HIGH);
    delay(200);

    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
    delay(100);
  }

  digitalWrite(BUZZER_PIN, LOW);   // ปิดเสียง Buzzer
  digitalWrite(RED_LED_PIN, HIGH); // ค้างไฟสีแดงไว้จบเกม
}

  // === เอฟเฟกต์เสียงระเบิด (Explosion Sound Effect) ===
  // 1. เสียงกระแทกตอนระเบิด (Noise/Tone Sweep)
  // for (int freq = 1500; freq > 100; freq -= 30) {
  //   tone(BUZZER_PIN, freq);
  //   digitalWrite(RED_LED_PIN, !digitalRead(RED_LED_PIN)); // ไฟแดงกระพริบถี่รวดเร็ว
  //   delay(5);
  // }
  // noTone(BUZZER_PIN); // ปิดเสียงระเบิดระลอกแรก

  // delay(100);

  // 2. เสียงไซเรนเตือนภัยหลังระเบิด (Alarm Siren) 10 ครั้ง
  // for (int i = 0; i < 10; i++)
  // {
  //   tone(BUZZER_PIN, 800); // เสียงสูง
  //   digitalWrite(RED_LED_PIN, HIGH);
  //   delay(150);
    
  //   tone(BUZZER_PIN, 400); // เสียงต่ำ
  //   digitalWrite(RED_LED_PIN, LOW);
  //   delay(150);
  // }
  
  // noTone(BUZZER_PIN);             // หยุดส่งเสียง
  // digitalWrite(BUZZER_PIN, LOW);
  // digitalWrite(RED_LED_PIN, HIGH); // ค้างไฟสีแดงไว้
// }

//   Alarm
//   int i;
//   for (i = 0; i < 10; i++)
//   {
//     digitalWrite(BUZZER_PIN, HIGH);
//     delay(200);
//     digitalWrite(BUZZER_PIN, LOW);
//     delay(100);
//     // Red LED blink
//     digitalWrite(RED_LED_PIN, !digitalRead(RED_LED_PIN));
//   }
//   digitalWrite(BUZZER_PIN, LOW);
//   digitalWrite(RED_LED_PIN, HIGH);
// }

void loop()
{
  // Waiting for player to start
  if (Started == false &&
      Finished == false)
  {
    checkStart();
  }

  // Game is running
  if (Started == true &&
      Finished == false)
  {
    updateTime();
    readPotentiometer();
    checkShake();
    CheckAnswerButton();
    updateOLED();

    //ถ้าตอบถูก/ผิดจนจบเกมแล้ว ไม่ต้องสั่ง updateOLED ทับ
    if (Started == true && Finished == false){
      updateOLED();
    }

    // Check if time is over
    if (Time <= 0)
    {
      gameOver();
    }
  }

  // ส่วนที่เพิ่ม กดปุ่มเพื่อเล่นใหม่
  if (Finished == true)
  {
    // ถ้ามีการกดปุ่ม
    if (digitalRead(BUTTON_PIN) == LOW)
    {
      delay(200); 

      // รีเซ็ตสถานะของไฟและ Servo กลับสู่ค่าเริ่มต้น
      digitalWrite(RED_LED_PIN, LOW);
      digitalWrite(GREEN_LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
      LockServo.write(0); // ล็อก Servo กลับเข้าที่เดิม

      // เปลี่ยนสถานะเพื่อกลับสู่โหมดเริ่มต้น
      Finished = false;
      Started = false;

      // แสดงหน้าจอ Home / Start เหมือนช่วง setup()
      oled.clearDisplay();
      oled.setTextColor(WHITE);
      oled.setTextSize(2);
      oled.setCursor(10, 12);
      oled.print("BOMB GAME");

      oled.setTextSize(1);
      oled.setCursor(28, 42);
      oled.print("PRESS BUTTON");
      oled.display();

      // รอจนกว่าจะปล่อยปุ่ม
      while (digitalRead(BUTTON_PIN) == LOW)
      {
        delay(10);
      }
    }
  }
}