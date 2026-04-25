#pragma once

#include "Core/Aliases.h"

struct FDelay
{
    template<uint32_t InTicks>
    FORCEINLINE static void Ticks() 
    {
#ifndef __IDE__
        __builtin_avr_delay_cycles(InTicks);
#endif
    }
    
    template<uint32_t InMicroseconds>
    FORCEINLINE static void Microseconds() 
    {
        Ticks<static_cast<uint32_t>(static_cast<double>(F_CPU) * InMicroseconds / 1000000.0)>();
    }
    
    template<uint32_t InMilliseconds>
    FORCEINLINE static void Milliseconds() 
    {
        Microseconds<InMilliseconds * 1000>();
    }
};
