export interface LightState {
  led_color: number;
  led_on: boolean;
}

export interface LightMqttSettings { 
  unique_id : string;
  name: string;
  mqtt_path : string;
}
