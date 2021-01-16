#include <SavedDataStateService.h>

SavedDataStateService::SavedDataStateService(AsyncWebServer* server,
                                     SecurityManager* securityManager) :
    _httpEndpoint(SavedDataState::read,
                  SavedDataState::update,
                  this,
                  server,
                  SAVED_DATAS_SETTINGS_ENDPOINT_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
     _fsPersistence(SavedDataState::read, SavedDataState::update, this, &ESPFS, SAVED_DATAS_SETTINGS_FILE)
 {

  // configure settings service update handler to update LED state
  addUpdateHandler([&](const String& originId) { onConfigUpdated(); }, false);
}

void SavedDataStateService::begin() {
  _state.ledOn = DEFAULT_LED_STATE;
  _state.ledColor = 1;
  _fsPersistence.readFromFS();
  onConfigUpdated();
}

void SavedDataStateService::onConfigUpdated() {

}

