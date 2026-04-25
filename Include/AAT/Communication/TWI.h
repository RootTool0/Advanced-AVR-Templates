#pragma once

#include "../Core/Aliases.h"

#include <avr/io.h>

struct FTWI
{
    FORCEINLINE static void Initialize()
    {
        TWBR = 0x04; 
        TWSR = 0x00;
        TWCR = (1 << TWEN);
    }
    
    FORCEINLINE static void Start()
    {
        TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
        while (!(TWCR & (1 << TWINT)));
    }
    
    FORCEINLINE static void Stop()
    {
        TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
    }
    
    FORCEINLINE static void Write(uint8 InData)
    {
        TWDR = InData;
        TWCR = (1 << TWINT) | (1 << TWEN);
        while (!(TWCR & (1 << TWINT)));
    }
};
