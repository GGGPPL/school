# LEGO_2 實驗結報
組別：4 / E24146107 E24126571 / 王翊權 蘇冠逸

## 實驗目的
1. 學習並實作 RobotC 環境中多種感測器（Sensors）的讀取與控制邏輯。
2. 掌握超音波感測器（Ultrasonic Sensor）的避障原理與距離判定。
3. 應用陀螺儀感測器（Gyro Sensor）進行精確的角度轉向控制。
4. 學習顏色感測器（Color Sensor）的色調（Hue）與名稱識別功能，實現多路徑導航。
5. 實踐觸碰感測器（Touch Sensor）作為程式啟動與停止的實體開關。

## 核心感測器與控制邏輯分析

### 1. 超音波感測器 (Ultrasonic Sensor)
- **原理與指令**：利用 `getUSDistance(S4)` 獲取障礙物距離（單位：cm）。
- **邏輯解析**：
    - 當距離大於預設值（如 20cm）時，馬達持續運轉；若小於該值，則停止馬達以避免碰撞。
    - 這屬於基礎的「環境感測反饋」，是自動避障功能的基礎。

### 2. 陀螺儀感測器 (Gyro Sensor)
- **指令分析**：
    - `resetGyro(S2)`：將當前角度歸零，這是精確轉向前的必要步驟。
    - `getGyroDegrees(S2)`：讀取累計旋轉角度。
    - `getGyroRate(S2)`：讀取角速度，可用於動態修正機器人的直行偏航（如實驗範例中根據旋轉速率自動補償左右輪速）。
- **轉向邏輯**：使用 `repeatUntil(getGyroDegrees(S2) > 90)` 搭配輪速差，可實現精確的 90 度轉向。

### 3. 顏色感測器 (Color Sensor)
- **多模式應用**：
    - 使用 `getColorName(S2)` 直接辨識顏色名稱，實現直觀的指令解析。
- **導航邏輯**：
    - **紅色 (colorRed)**：優先權最高，強制停止馬達。
    - **藍色/黃色 (colorBlue/colorYellow)**：觸發陀螺儀轉向程序，轉向期間不處理超音波避障。
    - **綠色 (colorGreen)**：啟動直行行為，並在直行狀態下啟用超音波監測。

---

## 實驗練習實作分析

### 任務一：避障輪型機器人
- **硬體配置**：雙輪驅動馬達 + S3 超音波感測器。
- **實作邏輯**：僅在「直行行為區」內啟用。當 `getUSDistance(S3)` 小於 `Near (15cm)` 時，執行原地迴轉（馬達反轉速差）以避開障礙物。

### 任務二：顏色指令導航
- **程式架構**：採用 `if-else if` 結構區分顏色指令。
    - **轉向實作**：配合 `resetGyro(S4)` 與 `repeatUntil` 確保旋轉精確達到 90 度後才停止。
    - **狀態維持**：設計了狀態檢查，確保在離開綠色標籤後仍能維持直行狀態，直到遇到其他指令。

### 任務三：Touch Sensor 開關
- **實作方式**：定義 `bool running` 變數。偵測到 S1 按下時，利用 `running = !running` 切換系統狀態，並搭配 `while(getTouchValue(S1) == 1)` 防止彈跳（Debounce）導致連續切換。

---

## 實驗影片證明
本實驗之完整運行過程（包含避障與顏色指令辨識）已上傳至以下連結供參考：
- **示範影片**：[LEGO EV3 實驗練習展示](https://youtube.com/shorts/G67dCyM9m-k?si=92D2K3-xzxRPRCmK)

---

## 程式碼
```c
#pragma config(Sensor, S1, Touch, sensorEV3_Touch)
#pragma config(Sensor, S2, Color, sensorEV3_Color)
#pragma config(Sensor, S3, US, sensorEV3_Ultrasonic)
#pragma config(Sensor, S4, Gyro, sensorEV3_Gyro)

#define Near 15
#define SPEED 30 

task main()
{
    bool running = false;
    eraseDisplay();

    while(true)
    {
        // --- 1. 系統啟停控制 ---
        if(getTouchValue(S1) == 1)
        {
            running = !running;
            if(!running) {
                setMotorSpeed(motorA, 0);
                setMotorSpeed(motorC, 0);
                displayTextLine(1, "SYSTEM: OFF");
            } else {
                displayTextLine(1, "SYSTEM: READY");
            }
            while(getTouchValue(S1) == 1); 
            delay(200);
        }

        if(running)
        {
            TLegoColors color = getColorName(S2);
            int distance = getUSDistance(S3);

            // --- 2. 指令解析與行為控制 ---

            // A. 停止指令：紅色
            if (color == colorRed) { 
                displayTextLine(2, "CMD: RED (STOP)");
                displayTextLine(3, "US: IGNORED");
                setMotorSpeed(motorA, 0);
                setMotorSpeed(motorC, 0);
            }
            // B. 轉向指令：藍色/黃色
            else if (color == colorBlue) { 
                displayTextLine(2, "CMD: BLUE (LEFT)");
                displayTextLine(3, "US: IGNORED");
                resetGyro(S4);
                repeatUntil(getGyroDegrees(S4) < -90) {
                    setMotorSpeed(motorA, SPEED);
                    setMotorSpeed(motorC, -SPEED);
                }
                setMotorSpeed(motorA, 0);
                setMotorSpeed(motorC, 0);
            }
            else if (color == colorYellow) { 
                displayTextLine(2, "CMD: YELLOW (RIGHT)");
                displayTextLine(3, "US: IGNORED");
                resetGyro(S4);
                repeatUntil(getGyroDegrees(S4) > 90) {
                    setMotorSpeed(motorA, -SPEED);
                    setMotorSpeed(motorC, SPEED);
                }
                setMotorSpeed(motorA, 0);
                setMotorSpeed(motorC, 0);
            }
            // C. 直行行為區：綠色指令 OR 維持狀態
            else {
                if (color == colorGreen || getMotorSpeed(motorA) != 0) {
                    displayTextLine(2, "STATUS: STRAIGHT");
                    
                    if (distance < Near) {
                        displayTextLine(3, "US: [BLOCK] AVOID");
                        setMotorSpeed(motorA, -SPEED); 
                        setMotorSpeed(motorC, SPEED);
                    } else {
                        displayTextLine(3, "US: [CLEAR] GO");
                        setMotorSpeed(motorA, SPEED); 
                        setMotorSpeed(motorC, SPEED);
                    }
                } 
                else {
                    displayTextLine(2, "STATUS: IDLE");
                    displayTextLine(3, "US: IGNORED");
                }
            }
        }
        delay(20);
    }
}
```
