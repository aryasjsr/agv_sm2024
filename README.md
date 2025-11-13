# AGV Smart Manufacturing 2024

![C++](https://img.shields.io/badge/C++-80.2%25-blue)
![C](https://img.shields.io/badge/C-19.8%25-green)
![Arduino](https://img.shields.io/badge/Platform-Arduino-00979D?logo=arduino)
![ESP32](https://img.shields.io/badge/Platform-ESP32-E7352C?logo=espressif)

## 📋 Deskripsi

AGV Smart Manufacturing 2024 adalah proyek Automated Guided Vehicle (AGV) yang dirancang untuk aplikasi smart manufacturing. Proyek ini mengintegrasikan line follower robot berbasis PID control dengan sistem IoT menggunakan ESP32/ESP8266 untuk kontrol jarak jauh melalui web interface.

## ✨ Fitur Utama

### Line Follower Robot
- **5 IR Sensors**: Sensor infrared untuk deteksi garis dengan akurasi tinggi
- **PID Controller**: Kontrol PID (Kp=20, Ki=0, Kd=5) untuk pergerakan smooth
- **Smart Navigation**: Deteksi persimpangan, belokan tajam, dan U-turn
- **Vision Integration**: Integrasi dengan sistem vision untuk obstacle detection
- **Dual Mode**: Mode otomatis (line following) dan manual (remote control)
- **Motor Control**: L298N motor driver dengan PWM speed control

### IoT Remote Control
- **ESP32/ESP8266 Web Server**: Interface web untuk kontrol jarak jauh
- **Async Web Server**: Non-blocking web server untuk responsivitas tinggi
- **Real-time Control**: Kontrol arah robot (Forward, Reverse, Left, Right, Stop)
- **Auto/Manual Switching**: Toggle antara mode otomatis dan manual
- **Arduino Communication**: Komunikasi serial antara ESP32 dan Arduino

## 🏗️ Struktur Proyek

```
agv_sm2024/
├── IoT/
│   ├── IoT/
│   │   ├── IoT.ino                    # Kode ESP32/ESP8266
│   │   └── index.h                    # Web interface HTML
│   ├── AsyncTCP-master.zip         # Library AsyncTCP
│   └── ESPAsyncWebServer-master.zip # Library Async Web Server
└── Line_Follower_Robot/
    └── Line_Follower_Robot.ino    # Kode Arduino untuk line follower
```

## 🔧 Hardware Requirements

### Line Follower Robot
- **Microcontroller**: Arduino Uno/Nano/Mega
- **Motor Driver**: L298N atau motor driver sejenis
- **IR Sensors**: 5x IR sensor module (TCRT5000 atau sejenis)
- **Motors**: 2x DC motors dengan encoder (opsional)
- **Switch**: Push button untuk start/stop
- **Power Supply**: Battery 7.4V-12V

### IoT Module
- **Microcontroller**: ESP32 atau ESP8266 (NodeMCU/Wemos D1)
- **Communication**: 6 digital pins untuk komunikasi dengan Arduino
- **Power Supply**: 5V dari Arduino atau external

### Pin Configuration

#### Arduino Pin Mapping
```cpp
// IR Sensors
Pin 2  - Sensor 1 (Left most)
Pin 3  - Sensor 2
Pin 4  - Sensor 3 (Center)
Pin 11 - Sensor 4
Pin 12 - Sensor 5 (Right most)

// Motor Driver (L298N)
Pin 5  - ENA (Left Motor Speed)
Pin 6  - IN1 (Left Motor)
Pin 7  - IN2 (Left Motor)
Pin 8  - IN3 (Right Motor)
Pin 9  - IN4 (Right Motor)
Pin 10 - ENB (Right Motor Speed)

// Switch
Pin 13 - Push Button (with internal pullup)

// ESP32 Communication
A0 - Auto/Manual mode
A1 - Stop command
A2 - Forward command
A3 - Reverse command
A4 - Left command
A5 - Right command
```

## 📦 Instalasi

### 1. Persiapan Arduino IDE

```bash
# Install Arduino IDE dari https://www.arduino.cc/en/software
# Tambahkan ESP32/ESP8266 board support:
# File -> Preferences -> Additional Board Manager URLs:
https://dl.espressif.com/dl/package_esp32_index.json
https://arduino.esp8266.com/stable/package_esp8266com_index.json
```

### 2. Install Libraries

**Untuk ESP32/ESP8266:**
- Extract `AsyncTCP-master.zip` ke folder libraries Arduino
- Extract `ESPAsyncWebServer-master.zip` ke folder libraries Arduino

**Atau install via Library Manager:**
- AsyncTCP (untuk ESP32) / ESPAsyncTCP (untuk ESP8266)
- ESPAsyncWebServer

### 3. Upload Code

**Upload ke Arduino:**
```bash
1. Buka Line_Follower_Robot/Line_Follower_Robot.ino
2. Pilih board: Arduino Uno/Nano/Mega
3. Pilih COM port yang sesuai
4. Upload
```

**Upload ke ESP32/ESP8266:**
```bash
1. Buka IoT/IoT/IoT.ino
2. Sesuaikan WiFi credentials di code
3. Pilih board: ESP32 Dev Module atau NodeMCU
4. Upload
5. Buka Serial Monitor untuk melihat IP address
```

## 🚀 Cara Penggunaan

### Mode Otomatis (Line Following)

1. **Power ON**: Nyalakan robot
2. **Posisikan Robot**: Letakkan robot di atas garis hitam
3. **Start**: Tekan push button sekali atau kirim command '4' via serial
4. **Robot akan:**
   - Mengikuti garis dengan PID control
   - Belok kiri/kanan di persimpangan
   - Melakukan U-turn jika kehilangan garis
   - Berhenti jika mendeteksi stop marker

### Mode Manual (Remote Control)

1. **Koneksi WiFi**: 
   - Robot akan membuat WiFi Access Point atau terhubung ke WiFi existing
   - Cari IP address di Serial Monitor

2. **Buka Web Interface**:
   ```
   http://[IP_ADDRESS]
   ```

3. **Kontrol**:
   - **Auto/Manual Toggle**: Switch mode
   - **↑ Forward**: Maju
   - **↓ Reverse**: Mundur
   - **← Left**: Belok kiri
   - **→ Right**: Belok kanan
   - **■ Stop**: Berhenti

### Integrasi Vision System

Robot menerima command via Serial untuk obstacle avoidance:

```cpp
'1' - Object detected on left (turn slight right)
'3' - Object detected on right (turn slight left)
'4' - Start/Resume movement
'5' - Stop movement
```

## 🔍 Algoritma Line Following

### Error Calculation

Robot menggunakan 5 sensor untuk menghitung posisi relatif terhadap garis:

```
Position:  Left4 | Left3 | Center | Right3 | Right4
Error:      +4   |  +3   |   0    |  -3    |  -4
```

### PID Formula

```cpp
P = error
I = previous_error + error
D = error - previous_error
PID_value = (Kp * P) + (Ki * I) + (Kd * D)

left_motor_speed = initial_speed - PID_value
right_motor_speed = initial_speed + PID_value
```

### Status Codes

- **99**: Normal line following
- **100**: Left turn detected
- **101**: Right turn detected
- **102**: Lost line / U-turn needed
- **103**: Stop marker detected
- **104**: Slight right adjustment (obstacle on left)
- **105**: Slight left adjustment (obstacle on right)

## ⚙️ Konfigurasi & Tuning

### PID Parameters

```cpp
float Kp = 20;  // Proportional gain
float Ki = 0;   // Integral gain
float Kd = 5;   // Derivative gain
```

**Tuning Tips:**
- ↑ Kp: Respon lebih cepat, tapi bisa oscillate
- ↑ Kd: Mengurangi oscillation, lebih smooth
- ↑ Ki: Mengurangi steady-state error (jarang digunakan untuk line follower)

### Motor Speed

```cpp
int initial_motor_speed = 110;  // Base speed (0-255)
```

Sesuaikan berdasarkan:
- Kondisi lantai (smooth/rough)
- Ketajaman belokan track
- Response time yang diinginkan

## 💡 Fitur Khusus

### 1. Smart Intersection Handling

Robot dapat mendeteksi dan menangani berbagai jenis persimpangan:
- T-junction (kiri/kanan)
- Cross intersection (+)
- Sharp turns (90°)
- U-turn

### 2. Obstacle Avoidance Integration

Integrasi dengan computer vision untuk:
- Deteksi objek di jalur
- Slight adjustment untuk menghindari objek
- Resume line following setelah melewati objek

### 3. Dual Control System

Seamless switching antara:
- **Automatic**: PID-based line following
- **Manual**: Web-based remote control

## 🛠️ Troubleshooting

### Robot Tidak Bergerak
- Cek koneksi motor driver
- Cek power supply (minimal 7.4V)
- Tekan push button untuk start
- Kirim command '4' via serial

### Robot Bergerak Tidak Stabil
- Tuning PID parameters
- Cek mounting sensor (harus sejajar)
- Cek jarak sensor ke lantai (optimal 5-10mm)
- Cek kondisi track (kontras hitam-putih)

### WiFi Tidak Terkoneksi
- Cek WiFi credentials di code
- Cek Serial Monitor untuk IP address
- Pastikan ESP32/ESP8266 ter-flash dengan benar
- Cek power supply ESP32

### Sensor Tidak Responsif
- Cek wiring sensor ke Arduino
- Test sensor dengan Serial Monitor
- Adjust sensor sensitivity (potensiometer on module)
- Pastikan track memiliki kontras yang baik

## 📝 Development Notes

- **Competition Ready**: Dirancang untuk kompetisi Smart Manufacturing 2024
- **Modular Design**: Easy to modify dan expand
- **Real-world Application**: Applicable untuk warehouse automation
- **Safety Features**: Multiple stop conditions untuk keamanan

## 📚 Resources

### Documentation
- [Arduino Reference](https://www.arduino.cc/reference/en/)
- [ESP32 Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
- [PID Control Explained](https://www.youtube.com/watch?v=wkfEZmsQqiA)

### Libraries
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- [AsyncTCP](https://github.com/me-no-dev/AsyncTCP)

## 👤 Author

**Arya Sangaji**
- Email: aryasangajiarya@gmail.com
- GitHub: [@aryasjsr](https://github.com/aryasjsr)

## 📄 License

TODO: License declaration

## 🏆 Acknowledgments

Proyek ini dikembangkan untuk kompetisi Smart Manufacturing 2024.

---

**Note**: Repository ini berisi implementasi lengkap AGV untuk aplikasi industri dengan fokus pada reliability, accuracy, dan ease of control.
