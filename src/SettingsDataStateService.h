#ifndef SettingsDataStateService_h
#define SettingsDataStateService_h


#include <HttpEndpoint.h>
#include <FSPersistence.h>
#include <ESPFS.h>


#define SETTINGS_DATAS_SETTINGS_ENDPOINT_PATH "/rest/SettingsDataState"

#define SETTINGS_DATAS_SETTINGS_FILE "/config/SettingsDataState.json"

class SettingsDataState {
 public:
	float MS_SPRAY;
  float MS_Ratio;
  float MaxRatio;
	float MS_RETARD_DEMARRAGE;
	float MS_Arret;
	float D_Min_level_cuve;
    bool Reset_counters;

  static void read(SettingsDataState& settings, JsonObject& root) {
    root["MS_SPRAY"] = settings.MS_SPRAY;
    root["MS_RETARD_DEMARRAGE"] = settings.MS_RETARD_DEMARRAGE;
    root["MS_Arret"] = settings.MS_Arret;
    root["D_Min_level_cuve"] = settings.D_Min_level_cuve;
    root["Reset_counters"] = settings.Reset_counters;
    root["MS_Ratio"] = settings.MS_Ratio;
    root["MaxRatio"] = settings.MaxRatio;
  }

  static StateUpdateResult update(JsonObject& root, SettingsDataState& savedState) {
    float newMSpray = root.containsKey("MS_SPRAY") ? root["MS_SPRAY"]:1;
    float newMSprayMax = root.containsKey("MS_Ratio") ? root["MS_Ratio"]:1;
    float newMSprayMaxTO = root.containsKey("MaxRatio") ? root["MaxRatio"]:1;
    float newMSRetard = root.containsKey("MS_RETARD_DEMARRAGE") ? root["MS_RETARD_DEMARRAGE"]:1;   
    float newMSArret = root.containsKey("MS_Arret") ? root["MS_Arret"]:1;
    float newMinLevel = root.containsKey("D_Min_level_cuve") ? root["D_Min_level_cuve"]:1;
    bool newReset = root.containsKey("Reset_counters") ? root["Reset_counters"]:false;
    if ( ((savedState.MS_SPRAY) != newMSpray) |((savedState.MS_RETARD_DEMARRAGE) != newMSRetard) |((savedState.MS_Arret) != newMSArret) |((savedState.D_Min_level_cuve) != newMinLevel)|((savedState.Reset_counters) != newReset) | ((savedState.MS_Ratio) != newMSprayMax) |((savedState.MaxRatio) != newMSprayMaxTO) ) {
      savedState.MS_SPRAY = newMSpray;
      savedState.MS_Ratio = newMSprayMax;
      savedState.MaxRatio = newMSprayMaxTO;
      savedState.MS_RETARD_DEMARRAGE = newMSRetard;
      savedState.MS_Arret = newMSArret;
      savedState.D_Min_level_cuve = newMinLevel;
      savedState.Reset_counters=newReset;
      return StateUpdateResult::CHANGED;
    }
    return StateUpdateResult::UNCHANGED;
  }

};

class SettingsDataStateService : public StatefulService<SettingsDataState> {
 public:
  SettingsDataStateService(AsyncWebServer* server,
                    SecurityManager* securityManager);
  void begin();

 private:
  HttpEndpoint<SettingsDataState> _httpEndpoint;
  FSPersistence<SettingsDataState> _fsPersistence;
 
  void onConfigUpdated();
};

#endif
