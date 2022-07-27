#include <alarm.h>
#include <Arduino.h>
#include "EEPROM.h"

Alarm::Alarm(int address)
{
    Alarm::address = address;
}

void Alarm::save()
{
    EEPROM.write(address, 0xBB);
    EEPROM.write(address + sizeof(uint8_t), start_time);
    EEPROM.write(address + sizeof(uint8_t) + sizeof(uint16_t), state);
}

void Alarm::set_active(bool active)
{
    bitWrite(state, 7, active);
}
void Alarm::set_time(uint8_t hour, uint8_t minute)
{
    start_time = 60 * hour + minute;
}
void Alarm::set_state(uint8_t state)
{
    Alarm::state = state;
}
// TUDU : find
bool Alarm::should_ring(unsigned long time)
{
    int hours = (time % 86400L) / 3600;
    int minutes = (time % 3600) / 60;
    int day = ((time / 86400L) + 4) % 7;
    int start = 60 * hours + minutes;
    // TODO: Implement zmena casu
    if (bitRead(state, 7) && bitRead(state, day) && last_invoke <= time - 60 && Alarm::start_time == start)
        return true;
    return false;
}
void Alarm::ring(unsigned long time)
{
    int seconds = time % 60;
    // time - seconds_in_current_minute ?? - 1 or eq to should_ring
    last_invoke = time - seconds;
}
