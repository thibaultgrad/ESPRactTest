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
float MS_SPRAY;
float MS_Ratio;
float MaxRatio;
float MS_RETARD_DEMARRAGE;
float MS_Arret;
float D_Min_level_cuve;
bool Reset_counters;

unsigned long refresh_date;

enum Etats {
  Attente,
  Spraying,
  Attente_demarrage,
  Attente_Max_Ratio,
  Erreur,
  Niveau_produit_bas,
};

boolean etat_spray;

unsigned long loop_timer = 0UL;

long t_debut_etat;
unsigned int duree_etat;
int nb_ouvertures_vanne = 0;
bool presence = false;
Etats etat = Attente;

#define Onechantillons 4000
float tailleEch = Onechantillons;
bool OnTimes[Onechantillons];
int i;
float currentRatio = 0;
float refreshrate;
unsigned long lastratiotime;

#define pin_relais_Spray 14
//#define pin_moteur_relais2 4

#define pin_detection 16

int nb_spray_non_enregistre;
#define nb_spray_avt_refresh 10

const char* stateStr[] =
    {"Attente", "Pulverisation", "Attente demarrage", "Ratio trop important", "Erreur", "Niveau produit bas"};

void ReadSavedDatas() {
  savedDataStateService.read([](SavedDataState _state) {
    temps_total_spray = _state.temps_total_spray;
    nb_total_passage = _state.nb_total_passage;
    nb_ouvertures_vanne = _state.nb_total_ouverture_vanne;
  });
}
void ReadSettings() {
  settingsDataStateService.read([](SettingsDataState _state) {
    MS_SPRAY = _state.MS_SPRAY;
    MS_Ratio = _state.MS_Ratio;
    MaxRatio = constrain(_state.MaxRatio / 100, 0, 1);
    MS_RETARD_DEMARRAGE = _state.MS_RETARD_DEMARRAGE;
    MS_Arret = _state.MS_Arret;
    D_Min_level_cuve = _state.D_Min_level_cuve;
    Reset_counters = _state.Reset_counters;
    refreshrate = MS_Ratio / tailleEch;
  });
}

void UpdateSavedDatas() {
  savedDataStateService.update(
      [](SavedDataState& state) {
        state.temps_total_spray = temps_total_spray;
        state.nb_total_passage = nb_total_passage;
        state.nb_total_ouverture_vanne = nb_ouvertures_vanne;
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
        state.currentRatio = currentRatio;
        state.presence = presence;
        state.duree_etat = duree_etat / 1000.0;
        return StateUpdateResult::CHANGED;
      },
      "Jean");
}
void SprayOff() {
  t_debut_etat = millis();
  digitalWrite(pin_relais_Spray, LOW);
  // delay(2);
  // digitalWrite(pin_moteur_relais2, HIGH);
  etat_spray = 0;
}
void SprayOn() {
  t_debut_etat = millis();
  etat_spray = 1;
  digitalWrite(pin_relais_Spray, HIGH);
  nb_ouvertures_vanne += 1;
  // delay(2);
  // digitalWrite(pin_moteur_relais2, LOW);
}
void ajout_temps_spraying() {
  temps_total_spray += (double)(duree_etat) / (1000.0 * 3600.0);
  nb_total_passage++;
  UpdateSavedDatas();
}

void setup() {
  for (i = 0; i <= 999; i++) {
    OnTimes[i] = 0;
  }
  i = 0;
  // start serial and filesystem
  pinMode(pin_relais_Spray, OUTPUT);

  digitalWrite(pin_relais_Spray, LOW);

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

  pinMode(pin_detection, INPUT);

  t_debut_etat = millis();
  etat_spray = 0;
  lastratiotime = millis();
  loop_timer = millis();
  refreshrate = 0;
}

void loop() {
  if (abs((long)(lastratiotime - millis())) > refreshrate) {
    OnTimes[i] = etat_spray;
    currentRatio += etat_spray ? 0.00025f : 0.0f;
    currentRatio -= OnTimes[i <= tailleEch - 2 ? i + 1 : 0] ? 0.00025f : 0.0f;
    if (i <= tailleEch - 1) {
      i += 1;
    } else {
      i = 0;
    }
    lastratiotime = millis();
  }

  // run the framework's loop function
  esp8266React.loop();

  if (abs((long)(refresh_date - millis())) > 1000) {
    ReadSettings();
    if (Reset_counters == true) {
      nb_total_passage = 0;
      nb_ouvertures_vanne = 0;
      temps_total_spray = 0;
      Reset_counters = false;
      UpdateSavedDatas();
    }
    refresh_date = millis();
  }

  presence = digitalRead(pin_detection);
  duree_etat = (unsigned int)abs((long)(millis() - t_debut_etat));
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
      if (currentRatio > MaxRatio) {
        ajout_temps_spraying();
        t_debut_etat = millis();
        etat = Attente_Max_Ratio;
        SprayOff();
      } else if (duree_etat > MS_SPRAY) {
        ajout_temps_spraying();
        if (MS_RETARD_DEMARRAGE <= 0 && MS_Arret <= 0 && presence) {
          t_debut_etat = millis();
        } else {
          etat = Attente;
          SprayOff();
        }
      }

      break;
    }
    case (int)Attente_Max_Ratio: {
      if (currentRatio < MaxRatio) {
        t_debut_etat = millis();
        etat = Attente;
      }
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
