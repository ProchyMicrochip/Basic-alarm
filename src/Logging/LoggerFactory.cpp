#include <Arduino.h>
//class LoggerFactory;
/*class Logger;
class Logger
{
private:
    void Log(String prefix, String severity ,String message);
    File logfile;
    String _prefix;
    LoggerFactory* _factory;
    bool valid = false;
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
    void Log(String prefix, String severity ,String message);
    
public:
    LoggerFactory();
    ~LoggerFactory();
    void Begin();
    //Logger createInstance(String prefix);
    void Debug(String prefix, String message);
    void Info(String prefix, String message);
    void Warning(String prefix, String message);
    void Error(String prefix, String message);
};

LoggerFactory::LoggerFactory(/* args */)
{
}

LoggerFactory::~LoggerFactory()
{
}
void LoggerFactory::Begin(){
    

}

/*Logger LoggerFactory::createInstance(String prefix){
    return Logger(this,prefix);
}*/

void LoggerFactory::Debug(String prefix, String message){
    Log(prefix, "Debug", message);
}
void LoggerFactory::Info(String prefix, String message){
    Log(prefix, "Info", message);
}
void LoggerFactory::Warning(String prefix, String message){
    Log(prefix, "Warning", message);
}
void LoggerFactory::Error(String prefix, String message){
    Log(prefix, "Error", message);
}
void LoggerFactory::Log(String prefix, String severity ,String message){
    String compiledmessage = String(millis()/1000) + " | " + prefix + " | " + severity + " | " + message;
    Serial.println(compiledmessage);
}



/*Logger::Logger(LoggerFactory* factory, String prefix){
    Serial.println(prefix);
    valid = true;
    _prefix = String(prefix);
    _factory = factory;
}
Logger::Logger(){
    valid = false;
}
Logger::~Logger(){
   
}
void Logger::setPrefix(String prefix){
    _prefix = prefix;
}

void Logger::Debug(String message){
    if(valid)
    _factory->Debug(_prefix,message);
}
void Logger::Info(String message){
    if(valid)
    _factory->Info(_prefix,message);
}
void Logger::Warning(String message){
    if(valid)
    _factory->Warning(_prefix,message);
}
void Logger::Error( String message){
    if(valid)
    _factory->Error(_prefix,message);
}*/

