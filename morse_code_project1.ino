#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


int morseButton = 9;
int modeButton = 10;


int ledOutput = 13;

int mode = 0;
int flag = 0;
int buttonState = 0;
int lastButtonState = 0;
int startPressed = 0;
int endPressed = 0;
int holdTime = 0;
int idle = 0;

int audio = 7;
int note = 1200;

String morse = "";

String morse_code[] = {
  ".-",
  "-...",
  "-.-.",
  "-..",
  ".",
  "..-.",
  "--.",
  "....",
  "..",
  ".---",
  "-.-",
  ".-..",
  "--",
  "-.",
  "---",
  ".--.",
  "--.-",
  ".-.",
  "...",
  "-",
  "..-",
  "...-",
  ".--",
  "-..-",
  "-.--",
  "--..",
  "-----",
  ".----",
  "..---",
  "...--",
  "....-",
  ".....",
  "-....",
  "--...",
  "--...",
  "---..",
  "----.",
};

String alphabet[] = {
  "A",
  "B",
  "C",
  "D",
  "E",
  "F",
  "G",
  "H",
  "I",
  "J",
  "K",
  "L",
  "M",
  "N",
  "O",
  "P",
  "Q",
  "R",
  "S",
  "T",
  "U",
  "V",
  "W",
  "X",
  "Y",
  "Z",
  "0",
  "1",
  "2",
  "3",
  "4",
  "5",
  "6",
  "7",
  "8",
  "9",
  "0",
};

void setup() {
  Serial.begin(9600);

  pinMode(morseButton, INPUT);
  pinMode(modeButton, INPUT);

  pinMode(ledOutput, OUTPUT);

  lcd.begin(16, 2);

}

void loop() {

  if (digitalRead(modeButton) == HIGH) {

    lcd.clear();

    if (mode == 0) {
      mode = 1;
    } else if (mode == 1) {
      mode = 0;
    }
  }

  if (mode == 0) {

    buttonState = digitalRead(morseButton);

    if (buttonState != lastButtonState) {

      if (buttonState == HIGH) {

        startPressed = millis();
        flag = 1;
        idle = 0;

      } else {

        endPressed = millis();
        holdTime = endPressed - startPressed;

        if (holdTime <= 300) {
          morse += ".";
        }

        if (holdTime > 300) {
          morse += "-";
        }

      }

    }

    lastButtonState = buttonState;

    if (flag == 1 & buttonState == LOW) {
      
      if (idle > 10) {
        flag = 0;

        String codeChar = morseDecriptor(morse);

        lcd.print(codeChar);
        Serial.println(morse);
        morseAudio(morse);

        morse = "";
      } else {
        idle++;
      }
    }
  }

  if (mode == 1) {

    morse = "";

    while (Serial.available()) {

      String a = Serial.readString();
      morse = morseEncriptor(a);

      Serial.println(a);
      lcd.print(morse);
      morseAudio(morse);

    }

  }

  delay(100);
}

String morseDecriptor(String message) {
  if (message != "") {

    for (byte i = 0; i < 26; i++) {

      if (message == morse_code[i]) {
        return alphabet[i];

      }
    }
  }
}

String morseEncriptor(String message) {
  
  if (message != "") {

    for (byte i = 0; i < 26; i++) {

      if (alphabet[i] == message) {
        return morse_code[i];

      }
    }
  }
}

void morseAudio(String morseChar) {

  for (byte i = 0; i < morseChar.length(); i++) {

    if (morseChar[i] == '-') {
     digitalWrite(ledOutput, HIGH);
     tone(audio, note, 300);
     delay(100);
     digitalWrite(ledOutput, LOW);
     noTone(audio);
     delay(200);
    } else {
      digitalWrite(ledOutput, HIGH);
  	  tone(audio, note, 100);
      delay(100);
      digitalWrite(ledOutput, LOW);
      noTone(audio);
      delay(200);
    }
  }
}

