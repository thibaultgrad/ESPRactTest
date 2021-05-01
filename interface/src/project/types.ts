export interface LightState {
  led_color: number;
  led_on: boolean;
}
export interface SavedDataState {
  temps_total_spray : number;
  nb_total_passage : number;
  nb_total_ouverture_vanne : number;

}

export interface SettingsDataState{
  MS_SPRAY : number;
  MS_Ratio : number;
  MaxRatio : number;
  MS_RETARD_DEMARRAGE : number;
  MS_Arret : number;
  D_Min_level_cuve : number;
  Reset_counters : boolean;
}

export interface LightMqttSettings { 
  unique_id : string;
  name: string;
  mqtt_path : string;
}

export interface PodomaticState{
  etat:string;
  currentRatio:number;
  presence:boolean;
  duree_etat:number;
}
