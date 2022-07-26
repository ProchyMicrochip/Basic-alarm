#define MON 0b00000001
#define TUE 0b00000010
#define WED 0b00000100
#define THU 0b00001000
#define FRI 0b00010000
#define SAT 0b00100000
#define SUN 0b01000000

#define ENABLED 0b10000000
#define DISABLED 0b00000000

class Alarm
{
private:
    /* data */
    int hour, minute;
    bool active = true
    uint8_t 
    unsigned long last_invoke = 0;

public:
    Alarm();
    // TUDU : take an array of booleans accoring to days
    void set_active(bool active);
    void set_time(int hour, int minute);
    bool should_ring(int hour, int minute, unsigned long time);
    // We could inject a DFRobotDFPlayerMini into this class and create a function to start and stop alarm
    void ring(unsigned long time, int seconds);

    static Alarm init_from_eeprom()
    {
        return Alarm();
    }
};