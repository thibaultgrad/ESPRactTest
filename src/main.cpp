#include <ESP8266React.h>
#include <SavedDataStateService.h>
#include <SettingsDataStateService.h>
#include <PodomaticStateService.h>

#define SERIAL_BAUD_RATE 115200

AsyncWebServer server(80);
ESP8266React esp8266React(&server);

SavedDataStateService savedDataStateService = SavedDataStateService(&server, esp8266React.getSecurityManager());

SettingsDataStateService settingsDataStateService =
    SettingsDataStateService(&server, esp8266React.getSecurityManager());
PodomaticStateService podomaticStateService = PodomaticStateService(&server, esp8266React.getSecurityManager());
double temps_total_spray;
unsigned long nb_total_passage;
unsigned int D_Min_mm;
unsigned int D_Max_mm;
unsigned long MS_SPRAY;
unsigned int MS_RETARD_DEMARRAGE;
unsigned int MS_Arret;
unsigned int D_Min_level_cuve;
bool Reset_counters;

unsigned long refresh_date;

enum Etats {
  Attente,
  Spraying,
  Attente_demarrage,
  Erreur,
  Niveau_produit_bas,
};

boolean etat_spray;

unsigned long loop_timer = 0UL;

long t_debut_etat;
unsigned int duree_etat;
bool presence = false;
Etats etat = Attente;

#define pin_moteur_relais1 2
#define pin_moteur_relais2 4

#define pin_detection 5

int nb_spray_non_enregistre;
#define nb_spray_avt_refresh 10

const char* stateStr[] = {"Attente", "Spraying", "Attente_demarrage", "Erreur", "Niveau_produit_bas"};

void ReadSavedDatas() {
  savedDataStateService.read([](SavedDataState _state) {
    temps_total_spray = _state.temps_total_spray;
    nb_total_passage = _state.nb_total_passage;
  });
}
void ReadSettings() {
  settingsDataStateService.read([](SettingsDataState _state) {
    MS_SPRAY = _state.MS_SPRAY;
    MS_RETARD_DEMARRAGE = _state.MS_RETARD_DEMARRAGE;
    MS_Arret = _state.MS_Arret;
    D_Min_level_cuve = _state.D_Min_level_cuve;
    Reset_counters = _state.Reset_counters;
  });
}

void UpdateSavedDatas() {
  savedDataStateService.update(
      [](SavedDataState& state) {
        state.temps_total_spray = temps_total_spray;
        state.nb_total_passage = nb_total_passage;
        return StateUpdateResult::CHANGED;
      },
      "Jean");
}
void UpdateSettings() {
  settingsDataStateService.update(
      [](SettingsDataState& state) {
        state.MS_SPRAY = MS_SPRAY;
        state.MS_RETARD_DEMARRAGE = MS_RETARD_DEMARRAGE;
        state.MS_Arret = MS_Arret;
        state.D_Min_level_cuve = D_Min_level_cuve;
        state.Reset_counters = Reset_counters;
        return StateUpdateResult::CHANGED;
      },
      "Jean");
}
void UpdatePodoState() {
  podomaticStateService.update(
      [](PodomaticState& state) {
        state.etat = (String)stateStr[(int)etat];
        state.mesure_niveau = 10;
        state.presence = presence;
        state.duree_etat = duree_etat / 1000.0;
        return StateUpdateResult::CHANGED;
      },
      "Jean");
}
void SprayOff() {
  t_debut_etat = millis();
  digitalWrite(pin_moteur_relais1, LOW);
  // delay(2);
  // digitalWrite(pin_moteur_relais2, HIGH);
  etat_spray = 0;
}
void SprayOn() {
  t_debut_etat = millis();
  etat_spray = 1;
  digitalWrite(pin_moteur_relais1, HIGH);
  // delay(2);
  // digitalWrite(pin_moteur_relais2, LOW);
}
void ajout_temps_spraying() {
  temps_total_spray += (double)(duree_etat) / (1000.0 * 3600.0);
  nb_total_passage++;
  UpdateSavedDatas();
}

void setup() {
  // start serial and filesystem
  Serial.begin(SERIAL_BAUD_RATE);

  // start the framework and demo project
  esp8266React.begin();

  savedDataStateService.begin();

  settingsDataStateService.begin();

  podomaticStateService.begin();

  refresh_date = millis();

  // start the server
  server.begin();

  ReadSavedDatas();

  pinMode(pin_moteur_relais1, OUTPUT);

  digitalWrite(pin_moteur_relais1, 0);

  pinMode(pin_detection, INPUT);

  t_debut_etat = millis();
  etat_spray = 0;
}

void loop() {
  // run the framework's loop function
  esp8266React.loop();

  if (abs(refresh_date - millis()) > 1000) {
    ReadSettings();
    if (Reset_counters == true) {
      nb_total_passage = 0;
      temps_total_spray = 0;
      Reset_counters = false;
      UpdateSavedDatas();
    }
    refresh_date = millis();
  }

  presence = digitalRead(pin_detection);
  duree_etat = (unsigned int)abs(millis() - t_debut_etat);
  UpdatePodoState();
  int val_etat = (int)etat;
  switch (val_etat) {
    case (int)Attente: {
      if ((presence) && (duree_etat > MS_Arret)) {
        etat = Attente_demarrage;
        t_debut_etat = millis();
      }
      break;
    }
    case (int)Attente_demarrage: {
      if (duree_etat > MS_RETARD_DEMARRAGE) {
        etat = Spraying;
        SprayOn();
      }
      break;
    }
    case (int)Spraying: {
      if (duree_etat > MS_SPRAY) {
        ajout_temps_spraying();
        if (MS_RETARD_DEMARRAGE <= 0 && MS_Arret <= 0 && presence) {
          ajout_temps_spraying();
          t_debut_etat = millis();
        } else {
          etat = Attente;
          SprayOff();
        }
      }

      break;
    }
    case (int)Erreur: {
      break;
    }
    case (int)Niveau_produit_bas: {
      break;
    }
    default: {
      break;
    }
  }
}
