void loadPreferences(bool prefsExist) {
  prefs.begin("arm");
  if (prefs.getInt("t") == 0) {
    Serial.printf("Detected 0 for top position...\n");
    prefsExist = false;
  }
  prefs.end();
  if (!prefsExist) writeCurrentValsToPrefs();
  prefs.begin("arm");
  servoTop         = prefs.getInt("t");
  servoBottom      = prefs.getInt("b");
  servoMiddle      = prefs.getInt("m");
  servoSide        = prefs.getInt("d");
  servoElbowOffset = prefs.getInt("e");
  jiggleTimes      = prefs.getInt("j");
  prefs.end();
}

void writeCurrentValsToPrefs() {
  Serial.printf("Rewriting default values\n");
  prefs.begin("arm");
  prefs.clear();
  prefs.putInt("t", servoTop);
  prefs.putInt("b", servoBottom);
  prefs.putInt("m", servoMiddle);
  prefs.putInt("d", servoSide);
  prefs.putInt("e", servoElbowOffset);
  prefs.putInt("j", jiggleTimes);
  prefs.end();
}

void printValues() {
  Serial.printf("\nCurrent Top (Wrist): %d\n", servoTop);    
  Serial.printf("Current Bottom (Wrist): %d\n", servoBottom);    
  Serial.printf("Current Middle (Shoulder): %d\n", servoMiddle);    
  Serial.printf("Current Side (Shoulder): %d\n", servoSide);    
  Serial.printf("Current Elbow Offset (Wrist - Elbow): %d\n", servoElbowOffset);    
  Serial.printf("Current number of jiggles: %d\n\n", jiggleTimes);    
  Serial.printf("Available serial commands:\n");  
  for (int i=0; i<inputChars.length(); i++) {    
    String key = inputChars.substring(i,i+1);
    Serial.printf("  %s - %s\n", key.c_str(), outputNames[i].c_str());  
  }
}

void saveJiggleTimes(int valu) {
  jiggleTimes = valu;
  writeCurrentValsToPrefs();            
}

void saveNewClosedPosition() {
  servoBottom = servoMotor[WRIST].readMicroseconds();
  servoMiddle = servoMotor[SHOULDER].readMicroseconds();
  servoElbowOffset = servoMotor[WRIST].readMicroseconds() - servoMotor[ELBOW].readMicroseconds();
  writeCurrentValsToPrefs();            
}

void saveNewOpenPosition() {
  servoTop = servoMotor[WRIST].readMicroseconds();
  servoSide = servoMotor[SHOULDER].readMicroseconds();
  servoElbowOffset = servoMotor[WRIST].readMicroseconds() - servoMotor[ELBOW].readMicroseconds();
  writeCurrentValsToPrefs();         
}
