int updatePulseData() {                                                           // write pulse data to arrays
  for (int i = 0; i < 2; i++) {
    if (analogRead(pulsePins[i]) > threshold) { 
      digitalWrite(ledPins[i], HIGH);                                             // switch led on
      if (thePulseStopwatch[i] + thePulseMinTime < millis()) {                    // not under the time for a regular pulse
        if (thePulseStopwatch[i] + thePulseMaxTime > millis()) {                  // in regular time for a pulse
          float bpm=60000/(millis()-thePulseStopwatch[i]);                        // this is the duration from one pulse to the next
          if (bpm>theBPMs[i]) theBPMs[i]++;                                       // if current bpm higher the bpm, increase
          if (bpm<theBPMs[i]) theBPMs[i]--;                                       // if current bpm lower the bpm, decrease
          //Serial.print(i);Serial.print(" ");Serial.println(theBPMs[i]);
        }
        thePulseStopwatch[i] = millis();                                          // reset stopwatch
      }
    } else {
      digitalWrite(ledPins[i], LOW);                                              // switch led off
    }
  }
}
