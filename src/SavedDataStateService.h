#ifndef SavedDataStateService_h
#define SavedDataStateService_h


#include <HttpEndpoint.h>
#include <FSPersistence.h>
#include <ESPFS.h>

#define LED_PIN 2
#define PRINT_DELAY 5000

#define DEFAULT_LED_STATE false
#define OFF_STATE "OFF"
#define ON_STATE "ON"



#define SAVED_DATAS_SETTINGS_ENDPOINT_PATH "/rest/SavedDataState"

#define SAVED_DATAS_SETTINGS_FILE "/config/SavedDataState.json"

class SavedDataState {
 public:
  boolean ledOn;
  int ledColor;

  static void read(SavedDataState& settings, JsonObject& root) {
    root["led_on"] = settings.ledOn;
    root["led_color"] = settings.ledColor;
  }

  static StateUpdateResult update(JsonObject& root, SavedDataState& lightState) {
    boolean newState = root["led_on"] | DEFAULT_LED_STATE;
    int newColor = root.containsKey("led_color") ? root["led_color"]:1;
    if (((lightState.ledOn) != newState) | ((lightState.ledColor) != newColor)) {
      lightState.ledOn = newState;
      lightState.ledColor = newColor;
      return StateUpdateResult::CHANGED;
    }
    return StateUpdateResult::UNCHANGED;
  }

};

class SavedDataStateService : public StatefulService<SavedDataState> {
 public:
  SavedDataStateService(AsyncWebServer* server,
                    SecurityManager* securityManager);
  void begin();

 private:
  HttpEndpoint<SavedDataState> _httpEndpoint;
  FSPersistence<SavedDataState> _fsPersistence;
 
  void onConfigUpdated();
};

#endif
