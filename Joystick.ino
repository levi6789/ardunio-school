/*
  Simple Joystick Reader
  Reads X, Y analog values and the pushbutton state.
  Works with common 2-axis joysticks (e.g., KY-023).
*/

const int joyX = A0;   // X-axis connected to analog pin A0
const int joyY = A1;   // Y-axis connected to analog pin A1
const int joySW = 2;   // Switch connected to digital pin 2

int xValue = 0;
int yValue = 0;
int buttonState = 0;

void setup() {
  Serial.begin(9600);
  pinMode(joySW, INPUT_PULLUP); // button is usually active LOW
}

void loop() {
  // Read analog values (0–1023)
  xValue = analogRead(joyX);
  yValue = analogRead(joyY);

  // Read button state (LOW when pressed)
  buttonState = digitalRead(joySW);

  // Output to Serial Monitor
  Serial.print("X: ");
  Serial.print(xValue);
  Serial.print("\tY: ");
  Serial.print(yValue);
  Serial.print("\tButton: ");
  Serial.println(buttonState == LOW ? "Pressed" : "Released");

  delay(200); // Small delay for readability
}
