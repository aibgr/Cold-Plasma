# Antibacterial Cold Plasma Generator – 100 kHz · 40 kV  
**Variable High-Voltage High-Frequency System with Embedded Real-Time Control**  
2021

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Hardware](https://img.shields.io/badge/Hardware-Custom%20Built-success)](https://github.com/aibgr/Cold-Plasma-Jet)
[![Microcontroller](https://img.shields.io/badge/MCU-ATmega328P-critical)](#)

> **Supervisor:** Dr. Borhan Beigzadeh – Head of Biomechatronics and Cognitive Engineering Lab (BACE), Iran University of Science and Technology  
> **Lead Designer:** A. Bagheri (B.Sc. Mechanical Engineering & Computer Engineering)

---

## 🔬 Project Overview

A fully custom-designed **cold atmospheric plasma generator** capable of delivering high-frequency (≈100 kHz) and high-voltage (up to 40 kV) output for antibacterial and experimental applications.

The system integrates **power electronics, embedded control, sensing, and safety mechanisms** into a single platform, fully designed, built, and tested from scratch.

---

## ⚡ System Highlights

- Variable output: **~100 kHz / up to 40 kV**
- Full custom power chain:
  - Function generator  
  - Pre-driver stage  
  - MOSFET full-bridge  
  - Ferrite transformer  
  - High-voltage output stage  
- Embedded real-time monitoring:
  - Temperature (MOSFETs & transformer)  
  - Frequency & duty-cycle feedback  
  - Arc and abnormal condition detection  
- Safety-first architecture:
  - Isolated feedback paths  
  - Over-temperature protection  
  - Automatic shutdown & restart logic  
- Fully custom hardware:
  - PCB design (Altium)  
  - Transformer winding  
  - Assembly & testing  

---

## 🧠 Embedded Control System (Arduino Nano / ATmega328P)

The control unit is implemented using an **Arduino Nano (ATmega328P)** and demonstrates a **modular embedded C-style architecture using the Arduino C++ framework**.

### Key Responsibilities

- Startup UI (OLED): `"Cold Plasma"`
- Real-time display:
  - Frequency (Hz)  
  - Duty cycle (%)  
  - Temperature (°C)  
- Thermal management:
  - PWM fan control based on temperature  
- State machine control:
  - **Adjusting → Running → Overheat → Restart**  
- Stopwatch timing (using `micros()`) during operation phase  
- Safety signaling:
  - Buzzer + LED warning before critical conditions  
- Relay control:
  - Normally-closed relay for protection and system control  
- Overheat handling:
  - Forced cooling  
  - Controlled restart behavior  

---

## 🧩 Firmware Architecture (Public Demo)

This repository includes a **public demo firmware** designed to showcase code quality and system architecture without exposing sensitive implementation details.

### File Structure


cold_plasma_controller_demo.ino # main entry point

config.h # pins, thresholds, timing constants

system_types.h # shared data structures & state definitions

sensors.h / sensors.cpp # measurement layer

control.h / control.cpp # state machine & control logic

display_ui.h / display_ui.cpp # OLED interface



### Design Approach

- Modular layered architecture  
- Clear separation of:
  - sensing  
  - control logic  
  - hardware outputs  
  - user interface  
- State-machine driven system behavior  
- Non-blocking timing using `millis()` / `micros()`  

---

## ⚠️ Note on Public Firmware

This demo firmware intentionally:

- Uses **placeholder calibration values**  
- Simplifies measurement and protection logic  
- Omits hardware-specific tuning and proprietary algorithms  

Its purpose is to demonstrate:

> **Code structure, embedded design thinking, and system-level architecture**

—not to replicate the full production system.

---

## 📊 System Block Diagram

<img src="https://github.com/user-attachments/assets/535c2017-c1c3-419b-9036-e17158193c51" alt="Block diagram"/>

---

## 🖼️ Hardware Gallery

<table>
  <tr>
    <td align="center"><img src="https://github.com/user-attachments/assets/a4b93d95-1d00-41ac-9105-7c95f897c201"></td>
    <td align="center"><img src="https://github.com/user-attachments/assets/bd473969-7df3-49f7-ad3f-5e3e214251cb"></td>
    <td align="center"><img src="https://github.com/user-attachments/assets/2922ab8e-79b1-46ad-9e40-b84fdbf30da8"></td>
  </tr>
  <tr>
    <td align="center"><img src="https://github.com/user-attachments/assets/c6d14dfe-e83a-4a29-af92-2cc8a906fe56"></td>
    <td align="center"><img src="https://github.com/user-attachments/assets/23eb73cb-98fe-44fe-a692-d45a8aba2ab8"></td>
    <td align="center"><img src="https://github.com/user-attachments/assets/7c9a3753-a06a-4e6f-8ab6-43a0bf22320b"></td>
  </tr>
</table>

---

## 🛠️ Technical Domains

- High-frequency high-voltage power electronics  
- Ferrite transformer design & winding  
- Full-bridge MOSFET drivers with dead-time control  
- Embedded firmware design (ATmega328P)  
- Real-time monitoring and protection systems  
- PCB design (Altium Designer)  
- EMI & safety considerations  

---

## 👤 Author

**A. Bagheri**  
B.Sc. Mechanical Engineering & Computer Engineering – IUST (2021)  

GitHub: https://github.com/aibgr  
LinkedIn: https://www.linkedin.com/in/aibgr  

---

## ⭐ Support

If you find this project interesting or useful, consider giving it a **star**.

> Schematics and full firmware may be shared under collaboration or agreement.

---

**One of my earliest full-stack hardware projects — still one of my favorites!**
