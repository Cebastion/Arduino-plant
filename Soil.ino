#define CapasitiveSoilPIN A0

int getSoil() {
  int soil = analogRead(CapasitiveSoilPIN);
  int percent = map(soil, 0, 1023, 0, 100);
  /*Serial.print("Влажность почвы: ");
  Serial.print(percent);
  Serial.println("%");*/

  return 100 - percent;
};
