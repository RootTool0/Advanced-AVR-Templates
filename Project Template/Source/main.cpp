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
    
    return 0;
}
