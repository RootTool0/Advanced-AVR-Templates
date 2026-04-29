# Advanced AVR Templates

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)]()
[![AVR](https://img.shields.io/badge/AVR-ATmega%2C%20ATtiny-orange)]()
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

**[EN]** | **[[RU]](README_RU.md)**

**Header-only C++17 framework for AVR microcontrollers**

---

## About

I noticed that in every new AVR project, I kept copying the same code: pin setup, delays, EEPROM handling, display drivers. Instead of doing this for the tenth time, I put all the accumulated code into a single **header-only** library

The focus is on **readable** and **type-safe** code that compiles into the same **optimal assembly** as **plain C**

No virtual functions, no RTTI, no exceptions - only **static polymorphism via templates** and **aggressive inlining**

---

## Features

| Module                    | File                     | Template/Struct                                                    | Description                                                |
|---------------------------|--------------------------|--------------------------------------------------------------------|------------------------------------------------------------|
| **Delays**                | `Delay.h`                | `FDelay`                                                           | Compile-time delay in clocks / microseconds / milliseconds |
| **Digital pins**          | `GPIO/Pin.h`             | `TPin<uint8 InPin>`                                                | Digital pin operations (Arduino-style mapping)             |
| **Buttons**               | `GPIO/PinButton.h`       | `TPinButton<uint8 InPin>`                                          | Button polling with pull-up + state flags                  |
| **PWM**                   | `GPIO/PinPWM.h`          | `TPinPWM<uint8 InPin, bool bInverted>`                             | Hardware timer PWM                                         |
| **Serial**                | `Communication/Serial.h` | `FSerial`                                                          | UART with compile-time baud rate                           |
| **I2C**                   | `Communication/TWI.h`    | `FTWI`                                                             | TWI (Two-Wire Interface)                                   |
| **EEPROM**                | `Memory/EEPROM.h`        | `FEEPROM`                                                          | Read / Write                                               |
| **PROGMEM**               | `Memory/PROGMEM.h`       | `FPROGMEM`                                                         | Flash data access                                          |
| **LED matrix**            | `Display/MAX7219.h`      | `TMAX7219<uint8 InDIO, uint8 InCS, uint8 InCLK, uint8 InSize = 1>` | MAX7219 driver                                             |
| **OLED**                  | `Display/OLED.h`         | `TOLED<uint8 InAddress = 0x3C>`                            | SSD1306 driver                                             |

---

## Quick Example

```cpp
#include "AAT/GPIO/Pin.h"
#include "AAT/Delay.h"

using MyLedPin = TPin<13>;

int main()
{
    MyLedPin::InitializeAsOutput();
    
    while(true)
    {
        MyLedPin::Toggle();
        FDelay::Milliseconds<1000>();
    }
}
```

**Comparison with Arduino Blink _(ATmega328P)_:**

|                      | **Arduino** | **AAT**       |
|----------------------|-------------|---------------|
| Flash (.text)        | 924 bytes   | **139 bytes** |
| RAM (.bss)           | 9 bytes     | **0 bytes**   |
| RAM (.data)          | 0 bytes     | **0 bytes**   |
| **Total size**       | 933 bytes   | **139 bytes** |

**Flash is 6.6x smaller, and RAM usage is zero.**

---

## Project Template

The repository also includes a `Project Template` folder - a ready-to-use structure for new projects.

### How to use

1. Copy the `Project Template` folder to a new name
2. (If needed) edit `CMakeLists.txt` - set MCU type, frequency, port, etc.
3. Run `Build.bat`

**What `Build.bat` does:**
- Copies `AAT` from `Include/` into the project folder
- Runs **CMake** with preset parameters
- Creates a `.sln` project in the `Build` folder
- Generates `Upload.bat` via **CMake** for flashing the MCU
- **Self-destructs** (deletes itself)

After the build, write your code and run `Upload.bat` to flash the MCU

_(`Build.bat` runs only once - this prevents accidentally overwriting your changes. Use **CMake** directly for subsequent builds!)_

### Manual setup (without the template)

1. Copy the `AAT` folder from `Include/` into your project
2. Compile with these flags:
   ```
   -std=c++17 -fno-exceptions -fno-rtti -Os
   ```

### Notes

For IDEs that don't support **AVR-specific extensions**, the macro `__IDE__` is provided

It substitutes AVR-specific constructs with neutral stubs - without affecting real compilation (the macro is **not defined** in production builds)

**AAT/Core/Aliases.h**
```cpp
#ifdef __IDE__

// Dirty hack to stop the IDE from complaining about AVR-specific constructs
#define F_CPU 0
#define __extension__
#define __volatile__
#define __DOXYGEN__
#define __AVR_ATmega328__

#endif
```

This doesn't affect the final binary - it's just a way to preserve syntax highlighting, autocompletion, and a clean error log in the IDE

---

## Why **Advanced AVR Templates** instead of **Arduino**?

**Arduino** is fine for prototyping, but its digital pins, `delay()`, `Serial`, etc., come with significant overhead *(bounds checking, indirect calls)*

**Advanced AVR Templates** provides the same convenient interface but with **zero-cost abstractions** - the compiler **aggressively inlines everything**, **generates direct `SBI`/`CBI` instructions**, and **evaluates all constants at compile time**

---

## Finally

**Advanced AVR Templates** was built for **harsh conditions** - when **every byte of RAM is precious**, and **indirect calls**, **stack overflow**, and **memory corruption** become critical problems

It all started with a project on an MCU with **1 KB of RAM**. The problem is that the **stack grows downward** (or upward - depends on the architecture), and static variables live nearby. Free memory is squeezed between them

With just one deeply nested call, the stack starts overwriting those variables - or variables overwrite the stack

Standard Arduino approaches (functions calling functions, temporary local variables, function pointers) only make things worse: indirect calls = more return addresses on the stack = deeper nesting = higher risk of memory corruption

I had to write my own ~~bicycle~~ library _(yes, I reinvented the wheel)_

### Core principles

- **Minimal RAM** - ideally 0 bytes
- **Minimal indirect calls** - no virtual functions, no function pointers
- **Minimal stack depth** - to stay away from static data
- **Everything possible at compile time** - templates, `constexpr`, `if constexpr`, `static_assert`

Yes, templates and aggressive inlining **increase Flash** usage. But usually **Flash is much larger**, while **RAM is not**. So:

> **"Heavy at compile time - light at runtime"**
>
> *(c) RootTool*

---

## License

This project is distributed under the MIT License - Do what you want. For more information, see the [LICENSE](LICENSE) file

---

## Feedback

If you have questions, suggestions, or bugs, you can contact me:

- **Telegram (channel/blog)**: [RootTool Blog](https://t.me/RootToolBlog)
- **Telegram (DM)**: [@RootTool28](https://t.me/RootTool28)
- **Discord**: `.roottool` (or `RootTool#5776`)
- **YouTube**: [RootTool](https://www.youtube.com/@RTEdits0)

_(please do not contact me without reason)_
