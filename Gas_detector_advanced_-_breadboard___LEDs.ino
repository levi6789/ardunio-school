const int alcoholPin = A0;   // Alcohol sensor input
const int smokePin = A1;     // Smoke sensor input

const int whiteLED = 8;      // White LED for Smoke
const int redLED = 9;        // Red LED for Alcohol

// Thresholds for alcohol sensor (relative to baseline)
const int alcoholLow = 10;
const int alcoholLowMed = 50;
const int alcoholMedHigh = 100;
const int alcoholHighVeryHigh = 200;
const int alcoholVeryHighDanger = 300;

// Thresholds for smoke sensor (relative to baseline)
const int smokeLow = 10;
const int smokeLowMed = 50;
const int smokeMedHigh = 100;
const int smokeHighVeryHigh = 200;
const int smokeVeryHighDanger = 300;

// Timing variables
unsigned long previousMillis = 0;
unsigned long serialMillis = 0;
const unsigned long serialInterval = 1000; // 1 second serial updates

// LED flashing states
bool alcoholLedState = false;
bool smokeLedState = false;
unsigned long alcoholPreviousMillis = 0;
unsigned long smokePreviousMillis = 0;

// Calibration baselines
int baselineAlcohol = 0;
int baselineSmoke = 0;

// Flash intervals for each level (in ms)
const int flashIntervals[6] = {0, 0, 1000, 500, 250, 100}; // index = level (0 to 5)

// Global flash intervals used in update functions
int alcoholFlashInterval = 0;
int smokeFlashInterval = 0;

void setup() {
  Serial.begin(9600);
  pinMode(whiteLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  digitalWrite(whiteLED, LOW);
  digitalWrite(redLED, LOW);

  calibrateSensors();
  Serial.println("Starting sensor monitoring...");
  Serial.println();
}

void loop() {
  unsigned long currentMillis = millis();

  // Update LED flashing based on sensor levels and timing
  updateAlcoholLED(currentMillis);
  updateSmokeLED(currentMillis);

  // Print sensor data every 1 second
  if (currentMillis - serialMillis >= serialInterval) {
    serialMillis = currentMillis;

    int alcoholRaw = analogRead(alcoholPin);
    int smokeRaw = analogRead(smokePin);

    int alcoholValue = max(0, alcoholRaw - baselineAlcohol);
    int smokeValue = max(0, smokeRaw - baselineSmoke);

    int alcoholLevel = getLevel(alcoholValue, alcoholLow, alcoholLowMed, alcoholMedHigh, alcoholHighVeryHigh, alcoholVeryHighDanger);
    int smokeLevel = getLevel(smokeValue, smokeLow, smokeLowMed, smokeMedHigh, smokeHighVeryHigh, smokeVeryHighDanger);

    Serial.print("Alcohol Raw: ");
    Serial.print(alcoholRaw);
    Serial.print(", Adjusted: ");
    Serial.print(alcoholValue);
    Serial.print(" (");
    Serial.print(levelToText(alcoholLevel));
    Serial.print(") | Smoke Raw: ");
    Serial.print(smokeRaw);
    Serial.print(", Adjusted: ");
    Serial.print(smokeValue);
    Serial.print(" (");
    Serial.print(levelToText(smokeLevel));
    Serial.println(")");

    Serial.print("Alcohol Level: ");
    Serial.println(levelToText(alcoholLevel));
    Serial.print("Smoke Level: ");
    Serial.println(levelToText(smokeLevel));
    Serial.println();

    // Update flashing intervals for LEDs based on current levels
    setAlcoholFlashInterval(alcoholLevel);
    setSmokeFlashInterval(smokeLevel);
  }
}

// Calibrate sensors by averaging readings for 5 seconds
void calibrateSensors() {
  Serial.println("Calibrating sensors, please wait 5 seconds...");
  const int samples = 100;
  long sumAlcohol = 0;
  long sumSmoke = 0;

  for (int i = 0; i < samples; i++) {
    sumAlcohol += analogRead(alcoholPin);
    sumSmoke += analogRead(smokePin);
    delay(50);
  }

  baselineAlcohol = sumAlcohol / samples;
  baselineSmoke = sumSmoke / samples;

  Serial.print("Calibration complete! Baselines - Alcohol: ");
  Serial.print(baselineAlcohol);
  Serial.print(", Smoke: ");
  Serial.println(baselineSmoke);
  Serial.println();
}

// Determine level based on thresholds
int getLevel(int value, int low, int lowMed, int medHigh, int highVeryHigh, int veryHighDanger) {
  if (value < low) return 0;
  else if (value < lowMed) return 1;
  else if (value < medHigh) return 2;
  else if (value < highVeryHigh) return 3;
  else if (value < veryHighDanger) return 4;
  else return 5;
}

// Convert level number to descriptive text
String levelToText(int level) {
  switch(level) {
    case 0: return "None";
    case 1: return "Low";
    case 2: return "Low-Medium";
    case 3: return "Medium-High";
    case 4: return "High-Very High";
    case 5: return "Very High-Dangerous";
    default: return "Unknown";
  }
}

// Update alcohol LED state based on flashing interval
void updateAlcoholLED(unsigned long currentMillis) {
  if (alcoholFlashInterval == 0) {
    // Solid ON or OFF depending on alcoholLedState
    digitalWrite(redLED, alcoholLedState ? HIGH : LOW);
    return;
  }

  if (currentMillis - alcoholPreviousMillis >= alcoholFlashInterval) {
    alcoholPreviousMillis = currentMillis;
    alcoholLedState = !alcoholLedState;
    digitalWrite(redLED, alcoholLedState ? HIGH : LOW);
  }
}

// Update smoke LED state based on flashing interval
void updateSmokeLED(unsigned long currentMillis) {
  if (smokeFlashInterval == 0) {
    // Solid ON or OFF depending on smokeLedState
    digitalWrite(whiteLED, smokeLedState ? HIGH : LOW);
    return;
  }

  if (currentMillis - smokePreviousMillis >= smokeFlashInterval) {
    smokePreviousMillis = currentMillis;
    smokeLedState = !smokeLedState;
    digitalWrite(whiteLED, smokeLedState ? HIGH : LOW);
  }
}

// Set flash interval for alcohol LED based on level
void setAlcoholFlashInterval(int level) {
  static int currentLevel = -1;

  if (level != currentLevel) {
    currentLevel = level;
    if (level == 0) {
      digitalWrite(redLED, LOW);
      alcoholFlashInterval = 0;
      alcoholLedState = false;
    } else if (level == 1) {
      digitalWrite(redLED, HIGH); // Solid ON
      alcoholFlashInterval = 0;
      alcoholLedState = true;
    } else {
      alcoholFlashInterval = flashIntervals[level];
      alcoholPreviousMillis = millis();
      alcoholLedState = false; // start off
    }
  }
}

// Set flash interval for smoke LED based on level
void setSmokeFlashInterval(int level) {
  static int currentLevel = -1;

  if (level != currentLevel) {
    currentLevel = level;
    if (level == 0) {
      digitalWrite(whiteLED, LOW);
      smokeFlashInterval = 0;
      smokeLedState = false;
    } else if (level == 1) {
      digitalWrite(whiteLED, HIGH); // Solid ON
      smokeFlashInterval = 0;
      smokeLedState = true;
    } else {
      smokeFlashInterval = flashIntervals[level];
      smokePreviousMillis = millis();
      smokeLedState = false; // start off
    }
  }
}
