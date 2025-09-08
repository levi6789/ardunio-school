const int MQ2_PIN = A0;       // Analog pin for MQ-2 sensor
const float RL = 10.0;        // Load resistance (kOhm)

float R0 = 10.0;              // Baseline sensor resistance in clean air

// Function to read sensor resistance Rs (kOhm)
float readRs() {
  int sensorValue = analogRead(MQ2_PIN);
  float voltage = sensorValue * (5.0 / 1023.0);
  if (voltage == 0) voltage = 0.0001;  // avoid division by zero
  float Rs = ((5.0 - voltage) * RL) / voltage;
  return Rs;
}

// Calculate ppm for each gas using datasheet log-log curves
// Formula: ppm = 10^((log10(Rs/R0) - b) / m)

// LPG (C3H8, C4H10)
float getLPGppm(float Rs_R0) {
  float m = -0.45;
  float b = 0.26;
  float log_ppm = (log10(Rs_R0) - b) / m;
  return pow(10, log_ppm);
}

// Methane (CH4)
float getMethanepmm(float Rs_R0) {
  float m = -0.38;
  float b = 0.22;
  float log_ppm = (log10(Rs_R0) - b) / m;
  return pow(10, log_ppm);
}

// Hydrogen (H2)
float getHydrogenppm(float Rs_R0) {
  float m = -0.48;
  float b = 0.08;
  float log_ppm = (log10(Rs_R0) - b) / m;
  return pow(10, log_ppm);
}

// Alcohol (C2H5OH)
float getAlcoholppm(float Rs_R0) {
  float m = -0.42;
  float b = 0.30;
  float log_ppm = (log10(Rs_R0) - b) / m;
  return pow(10, log_ppm);
}

// Smoke (general smoke, complex mixture)
float getSmokeppm(float Rs_R0) {
  float m = -0.43;
  float b = 0.53;
  float log_ppm = (log10(Rs_R0) - b) / m;
  return pow(10, log_ppm);
}

// Carbon Monoxide (CO)
float getCOppm(float Rs_R0) {
  float m = -0.39;
  float b = 0.22;
  float log_ppm = (log10(Rs_R0) - b) / m;
  return pow(10, log_ppm);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Calibrating MQ-2 sensor. Please wait and keep sensor in clean air...");
  
  float sumRs = 0;
  for (int i = 0; i < 50; i++) {
    sumRs += readRs();
    delay(100);
  }
  R0 = sumRs / 50.0;
  
  Serial.print("Calibration complete. R0 = ");
  Serial.print(R0);
  Serial.println(" kOhm");
}

void loop() {
  float Rs = readRs();
  float Rs_R0 = Rs / R0;

  Serial.print("Rs: ");
  Serial.print(Rs, 2);
  Serial.print(" kOhm | Rs/R0: ");
  Serial.print(Rs_R0, 2);
  Serial.println();

  Serial.print("LPG: ");
  Serial.print(getLPGppm(Rs_R0), 0);
  Serial.print(" ppm | Methane: ");
  Serial.print(getMethanepmm(Rs_R0), 0);
  Serial.print(" ppm | Hydrogen: ");
  Serial.print(getHydrogenppm(Rs_R0), 0);
  Serial.print(" ppm | Alcohol: ");
  Serial.print(getAlcoholppm(Rs_R0), 0);
  Serial.print(" ppm | Smoke: ");
  Serial.print(getSmokeppm(Rs_R0), 0);
  Serial.print(" ppm | CO: ");
  Serial.print(getCOppm(Rs_R0), 0);
  Serial.println(" ppm");

  delay(1500);
}
