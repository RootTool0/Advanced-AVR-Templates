#include "AAT/GPIO/Pin.h"
#include "AAT/Delay.h"

using LedPin = TPin<13>;

int main()
{
    LedPin::InitializeAsOutput();
    
    while(true)
    {
        LedPin::Toggle();
        FDelay::Milliseconds<1000>();
    }
    
    return 0;
}
