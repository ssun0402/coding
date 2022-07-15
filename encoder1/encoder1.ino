// 오른쪽 모터 엔코더 위치 제어

#define encoderPinA  2 // 인터럽트에 쓸 수 있는 디지털 핀
#define encoderPinB 3 // 인터럽트에 쓸 수 있는 디지털 핀

#define driverIn3 9 // IN3
#define driverIn4 10 // IN4
#define driverPwmR  11

const float ratio = 360. / 90. / 52.;

float encoderPosRight = 0;
float setha = 0;
float motorDeg = 0;
float error = 0;

float Kp = 13;
float control;

int in3, in4;
int pwm;

void doEncoderA() {
  encoderPosRight  += (digitalRead(encoderPinA) == digitalRead(encoderPinB)) ? 1 : -1;
}
void doEncoderB() {
  encoderPosRight  += (digitalRead(encoderPinA) == digitalRead(encoderPinB)) ? -1 : 1;
}

void interruptInit() {
  attachInterrupt(0, doEncoderA, CHANGE);   //uno pin 2
  attachInterrupt(1, doEncoderB, CHANGE);   //uno pin 3
}

void setha_serial();

void setup() {
  Serial.begin(115200);
  pinMode(driverPwmR, OUTPUT);
  pinMode(driverIn3, OUTPUT);
  pinMode(driverIn4, OUTPUT);
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  interruptInit();
  digitalWrite(driverIn3, LOW);
  digitalWrite(driverIn4, LOW);
  analogWrite(driverPwmR, 0);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("setha : ");
  Serial.print(setha);
  Serial.print(" / right : ");
  Serial.print(encoderPosRight);

  motorDeg = encoderPosRight * ratio;
  error = setha - motorDeg;
  control = Kp * error;
  pwm = min(abs(control), 255);


  Serial.print(" / motorDeg : ");
  Serial.print(motorDeg);
  Serial.print(" / error : ");
  Serial.print(error);
  Serial.print(" / control : ");
  Serial.println(control);

  setha_serial();

}

void setha_serial() {
  if (Serial.available() > 0) {
    setha = Serial.parseFloat();
  }
  if (setha < 0 && control < 0) {
    in3 = HIGH, in4 = LOW;

    if ((setha >= motorDeg) && (control >= 0)) {
      in3 = LOW, in4 = LOW;
      digitalWrite(driverIn3, in3);
      digitalWrite(driverIn4, in4);
      analogWrite(driverPwmR, 0);
    }
    digitalWrite(driverIn3, in3);
    digitalWrite(driverIn4, in4);
    analogWrite(driverPwmR, pwm);
  }
  else if (setha > 0 && control > 0) {
    in3 = LOW, in4 = HIGH;

    if ((setha <= motorDeg) && (control <= 0)) {
      in3 = LOW, in4 = LOW;
      digitalWrite(driverIn3, in3);
      digitalWrite(driverIn4, in4);
      analogWrite(driverPwmR, 0);
    }
    digitalWrite(driverIn3, LOW);
    digitalWrite(driverIn4, LOW);
    delay(20);
    digitalWrite(driverIn3, in3);
    digitalWrite(driverIn4, in4);
    analogWrite(driverPwmR, pwm);
  }
  else if (setha == 0) {
    in3 = LOW, in4 = LOW;
    digitalWrite(driverIn3, LOW);
    digitalWrite(driverIn4, LOW);
    delay(20);
    digitalWrite(driverIn3, in3);
    digitalWrite(driverIn4, in4);
    analogWrite(driverPwmR, 0);

    encoderPosRight = 0;
    setha = 0;
    motorDeg = 0;
    error = 0;

  }
}
