#ifndef Flog
#define Flog

#include <Arduino.h>
class LoggerFactory;

/*class Logger
{
private:
    void Log(String prefix, String severity ,String message);
    File logfile;
public:
    Logger(LoggerFactory* factory, String prefix);
    Logger();
    ~Logger();
    void setPrefix(String prefix);
    void Debug(String message);
    void Info(String message);
    void Warning(String message);
    void Error(String message);
};*/
class LoggerFactory
{
private:
    /* data */
    void Log(String prefix, String severity ,String message);
public:
    LoggerFactory(/* args */);
    ~LoggerFactory();
    //Logger createInstance(String prefix);
    void Begin();
    void Debug(String prefix, String message);
    void Info(String prefix, String message);
    void Warning(String prefix, String message);
    void Error(String prefix, String message);
    //static String data;
};
 #endif