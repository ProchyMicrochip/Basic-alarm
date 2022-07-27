#include <Arduino.h>
#include "EEPROM.h"

#define ALARM_SIZE 2 * sizeof(uint8_t) + sizeof(uint16_t)

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
    int address = 0;
    // start time in minutes
    uint16_t start_time = 0;
    // indexes 0..6 correspond to days and index 7 stores, whether the alarm is active
    uint8_t state = 0;
    unsigned long last_invoke = 0;

    void save();

public:
    Alarm(int address);
    void set_active(bool active);
    void set_time(uint8_t hour, uint8_t minute);
    void set_state(uint8_t state);
    bool should_ring(unsigned long time);
    // We could inject a DFRobotDFPlayerMini into this class and create a function to start and stop alarm
    void ring(unsigned long time);

    static Alarm init_from_eeprom(int address)
    {
        Alarm alarm(address);

        alarm.start_time = EEPROM.read(address + sizeof(uint8_t));
        alarm.state = EEPROM.read(address  + sizeof(uint8_t) + sizeof(uint16_t));

        return alarm;
    }
};