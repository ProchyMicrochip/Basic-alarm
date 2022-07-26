#ifndef Captive_h
#define Captive_h
#include <ESPUI.h>


class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request){
    Serial.println(request->url());
    if(request->url() == "/generate_204" || request->url() == "/chat")
    return true;
    return false;
  }

  void handleRequest(AsyncWebServerRequest *request) {
    Serial.println("Captive handler");
    request->redirect("/#tab3");
  }
};
#endif