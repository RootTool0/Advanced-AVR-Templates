cmake_minimum_required(VERSION 3.18)

# Settings
set(AVR_GCC_PATH "C:/Dev/avr-gcc-15.2.0-x64-windows")
set(MCU "atmega328p")
set(F_CPU "8000000UL")
set(BAUD "115200")
set(PORT "COM5")

# Setup
project(Advanced-AVR-Templates)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source (IDE)
file(GLOB_RECURSE SOURCE_FILES_IDE CONFIGURE_DEPENDS "Source/*")
add_executable(${PROJECT_NAME} ${SOURCE_FILES_IDE})

# Source (AVR)
file(GLOB_RECURSE SOURCE_FILES_AVR "Source/*.cpp")
set(SOURCE_FILES_AVR_STRING "")

foreach(src ${SOURCE_FILES_AVR})
    set(SOURCE_FILES_AVR_STRING "${SOURCE_FILES_AVR_STRING} \"${src}\"")
endforeach()

# AVR
target_include_directories(${PROJECT_NAME} PRIVATE "${AVR_GCC_PATH}/avr/include")
target_include_directories(${PROJECT_NAME} PRIVATE "../Include")
add_compile_definitions(__IDE__)

file(GENERATE OUTPUT "Upload.bat" CONTENT
"@echo off
setlocal enabledelayedexpansion

echo [1/5] Compiling...
\"${AVR_GCC_PATH}/bin/avr-gcc.exe\" -std=c++17 -Os -flto ^
    -fno-exceptions -fno-rtti -fno-threadsafe-statics -ffunction-sections -fdata-sections -Wl,--gc-sections ^
    -mmcu=${MCU} -DF_CPU=${F_CPU} -I\"../../Include\" -I\"../Source\" -o \"${PROJECT_NAME}.elf\" ${SOURCE_FILES_AVR_STRING}

if %errorlevel% neq 0 (
    echo ERROR: Compilation failed!
    pause
    exit /b %errorlevel%
)

echo [2/5] Memory Usage Statistics:
\"${AVR_GCC_PATH}/bin/avr-size.exe\" -B \"${PROJECT_NAME}.elf\"

echo [3/5] Generating HEX...
\"${AVR_GCC_PATH}/bin/avr-objcopy.exe\" -j .text -j .data -O ihex \"${PROJECT_NAME}.elf\" \"${PROJECT_NAME}.hex\"

echo [4/5] Uploading...
\"${AVR_GCC_PATH}/bin/avrdude.exe\" -v -c arduino -p ${MCU} -P ${PORT} -b ${BAUD} -D -U flash:w:\"${PROJECT_NAME}.hex\":i

echo [5/5] Cleaning...
if exist \"${PROJECT_NAME}.elf\" del \"${PROJECT_NAME}.elf\"
if exist \"${PROJECT_NAME}.hex\" del \"${PROJECT_NAME}.hex\"

echo DONE!
:: pause"
)

# Call:
# mkdir Build
# cd Build
# cmake ..
