void setup() {
  // Initialize all pins as output
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
}

void loop() {
  // Manually turn pins on and off
  digitalWrite(0, HIGH);
  delay(500);
  digitalWrite(0, LOW);

  digitalWrite(1, HIGH);
  delay(500);
  digitalWrite(1, LOW);

  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);

  digitalWrite(21, HIGH);
  delay(500);
  digitalWrite(21, LOW);

  digitalWrite(16, HIGH);
  delay(500);
  digitalWrite(16, LOW);

  digitalWrite(17, HIGH);
  delay(500);
  digitalWrite(17, LOW);
}
