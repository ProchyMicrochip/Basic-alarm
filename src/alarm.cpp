#include <alarm.h>
#include <Arduino.h>
#include "EEPROM.h"

Alarm::Alarm() {}

Alarm::Alarm(String name)
{
    Alarm::name = name;
    while (m_address < EEPROM_SIZE - ALARM_SIZE)
    {
        // tudu : load settings
        if (Alarm::is_alarm(m_address))
            m_address += ALARM_SIZE;
        else
            break;
    }

    start_time = 360;

    save(m_address);
}

Alarm::Alarm(int address, String name)
{
    m_address = address;
    save(m_address);
    Alarm::name = name;
}
int Alarm::getAddress()
{
    return m_address;
}
void Alarm::set_active(bool active)
{
    bitWrite(m_state, 7, active);
}
void Alarm::set_time(uint8_t hour, uint8_t minute)
{
    start_time = 60 * hour + minute;
}
void Alarm::set_state(uint8_t state)
{
    Alarm::m_state = state;
}
bool Alarm::should_ring(unsigned long time)
{
    int hours = (time % 86400L) / 3600;
    int minutes = (time % 3600) / 60;
    int day = ((time / 86400L) + 4) % 7;
    int start = 60 * hours + minutes;
    // TODO: Implement zmena casu
    if (bitRead(m_state, 7) && bitRead(m_state, day) && m_last_invoke <= time - 60 && Alarm::start_time == start)
        return true;
    return false;
}
void Alarm::ring(unsigned long time)
{
    int seconds = time % 60;
    // time - seconds_in_current_minute ?? - 1 or eq to should_ring
    m_last_invoke = time - seconds;
}

void Alarm::save(int address)
{
    EEPROM.write(address, ALARM_HEADER);
    EEPROM.write(address + sizeof(uint8_t), start_time);
    EEPROM.write(address + sizeof(uint8_t) + sizeof(uint16_t), m_state);

    EEPROM.commit();
}
