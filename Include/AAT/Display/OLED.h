#pragma once

#include "../Core/Aliases.h"
#include "../Communication/TWI.h"

template<uint8 InAddress = 0x3C>
struct TOLED
{
    FORCEINLINE static void Initialize()
    {
        WriteCMD(0xAE);                 // Display OFF (sleep mode)
        
        WriteCMD(0xD5); WriteCMD(0x80); // Set OSC Frequency / Display Clock Divide
        WriteCMD(0xA8); WriteCMD(0x3F); // Set Multiplex Ratio (128x64)
        WriteCMD(0xD3); WriteCMD(0x00); // Set Display Offset (no offset)
        WriteCMD(0x40);                 // Set Display Start Line (0)
        
        WriteCMD(0x8D); WriteCMD(0x14); // Enable Charge Pump (Required for OLED panel)
        WriteCMD(0x20); WriteCMD(0x00); // Set Memory Addressing Mode (Horizontal)
        WriteCMD(0xA1);                 // Set Segment Re-map (Column address 127 is mapped to SEG0)
        WriteCMD(0xC8);                 // Set COM Output Scan Direction (Remapped mode)
        WriteCMD(0xDA); WriteCMD(0x12); // Set COM Pins Hardware Configuration
        
        WriteCMD(0x81); WriteCMD(0xCF); // Set Contrast Control
        WriteCMD(0xD9); WriteCMD(0xF1); // Set Pre-charge Period
        WriteCMD(0xDB); WriteCMD(0x40); // Set VCOMH Deselect Level
        
        WriteCMD(0xA4);                 // Entire Display ON (Resume from RAM)
        WriteCMD(0xA6);                 // Set Normal Display (Not inverted)
        WriteCMD(0xAF);                 // Display ON
    }
    
    FORCEINLINE static void SetCursor(uint8 InPage, uint8 InCol)
    {
        FTWI::Start();
        FTWI::Write(InAddress << 1);
        FTWI::Write(0x00);
        FTWI::Write(0xB0 | (InPage & 0x07));
        FTWI::Write(0x00 | (InCol & 0x0F));
        FTWI::Write(0x10 | ((InCol >> 4) & 0x0F));
        FTWI::Stop();
    }
    
    FORCEINLINE static void Fill(uint8 InData)
    {
        for(uint8 page = 0; page < 8; ++page)
        {
            SetCursor(page, 0);
            
            FTWI::Start();
            FTWI::Write(InAddress << 1);
            FTWI::Write(0x40);
            for(uint8 i = 0; i < 128; ++i) FTWI::Write(InData);
            FTWI::Stop();
        }
    }
    
    FORCEINLINE static void Clear() { Fill(0x00); }
    
    FORCEINLINE static void DrawByte(uint8 InPage, uint8 InCol, uint8 InData)
    {
        SetCursor(InPage, InCol);
        
        FTWI::Start();
        FTWI::Write(InAddress << 1);
        FTWI::Write(0x40);
        FTWI::Write(InData);
        FTWI::Stop();
    }
    
private:
    static void WriteCMD(uint8 InCMD)
    {
        FTWI::Start();
        FTWI::Write(InAddress << 1);
        FTWI::Write(0x00);
        FTWI::Write(InCMD);
        FTWI::Stop();
    }
};
