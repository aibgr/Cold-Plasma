# Antibacterial Cold Plasma Generator – 100 kHz · 40 kV Variable High-Voltage High-Frequency System  
**Fully Designed, Built & Controlled with Embedded ATmega328 + Real-Time Condition Monitoring**  
2021

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Hardware](https://img.shields.io/badge/Hardware-Custom%20Built-success)](https://github.com/aibgr/Cold-Plasma-Jet)
[![Microcontroller](https://img.shields.io/badge/MCU-ATmega328P-critical)](#)

> **Supervisor:** Dr. Borna Beigzadeh – Head of Biomechatronics and Cognitive Engineering Lab (BACE), Iran University of Science and Technology  
> **Lead Designer:** A. Bagheri (B.Sc. MechE & CompE)

### Project Highlights
- Variable output: **100 kHz – 40 kV** cold atmospheric plasma generator
- Full custom power electronics chain: Function generator → Pre-driver → MOSFET bridge → Ferrite transformer → HV output
- Embedded real-time monitoring: Temperature (MOSFET + transformer), arc detection, over-current protection
- Safety-first design: Isolated feedback, soft-start, automatic shutdown
- All circuits designed, PCB routed, wound ferrite transformer, assembled and tested by me

### System Block Diagram
<img src="https://github.com/user-attachments/assets/535c2017-c1c3-419b-9036-e17158193c51" alt="Block diagram"/>

### Hardware Gallery

<table>
  <tr>
    <td align="center"><img src="https://github.com/user-attachments/assets/a4b93d95-1d00-41ac-9105-7c95f897c201" alt="Final assembly"></td>
    <td align="center"><img src="https://github.com/user-attachments/assets/bd473969-7df3-49f7-ad3f-5e3e214251cb" alt="Plasma jet in action"></td>
    <td align="center"><img src="https://github.com/user-attachments/assets/2922ab8e-79b1-46ad-9e40-b84fdbf30da8" alt="Back panel & connectors"></td>
  </tr>
  <tr>
    <td align="center"><img src="https://github.com/user-attachments/assets/c6d14dfe-e83a-4a29-af92-2cc8a906fe56" alt="Temperature sensors & MOSFET heatsink"></td>
    <td align="center"><img src="https://github.com/user-attachments/assets/23eb73cb-98fe-44fe-a692-d45a8aba2ab8" alt="Internal layout"></td>
    <td align="center"><img src="https://github.com/user-attachments/assets/7c9a3753-a06a-4e6f-8ab6-43a0bf22320b" alt="PWM driver & control board"></td>
  </tr>
</table>



### Key Technical Areas Covered
- High-frequency high-voltage power electronics
- Ferrite transformer design & winding
- Full-bridge MOSFET driver with dead-time control
- Analog & digital signal conditioning
- Embedded C programming (ATmega328P – Arduino core)
- PCB design (KiCad)
- Safety & EMI considerations

### Outcome
Stable, repeatable cold plasma jet successfully generated and used in preliminary biomedical tests at BACE Lab.

### Author
**A. Bagheri**  
B.Sc. Mechanical Engineering & Computer Engineering – IUST 2025  
GitHub: [@aibgr](https://github.com/aibgr)  
LinkedIn: [A-Bagheri](https://www.linkedin.com/in/aibgr)

If you find this project useful or inspiring, please give it a STAR  
Schematics and firmware will be released after related publications.

**One of my earliest full-stack hardware projects – still one of my favorites!**
