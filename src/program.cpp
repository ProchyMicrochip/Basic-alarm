#include "Logging/LoggerFactory.h"
#include <DNSServer.h>
#include <ESPUI.h>
#include "pins.h"
#include "EEPROM.h"
#include <FastLED.h>

#if defined(ESP32)
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

#include <NTPClient.h>
#include <alarm.h>

#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

CRGB leds[9];

class Program
{
private:
    // Otto MyOtto;  //This is Otto!
public:
    void setup(LoggerFactory &myfactory);
    void loop(void);
};

LoggerFactory _factory;
uint16_t console;
const byte DNS_PORT = 53;
// IPAddress apIP(192, 168, 14, 204);
DNSServer dnsServer;

uint16_t button1;
uint16_t switchOne;
uint16_t status;
uint16_t tab1;
uint16_t tab2;
uint16_t tab3;
uint16_t slider1;
uint16_t slider2;
uint16_t slider3;
uint16_t slider4;
uint16_t wifissid;
uint16_t wifipass;
String wifis[20];

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

Alarm alarm;

SoftwareSerial dfserial(D6,D7);
DFRobotDFPlayerMini dfplayer;

// Logger OttoLog("Otto");

void timeChanged(Control *sender, int value)
{
    Serial.print("Alarm time changed to :");
    Serial.println(sender->value);

    int hours = (sender->value[0] - '0') * 10 + sender->value[1] - '0';
    int minutes = (sender->value[3] - '0') * 10 + sender->value[4] - '0';
    alarm.set_time(hours, minutes);
    alarm.set_invoked(false);
}

void alarmStateChanged(Control *sender, int value)
{
    alarm.set_invoked(false);
    switch (value)
    {
    case S_ACTIVE:
        Serial.print("Active:");
        alarm.set_active(true);
        break;

    case S_INACTIVE:
        Serial.print("Inactive");
        alarm.set_active(false);
        break;
    }
}

void numberCall(Control *sender, int value)
{
    Serial.println(sender->value);
}

void textCall(Control *sender, int value)
{
    Serial.print("Text: ID: ");
    Serial.print(sender->id);
    Serial.print(", Value: ");
    Serial.println(sender->value);
}

void slider(Control *sender, int value)
{
    Serial.print("Slider: ID: ");
    Serial.print(sender->id);
    Serial.print(", Value: ");
    Serial.println(sender->value);
}

void buttonCallback(Control *sender, int value)
{
    switch (value)
    {
    case B_DOWN:
        Serial.println("Button DOWN");
        break;

    case B_UP:
        Serial.println("Button UP");
        break;
    }
}

void buttonExample(Control *sender, int value)
{
    switch (value)
    {
    case B_DOWN:
        Serial.println("Status: Start");
        ESPUI.updateControlValue(status, "Start");

        ESPUI.getControl(button1)->color = ControlColor::Carrot;
        ESPUI.updateControl(button1);
        break;

    case B_UP:
        Serial.println("Status: Stop");
        ESPUI.updateControlValue(status, "Stop");

        ESPUI.getControl(button1)->color = ControlColor::Peterriver;
        ESPUI.updateControl(button1);
        break;
    }
}

void padExample(Control *sender, int value)
{
    switch (value)
    {
    case P_LEFT_DOWN:

        Serial.print("left down");
        break;

    case P_LEFT_UP:
        Serial.print("left up");
        break;

    case P_RIGHT_DOWN:

        Serial.print("right down");
        break;

    case P_RIGHT_UP:
        Serial.print("right up");
        break;

    case P_FOR_DOWN:

        Serial.print("for down");
        break;

    case P_FOR_UP:
        Serial.print("for up");
        break;

    case P_BACK_DOWN:

        Serial.print("back down");
        break;

    case P_BACK_UP:
        Serial.print("back up");
        break;

    case P_CENTER_DOWN:
        Serial.print("center down");
        break;

    case P_CENTER_UP:
        Serial.print("center up");
        break;
    }

    Serial.print(" ");
    Serial.println(sender->id);
}

void switchExample(Control *sender, int value)
{
    switch (value)
    {
    case S_ACTIVE:
        Serial.print("Active:");
        ESPUI.updateVisibility(tab2, true);
        ESPUI.updateVisibility(tab3, true);
        ESPUI.updateVisibility(status, true);
        break;

    case S_INACTIVE:
        Serial.print("Inactive");
        ESPUI.updateVisibility(tab2, false);
        ESPUI.updateVisibility(tab3, false);
        ESPUI.updateVisibility(status, false);
        break;
    }
    Serial.print(" ");
    Serial.println(sender->id);
}

void selectExample(Control *sender, int value)
{
    Serial.print("Select: ID: ");
    Serial.print(sender->id);
    Serial.print(", Value: ");
    Serial.println(sender->value);
}

void otherSwitchExample(Control *sender, int value)
{
    switch (value)
    {
    case S_ACTIVE:
        Serial.print("Active:");
        break;

    case S_INACTIVE:
        Serial.print("Inactive");
        break;
    }

    Serial.print(" ");
    Serial.println(sender->id);
}

void trim(Control *sender, int value)
{
}

void save(Control *sender, int value)
{
}

bool saveEEPROMWifi(int eeprom_addrs, String ssid, String pass)
{
    if (ssid.length() > 20 || pass.length() > 20 || ssid.length() == 0 || pass.length() == 0)
    {
        Serial.println("Credentials invalid");
        return false;
    }
    EEPROM.writeString(eeprom_addrs, ssid);
    EEPROM.writeString(eeprom_addrs + sizeof(char) * 20, pass);
    Serial.print("SSID: ");
    Serial.println(ssid);
    Serial.print("PASS: ");
    Serial.println(pass);
    if (EEPROM.commit())
    {
        Serial.println("Credencials saved succesfully");
        return true;
    }
    else
    {
        Serial.print("Error when writing to EEPROM");
        return false;
    }
}

void savewifi(Control *sender, int value)
{
    Serial.println(ESPUI.getControl(wifissid)->label);
    Serial.println(ESPUI.getControl(wifissid)->value);
    Serial.println(ESPUI.getControl(wifipass)->label);
    Serial.println(ESPUI.getControl(wifipass)->value);
    if (value == B_UP)
    {
        if (saveEEPROMWifi(sizeof(int) * 6, ESPUI.getControl(wifissid)->value, ESPUI.getControl(wifipass)->value))
            ESP.restart();
        Serial.println("Wifi saved to EEPROM");
        Serial.println(sender->id);
    }
}

void Program::setup(LoggerFactory &myfactory)
{
    pinMode(D1,INPUT);
    _factory = myfactory;

    alarm = Alarm::init_from_eeprom();
    alarm.set_active(true);
    // OttoLog.Iniciialize(_factory);
    dnsServer.start(DNS_PORT, "*", WiFi.status() == WL_DISCONNECTED ? WiFi.softAPIP() : WiFi.localIP());
    Serial.println("\n\nWiFi parameters:");
    Serial.print("Mode: ");
    Serial.println(WiFi.status() == WL_DISCONNECTED ? "Station" : "Client");
    Serial.print("IP address: ");
    Serial.println(WiFi.status() == WL_DISCONNECTED ? WiFi.softAPIP() : WiFi.localIP());
    Serial.println(WiFi.softAPIP());
    //-- ESPUI
    tab1 = ESPUI.addControl(ControlType::Tab, "Settings 1", "Settings 1");
    tab2 = ESPUI.addControl(ControlType::Tab, "Settings 2", "Settings 2");
    tab3 = ESPUI.addControl(ControlType::Tab, "Settings 3", "Settings 3");
    // ESPUI.updateVisibility(tab2,false);
    // ESPUI.updateVisibility(tab3,false);

    // Alarm

    // uint16_t text_time = ESPUI.text("Time", &timeChanged, ControlColor::Dark, "13:00");
    // ESPUI.setInputType(text_time, "time");

    // ESPUI.addControl(ControlType::Time, "Vyberte čas", "6:00", ControlColor::Carrot, tab1, &timeChanged);
    uint16_t text_time = ESPUI.addControl(ControlType::Text, "Vyberte čas", "06:00", ControlColor::Wetasphalt, tab1, &timeChanged);
    ESPUI.setInputType(text_time, "time");

    // switchOne = ESPUI.switcher("Aktivní?", &switchExample, ControlColor::Alizarin);
    switchOne = ESPUI.addControl(ControlType::Switcher, "Aktivní?", "1", ControlColor::Alizarin, tab1, &alarmStateChanged);

    // End Alarm

    status = ESPUI.addControl(ControlType::Label, "Status:", "Stop", ControlColor::Turquoise);
    switchOne = ESPUI.switcher("Switch one", &switchExample, ControlColor::Alizarin);
    // TRIMS
    ESPUI.addControl(ControlType::Pad, "Movement", "", ControlColor::Carrot, tab2, &padExample);
    uint16_t panel = ESPUI.addControl(ControlType::Label, "TRIMS", "LEFT LEG", ControlColor::Wetasphalt, tab3);
    slider1 = ESPUI.addControl(ControlType::Slider, "", "50", ControlColor::Wetasphalt, panel, &trim);
    ESPUI.addControl(ControlType::Label, "RIGHT LEG", "RIGHT LEG", ControlColor::Wetasphalt, panel);
    slider2 = ESPUI.addControl(ControlType::Slider, "", "50", ControlColor::Wetasphalt, panel, &trim);
    ESPUI.addControl(ControlType::Label, "LEFT FOOT", "LEFT FOOT", ControlColor::Wetasphalt, panel);
    slider3 = ESPUI.addControl(ControlType::Slider, "", "50", ControlColor::Wetasphalt, panel, &trim);
    ESPUI.addControl(ControlType::Label, "RIGHT FOOT", "RIGHT FOOT", ControlColor::Wetasphalt, panel);
    slider4 = ESPUI.addControl(ControlType::Slider, "", "50", ControlColor::Wetasphalt, panel, &trim);
    ESPUI.addControl(ControlType::Button, "Save", "Save", ControlColor::Wetasphalt, panel, &save);
    // WIFI
    uint16_t wifipanel = ESPUI.addControl(ControlType::Label, "WiFi", "", ControlColor::Emerald, tab3);
    wifissid = ESPUI.addControl(ControlType::Select, "ssid", "", ControlColor::Emerald, wifipanel, &selectExample);
    int networks = WiFi.scanNetworks();
    for (int i = 0; i < networks; i++)
    {
        wifis[i].reserve(20);
        wifis[i] = WiFi.SSID(i);
        const char *ssid = wifis[i].c_str();
        ESPUI.addControl(ControlType::Option, ssid, ssid, ControlColor::Emerald, wifissid);
    }
    wifipass = ESPUI.addControl(ControlType::Text, "Password", "", ControlColor::Emerald, wifipanel, &textCall);
    ESPUI.addControl(ControlType::Button, "Save", "Save", ControlColor::Wetasphalt, wifipanel, &savewifi);

    FastLED.addLeds<WS2812B, ARGB, RGB>(leds, 9);

    // OttoLog.Info("Homed");
    for (int whiteLed = 0; whiteLed < 14; whiteLed = whiteLed + 1)
    {
        // Turn our current led on to white, then show the leds
        leds[whiteLed] = CRGB::Red;

        // Show the leds (only one of which is set to white, from above)
        FastLED.show();

        // Wait a little bit
        delay(100);

        // Turn our current led back to black for the next loop around
        leds[whiteLed] = CRGB::Black;
    }

    delay(50);

    ESPUI.begin("ESPUI Control");
    if (WiFi.status() == WL_CONNECTED)
        timeClient.begin();
    dfserial.begin(9600);
    if (!dfplayer.begin(dfserial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    }
    dfplayer.volume(5);
    // TODO: handle error

}

void Program::loop(void)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        timeClient.update();
        unsigned long time = timeClient.getEpochTime();
        if (alarm.should_ring(timeClient.getHours(), timeClient.getMinutes(), time))
        {
            Serial.println("ty vole, ono to funguje");
            alarm.ring(time, timeClient.getSeconds());
            dfplayer.play(1);
        }
    }
    dnsServer.processNextRequest();
    if(digitalRead(D1)){
        dfplayer.stop();
    }
    //delay(20);
}
