#include <alarm.h>

Alarm::Alarm(/* args */) {}

void Alarm::set_active(bool active)
{
    Alarm::active = active;
}
void Alarm::set_time(int hour, int minute)
{
    Alarm::hour = hour;
    Alarm::minute = minute;
}
// TUDU : find 
bool Alarm::should_ring(int hour, int minute, unsigned long time)
{
    // TODO: Implement zmena casu
    if (Alarm::active && last_invoke < time - 60 && Alarm::hour == hour && Alarm::minute == minute)
        return true;
    return false;
}
void Alarm::ring(unsigned long time, int seconds) {
    // time - seconds_in_current_minute ?? - 1 or eq to should_ring
    last_invoke = time - seconds;
}
