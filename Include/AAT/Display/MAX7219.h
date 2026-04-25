#pragma once

#include "../Delay.h"
#include "../Core/Aliases.h"
#include "../GPIO/Pin.h"

template<uint8 InDIO, uint8 InCS, uint8 InCLK, uint8 InSize = 1>
struct TMAX7219
{

public:
    FORCEINLINE static void Initialize()
    {
        PinDIO::InitializeAsOutput();
        PinCS ::InitializeAsOutput();
        PinCLK::InitializeAsOutput();
        
        PinCLK::Low();
        PinCS ::High();
        
        SendCommand(0x0F, 0x00); // Disable Test Mode (all LEDs on)
        SendCommand(0x0B, 0x07); // Set Scan Limit: Display all 8 rows (0-7)
        SendCommand(0x09, 0x00); // Set Decode Mode: No decoding
        SendCommand(0x0A, 0x01); // Set Display Intensity (0x00 to 0x0F)
        SendCommand(0x0C, 0x01); // Set Shutdown Register: Normal Operation (Wake up)
    }
    
    FORCEINLINE static void ClearAll()
    {
        for(uint8_t i = 1; i <= 8; ++i) SendCommand(i, 0x00);
    }
    
    FORCEINLINE static void DisplayRaw(const uint8* InRawPtr)
    {
        for(uint8 Row = 0; Row < 8; ++Row)
        {
            PinCS::Low();
            
            for(int8 Index = InSize - 1; Index >= 0; --Index)
            {
                ShiftOut(Row + 1);
                ShiftOut(InRawPtr[Index * 8 + Row]);
            }
        
            PinCS::High();
        }
    }
    
private:
    using PinDIO = TPin<InDIO>;
    using PinCS  = TPin<InCS >;
    using PinCLK = TPin<InCLK>;

    static void ShiftOut(uint8 Data)
    {
        for(int8 i = 7; i >= 0; --i)
        {
            PinDIO::Set((Data >> i) & 0x01);
            FDelay::Ticks<4>();
            PinCLK::High();
            FDelay::Ticks<4>();
            PinCLK::Low();
            FDelay::Ticks<4>();
        }
    }
    
    static void SendCommand(uint8 InRegister, uint8 InData)
    {
        PinCS::Low();
        for(uint8 i = 0; i < InSize; ++i) 
        {
            ShiftOut(InRegister);
            ShiftOut(InData);
        }
        PinCS::High();
    }
};
