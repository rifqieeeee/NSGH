void setup() {
  // Initialize all pins as output
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
}

// Pola Default: Menyalakan LED secara bergantian
void polaDefault() {
  digitalWrite(0, HIGH); delay(500); digitalWrite(0, LOW);
  digitalWrite(1, HIGH); delay(500); digitalWrite(1, LOW);
  digitalWrite(2, HIGH); delay(500); digitalWrite(2, LOW);
  digitalWrite(21, HIGH); delay(500); digitalWrite(21, LOW);
  digitalWrite(16, HIGH); delay(500); digitalWrite(16, LOW);
  digitalWrite(17, HIGH); delay(500); digitalWrite(17, LOW);
}

// Pola 1: Dari bawah ke atas per sisi
void polaBawahKeAtas() {
  // Kiri dari bawah ke atas
  digitalWrite(0, HIGH); delay(300); digitalWrite(0, LOW);
  digitalWrite(1, HIGH); delay(300); digitalWrite(1, LOW);
  digitalWrite(2, HIGH); delay(300); digitalWrite(2, LOW);

  // Kanan dari atas ke bawah
  digitalWrite(21, HIGH); delay(300); digitalWrite(21, LOW);
  digitalWrite(16, HIGH); delay(300); digitalWrite(16, LOW);
  digitalWrite(17, HIGH); delay(300); digitalWrite(17, LOW);
}

// Pola 2: Zig-zag
void polaZigZag() {
  digitalWrite(0, HIGH); delay(300); digitalWrite(0, LOW);
  digitalWrite(21, HIGH); delay(300); digitalWrite(21, LOW);
  digitalWrite(1, HIGH); delay(300); digitalWrite(1, LOW);
  digitalWrite(16, HIGH); delay(300); digitalWrite(16, LOW);
  digitalWrite(2, HIGH); delay(300); digitalWrite(2, LOW);
  digitalWrite(17, HIGH); delay(300); digitalWrite(17, LOW);
}

// Pola 3: Semua LED menyala bersamaan
void polaSemuaMenyala() {
  digitalWrite(0, HIGH); digitalWrite(1, HIGH); digitalWrite(2, HIGH);
  digitalWrite(21, HIGH); digitalWrite(16, HIGH); digitalWrite(17, HIGH);
  delay(500);
  digitalWrite(0, LOW); digitalWrite(1, LOW); digitalWrite(2, LOW);
  digitalWrite(21, LOW); digitalWrite(16, LOW); digitalWrite(17, LOW);
  delay(500);
}

// Pola 4: Dari kiri ke kanan lalu kanan ke kiri
void polaKiriKeKananDanBalik() {
  // Kiri ke kanan
  digitalWrite(0, HIGH); delay(300); digitalWrite(0, LOW);
  digitalWrite(1, HIGH); delay(300); digitalWrite(1, LOW);
  digitalWrite(2, HIGH); delay(300); digitalWrite(2, LOW);
  digitalWrite(21, HIGH); delay(300); digitalWrite(21, LOW);
  digitalWrite(16, HIGH); delay(300); digitalWrite(16, LOW);
  digitalWrite(17, HIGH); delay(300); digitalWrite(17, LOW);

  // Kanan ke kiri
  digitalWrite(17, HIGH); delay(300); digitalWrite(17, LOW);
  digitalWrite(16, HIGH); delay(300); digitalWrite(16, LOW);
  digitalWrite(21, HIGH); delay(300); digitalWrite(21, LOW);
  digitalWrite(2, HIGH); delay(300); digitalWrite(2, LOW);
  digitalWrite(1, HIGH); delay(300); digitalWrite(1, LOW);
  digitalWrite(0, HIGH); delay(300); digitalWrite(0, LOW);
}

// Pola 5: Knight Rider
void polaKnightRider() {
  digitalWrite(0, HIGH); delay(200); digitalWrite(0, LOW);
  digitalWrite(1, HIGH); delay(200); digitalWrite(1, LOW);
  digitalWrite(2, HIGH); delay(200); digitalWrite(2, LOW);
  digitalWrite(21, HIGH); delay(200); digitalWrite(21, LOW);
  digitalWrite(16, HIGH); delay(200); digitalWrite(16, LOW);
  digitalWrite(17, HIGH); delay(200); digitalWrite(17, LOW);
  digitalWrite(16, HIGH); delay(200); digitalWrite(16, LOW);
  digitalWrite(21, HIGH); delay(200); digitalWrite(21, LOW);
  digitalWrite(2, HIGH); delay(200); digitalWrite(2, LOW);
  digitalWrite(1, HIGH); delay(200); digitalWrite(1, LOW);
  digitalWrite(0, HIGH); delay(200); digitalWrite(0, LOW);
}

// Pola 6: Random
void polaRandom() {
  int randomPin = random(0, 22);
  if (randomPin == 0 || randomPin == 1 || randomPin == 2 ||
      randomPin == 21 || randomPin == 16 || randomPin == 17) {
    digitalWrite(randomPin, HIGH);
    delay(300);
    digitalWrite(randomPin, LOW);
  }
}

// Pola 7: Pasangan berlawanan
void polaPasanganBerlawanan() {
  digitalWrite(0, HIGH); digitalWrite(17, HIGH); delay(400);
  digitalWrite(0, LOW); digitalWrite(17, LOW);
  digitalWrite(1, HIGH); digitalWrite(16, HIGH); delay(400);
  digitalWrite(1, LOW); digitalWrite(16, LOW);
  digitalWrite(2, HIGH); digitalWrite(21, HIGH); delay(400);
  digitalWrite(2, LOW); digitalWrite(21, LOW);
}

void loop() {
  // Panggil pola satu per satu
  polaDefault();
  polaBawahKeAtas();
  polaZigZag();
  polaSemuaMenyala();
  polaKiriKeKananDanBalik();
  polaKnightRider();
  polaRandom();
  polaPasanganBerlawanan();
}
