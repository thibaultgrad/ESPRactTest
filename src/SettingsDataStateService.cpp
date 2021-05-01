#include <SettingsDataStateService.h>

SettingsDataStateService::SettingsDataStateService(AsyncWebServer* server, SecurityManager* securityManager) :
    _httpEndpoint(SettingsDataState::read,
                  SettingsDataState::update,
                  this,
                  server,
                  SETTINGS_DATAS_SETTINGS_ENDPOINT_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _fsPersistence(SettingsDataState::read, SettingsDataState::update, this, &ESPFS, SETTINGS_DATAS_SETTINGS_FILE) {
  // configure settings service update handler to update LED state
  addUpdateHandler([&](const String& originId) { onConfigUpdated(); }, false);
}

void SettingsDataStateService::begin() {
  _state.MS_SPRAY = 1500;
  _state.MS_Ratio = 4000;
  _state.MaxRatio = 1500;
  _state.MS_RETARD_DEMARRAGE = 0;
  _state.MS_Arret = 0;
  _state.D_Min_level_cuve = 1;
  _state.Reset_counters = false;
  _fsPersistence.readFromFS();
  onConfigUpdated();
}

void SettingsDataStateService::onConfigUpdated() {
}
