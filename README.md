# Automated-LED
This project was developed during the summer of 2024 using an Arduino Uno microcontroller. It automates an RGB LED strip based on ambient light levels, using a photocell (light-dependent resistor) to sense the surrounding brightness.

**Features**
- Automatically turns on the LED strip when ambient light drops below a certain threshold.
- Gradually changes LED colors to create a dynamic lighting effect.
- Uses analog input from a photocell for real-time light level sensing.
- Fully coded in Arduino C/C++ using the built-in functions and no external libraries.

**Components Used**
- Arduino Uno
- RGB LED strip
- Photocell (LDR)
- Resistors
- Breadboard & jumper wires
- External power supply

**How It Works**
- The photocell reads the ambient light level.
- If the light level is below a set threshold (indicating darkness), the LED strip activates.
- The LED strip cycles through different RGB colors to provide a visually appealing effect.
- If the light level rises again (e.g., during the day, desk lamp is on), the strip turns off automatically.
