#ifndef SavedDataStateService_h
#define SavedDataStateService_h


#include <HttpEndpoint.h>
#include <FSPersistence.h>
#include <ESPFS.h>

#define LED_PIN 2
#define PRINT_DELAY 5000


#define SAVED_DATAS_SETTINGS_ENDPOINT_PATH "/rest/SavedDataState"

#define SAVED_DATAS_SETTINGS_FILE "/config/SavedDataState.json"

class SavedDataState {
 public:
	double temps_total_spray;
	unsigned long nb_total_passage;
	unsigned long MS_SPRAY;
	unsigned int MS_RETARD_DEMARRAGE;
	unsigned int MS_Arret;
	unsigned int D_Min_level_cuve;
    bool Reset_counters;

  static void read(SavedDataState& settings, JsonObject& root) {
    root["temps_total_spray"] = settings.temps_total_spray;
    root["nb_total_passage"] = settings.nb_total_passage;
    root["MS_SPRAY"] = settings.MS_SPRAY;
    root["MS_RETARD_DEMARRAGE"] = settings.MS_RETARD_DEMARRAGE;
    root["MS_Arret"] = settings.MS_Arret;
    root["D_Min_level_cuve"] = settings.D_Min_level_cuve;
    root["Reset_counters"] = settings.Reset_counters;
  }

  static StateUpdateResult update(JsonObject& root, SavedDataState& savedState) {
    double newTSpray = root.containsKey("temps_total_spray") ? (double)root["temps_total_spray"]:1;
    unsigned long newTPassage = root.containsKey("nb_total_passage") ? root["nb_total_passage"]:1;
    unsigned long newMSpray = root.containsKey("MS_SPRAY") ? root["MS_SPRAY"]:1;
    unsigned int newMSRetard = root.containsKey("MS_RETARD_DEMARRAGE") ? root["MS_RETARD_DEMARRAGE"]:1;   
    unsigned int newMSArret = root.containsKey("MS_Arret") ? root["MS_Arret"]:1;
    unsigned int newMinLevel = root.containsKey("D_Min_level_cuve") ? root["D_Min_level_cuve"]:1;
    bool newReset = root.containsKey("Reset_counters") ? root["Reset_counters"]:false;

    if (((savedState.temps_total_spray) != newTSpray) | ((savedState.nb_total_passage) != newTPassage) | ((savedState.MS_SPRAY) != newMSpray) |((savedState.MS_RETARD_DEMARRAGE) != newMSRetard) |((savedState.MS_Arret) != newMSArret) |((savedState.D_Min_level_cuve) != newMinLevel)|((savedState.Reset_counters) != newReset) ) {
      savedState.temps_total_spray = newTSpray;
      savedState.nb_total_passage = newTPassage;
      savedState.MS_SPRAY = newMSpray;
      savedState.MS_RETARD_DEMARRAGE = newMSRetard;
      savedState.MS_Arret = newMSArret;
      savedState.D_Min_level_cuve = newMinLevel;
      savedState.Reset_counters=newReset;
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
