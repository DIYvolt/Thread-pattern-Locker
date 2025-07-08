const int patternPins[9] = {2, 3, 4, 5, 6, 7, 8, 9, 10};
const int enterPin = 12;
const int ledPin = 11;

const int limitUp = A0;     // Unlock position switch (NO)
const int limitDown = A1;   // Lock position switch (NO)

const int motorIN1 = A4;    // Motor direction 1
const int motorIN2 = A5;    // Motor direction 2

int secretPattern[9] = {0, 1, 1, 0, 1, 0, 1, 0, 0};

bool actionTriggered = false;

void setup() {
  for (int i = 0; i < 9; i++) {
    pinMode(patternPins[i], INPUT_PULLUP);
  }

  pinMode(enterPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  pinMode(limitUp, INPUT_PULLUP);    // NO switches need pull-up
  pinMode(limitDown, INPUT_PULLUP);

  pinMode(motorIN1, OUTPUT);
  pinMode(motorIN2, OUTPUT);

  stopMotor();
  digitalWrite(ledPin, LOW);
}

void loop() {
  if (digitalRead(enterPin) == LOW && !actionTriggered) {
    delay(200);  // Debounce

    int inputPattern[9];
    for (int i = 0; i < 9; i++) {
      inputPattern[i] = digitalRead(patternPins[i]) == LOW ? 1 : 0;
    }

    actionTriggered = true;

    if (checkPattern(inputPattern)) {
      digitalWrite(ledPin, HIGH); // Correct → LED ON
      unlock();
    } else {
      digitalWrite(ledPin, LOW);  // Wrong → LED OFF
      lock();
    }
  }

  // Reset trigger when button released
  if (digitalRead(enterPin) == HIGH) {
    actionTriggered = false;
  }
}

bool checkPattern(int input[]) {
  for (int i = 0; i < 9; i++) {
    if (input[i] != secretPattern[i]) {
      return false;
    }
  }
  return true;
}

void unlock() {
  // Run motor until upper limit switch is triggered (pressed = LOW)
  while (digitalRead(limitUp) == HIGH) {
    digitalWrite(motorIN1, HIGH);
    digitalWrite(motorIN2, LOW);
  }
  stopMotor();
}

void lock() {
  // Run motor until lower limit switch is triggered
  while (digitalRead(limitDown) == HIGH) {
    digitalWrite(motorIN1, LOW);
    digitalWrite(motorIN2, HIGH);
  }
  stopMotor();
}

void stopMotor() {
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, LOW);
}
