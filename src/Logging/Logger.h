#ifndef log
#define log


#include <Arduino.h>
#include <Logging/LoggerFactory.h>

class Logger
{
private:
    void Log(String prefix, String severity ,String message);
public:
    Logger(String prefix);
    ~Logger();
    void Iniciialize(LoggerFactory& factory);
    void Debug(String message);
    void Info(String message);
    void Warning(String message);
    void Error(String message);
};
#endif