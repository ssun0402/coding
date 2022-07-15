// 8bit에서 비트 연산자와 논리연산자를 써서 출력이 항상 16진수로 FF가 되는 문제

void setup() {
  Serial.begin(9600);

  byte b = 0xAF;
  byte m;

  Serial.print("입력 : ");
  print_in_8(b);
  Serial.println();

  Serial.print("출력 : ");
  m = b | 0xFF;
  print_FF(m);
  

}

void print_in_8(byte b) {
  for (int i = 7; i >= 0; i--) {
    boolean bb = (b >> i) & 0x01;
    Serial.print(bb);
  }
}

void print_FF(byte m) {
  for (int i = 7; i >= 0; i--) {
    boolean mm = (m >> i) & 0x01;
    Serial.print(mm);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
