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
      _state.temps_total_spray = 1;
      _state.nb_total_passage = 1;
      _state.MS_SPRAY = 1;
      _state.MS_RETARD_DEMARRAGE = 1;
      _state.MS_Arret = 1;
      _state.D_Min_level_cuve = 1;
      _state.Reset_counters=false;
  _fsPersistence.readFromFS();
  onConfigUpdated();
}

void SavedDataStateService::onConfigUpdated() {

}
