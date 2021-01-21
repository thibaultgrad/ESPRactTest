#ifndef PodomaticStateService_h
#define PodomaticStateService_h


#include <HttpEndpoint.h>
#include <WebSocketTxRx.h>

#define LIGHT_SETTINGS_SOCKET_PATH "/ws/lightState"


#define SAVED_DATAS_SETTINGS_ENDPOINT_PATH "/rest/PodomaticState"


class PodomaticState {
 public:
	String etat;
	float mesure_niveau;

  static void read(PodomaticState& settings, JsonObject& root) {
    root["etat"] = settings.etat;
    root["mesure_niveau"] = settings.mesure_niveau;
  }

  static StateUpdateResult update(JsonObject& root, PodomaticState& savedState) {

    String newTSpray;

    if(root.containsKey("etat"))
    {
      newTSpray= root.containsKey("etat") ;
    }
    else{
      newTSpray="mich";
    }

    //String newTSpray = root.containsKey("etat") ? root["etat"] : new String("mich");
    float newTPassage = root.containsKey("mesure_niveau") ? root["mesure_niveau"]:1;

    if (((savedState.etat) != newTSpray) | ((savedState.mesure_niveau) != newTPassage)  ) {
      savedState.etat = newTSpray;
      savedState.mesure_niveau = newTPassage;
      return StateUpdateResult::CHANGED;
    }
    return StateUpdateResult::UNCHANGED;
  }

};

class PodomaticStateService : public StatefulService<PodomaticState> {
 public:
  PodomaticStateService(AsyncWebServer* server,
                    SecurityManager* securityManager);
  void begin();

 private:
  HttpEndpoint<PodomaticState> _httpEndpoint;
  WebSocketTxRx<PodomaticState> _webSocket;
 
  void onConfigUpdated();
};

#endif
