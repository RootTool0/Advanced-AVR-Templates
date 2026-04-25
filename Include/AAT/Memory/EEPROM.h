// ReSharper disable CppClangTidyPerformanceNoIntToPtr
#pragma once

#include "../Core/Aliases.h"

#include <avr/eeprom.h>

struct FEEPROM
{
    template <typename T>
    FORCEINLINE static void Write(uint16 InAddress, const T& InData)
    {
        eeprom_update_block(&InData, reinterpret_cast<void*>(InAddress), sizeof(T));
    }
    
    template <typename T>
    FORCEINLINE static void Read(uint16 InAddress, T& OutData)
    {
        eeprom_read_block(&OutData, reinterpret_cast<const void*>(InAddress), sizeof(T));
    }
    
    FORCEINLINE static void Clear(uint16 InAddress, uint16 InSize)
    {
        for(uint16 i = 0; i < InSize; ++i)
            eeprom_update_byte(reinterpret_cast<uint8*>(InAddress + i), 0);
    }
};
