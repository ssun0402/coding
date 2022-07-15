// 오른쪽 모터 엔코더 위치 제어

#define encoderPinA  2
#define encoderPinB 3

#define driverIn3 9
#define driverIn4 10
#define driverPwmR  11

const float ratio = 360. / 90. / 68.;

float encoderPosRight = 0;
float setha = 0;
float motorDeg = 0;
float error = 0;

int in3, in4;

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
  Serial.begin(9600);
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
  error = motorDeg - setha;

  Serial.print(" / motorDeg : ");
  Serial.print(motorDeg);
  Serial.print(" / error : ");
  Serial.println(error);

  setha_serial();

}

void setha_serial() {
  if (Serial.available() > 0) {
    setha = Serial.parseFloat();
  }
  if (setha < 0) {
    in3 = HIGH, in4 = LOW;

    if (setha >=  motorDeg) {
      in3 = LOW;
    }
    digitalWrite(driverIn3, LOW);
    digitalWrite(driverIn4, LOW);
    delay(20);
    digitalWrite(driverIn3, in3);
    digitalWrite(driverIn4, in4);
    analogWrite(driverPwmR, 75);
  }
  else if (setha > 0) {
    in3 = LOW, in4 = HIGH;

    if (setha <= motorDeg) {
      in4 = LOW;
    }

    digitalWrite(driverIn3, LOW);
    digitalWrite(driverIn4, LOW);
    delay(20);
    digitalWrite(driverIn3, in3);
    digitalWrite(driverIn4, in4);
    analogWrite(driverPwmR, 75);
  }
  else if (setha == 0) {
    in3 = LOW, in4 = LOW;
    digitalWrite(driverIn3, LOW);
    digitalWrite(driverIn4, LOW);
    delay(20);
    digitalWrite(driverIn3, in3);
    digitalWrite(driverIn4, in4);
    analogWrite(driverPwmR, 75);
    encoderPosRight = 0;
    setha = 0;
    motorDeg = 0;
  }
}
