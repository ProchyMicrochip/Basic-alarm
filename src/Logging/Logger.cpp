#include <Arduino.h>
#include <Logging/LoggerFactory.h>

class Logger
{
private:
    void Log(String prefix, String severity ,String message);
    String _prefix;
    LoggerFactory _factory;
    bool inicialized = false;
public:
    Logger(String prefix);
    ~Logger();
    void Debug(String message);
    void Info(String message);
    void Warning(String message);
    void Error(String message);
    void Iniciialize(LoggerFactory& factory);
};

Logger::Logger(String prefix){
    _prefix = prefix;
    inicialized = true;
}
Logger::~Logger(){}

void Logger::Iniciialize(LoggerFactory& factory){
    _factory = factory;
}
void Logger::Debug(String message){
    if(!inicialized){
        Serial.println(_prefix + " logger not inicialized");
        return;
    }
    _factory.Debug(_prefix,message);
}
void Logger::Info(String message){
    if(!inicialized){
        Serial.println(_prefix + " logger not inicialized");
        return;
    }
    _factory.Info(_prefix,message);
}
void Logger::Warning(String message){
    if(!inicialized){
        Serial.println(_prefix + " logger not inicialized");
        return;
    }
    _factory.Warning(_prefix,message);
}
void Logger::Error( String message){
    if(!inicialized){
        Serial.println(_prefix + " logger not inicialized");
        return;
    }
    _factory.Error(_prefix,message);
}