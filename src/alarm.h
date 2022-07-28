#include <Arduino.h>
#include "EEPROM.h"

#define FIRST_ALARM_ADDR sizeof(char) * 40
#define EEPROM_SIZE sizeof(byte) * 1024
#define ALARM_SIZE 2 * sizeof(uint8_t) + sizeof(uint16_t)
#define ALARM_HEADER 0xBB

/*#define SUN 0b00000001
#define MON 0b00000010
#define TUE 0b00000100
#define WED 0b00001000
#define THU 0b00010000
#define FRI 0b00100000
#define SAT 0b01000000 */

class Alarm
{
private:
    /* data */
    int m_address = FIRST_ALARM_ADDR;
    // start time in minutes
    // uint16_t start_time = 0;
    // indexes 0..6 correspond to days and index 7 stores, whether the alarm is active
    uint8_t m_state = 0;
    unsigned long m_last_invoke = 0;

public:
    String name;
    uint16_t start_time = 0;
    Alarm();
    explicit Alarm(String name);
    Alarm(int address, String name);
    int get_address();
    bool get_active();
    bool get_day(int day);
    void set_active(bool active);
    void set_time(uint8_t hour, uint8_t minute);
    void set_state(uint8_t state);
    bool should_ring(unsigned long time);
    // We could inject a DFRobotDFPlayerMini into this class and create a function to start and stop alarm
    void ring(unsigned long time);
    void save() const;
    void save(int address) const;

    static Alarm* init_from_eeprom(int address, String name)
    {
        auto* alarm = new Alarm(address, std::move(name));

        alarm->start_time = EEPROM.read(address + sizeof(uint8_t));
        alarm->m_state = EEPROM.read(address + sizeof(uint8_t) + sizeof(uint16_t));

        return alarm;
    }

    static bool is_alarm(int address)
    {
        return EEPROM.read(address) == ALARM_HEADER;
    }
};