#include <RoboClaw.h>

uint32_t timeout = 10000;
RoboClaw roboclaw(&Serial1, timeout);

#define address 0x80
#define baudRate 38400

float Kp1 = 3.7123, Ki1 = 0.3608, Kd1 = 0.6457;
float Kp2 = 3.6853, Ki2 = 0.3578, Kd2 = 0.6397;
uint32_t qpps1 = 6000, qpps2 = 6000;

#define accel 400
#define speedcmd 1000

void setup(){
	Serial.begin(57600);
	roboclaw.begin(baudRate);

	char roboclaw_version;
	bool ok = roboclaw.ReadVersion(address, &roboclaw_version);
	if(ok){
		Serial.print("Roboclaw-Version : "); Serial.println(roboclaw_version);
    delay(500);
	} else{
		Serial.println("Not OK! Couldn't fetch version.");
	}

  bool validError;
  uint32_t error = roboclaw.ReadError(address, &validError);
  if(validError){
    Serial.print("Error State : "); Serial.println(error);
  } else{
    // RoboClaw Setup
    roboclaw.SetM1VelocityPID(address, Kp1, Ki1, Kd1, qpps1);
    roboclaw.SetM2VelocityPID(address, Kp2, Ki2, Kd2, qpps2);
  }
}

void resetEnc(){
  roboclaw.SetEncM1(address, 0);
  roboclaw.SetEncM2(address, 0);
}

void moveForward(){
  Serial.println("FWD");
  roboclaw.SpeedAccelM1(address, accel, speedcmd);
  roboclaw.SpeedAccelM2(address, accel, speedcmd);
}

void moveReverse(){
  Serial.println("REV");
  roboclaw.SpeedAccelM1(address, accel, -speedcmd);
  roboclaw.SpeedAccelM2(address, accel, -speedcmd);
}

void rotateCW(){
  Serial.println("RCW");
  roboclaw.SpeedAccelM1(address, accel, -speedcmd);
  roboclaw.SpeedAccelM2(address, accel, speedcmd);
}

void rotateACW(){
  Serial.println("RACW");
  roboclaw.SpeedAccelM1(address, accel, speedcmd);
  roboclaw.SpeedAccelM2(address, accel, -speedcmd);
}

void stop(){
  Serial.println("STOP");
  roboclaw.SpeedAccelM1(address, accel, 0);
  roboclaw.SpeedAccelM2(address, accel, 0);
}

void displayStatus(){
  Serial.println("------------------------------------------");
  bool validError;
  uint32_t error = roboclaw.ReadError(address, &validError);
  if(validError){
    Serial.print("Error State : "); Serial.println(error);
  } else{
    Serial.println("Error : None.");
  }
  
  uint16_t temperature;
  roboclaw.ReadTemp(address, temperature);
  Serial.print("Temperature : "); Serial.print(temperature-273); Serial.println(" *C");

  bool validReading;
  uint16_t voltage = roboclaw.ReadMainBatteryVoltage(address, &validReading);
  if(validReading){
    Serial.print("Main Battery Voltage : "); Serial.print(voltage/10); Serial.println(" V");
  }
  Serial.println("------------------------------------------");
}

void displayStatus_(){
  uint32_t t_in = millis();
  while(millis()-t_in < 5000){
  
    Serial.println("------------------------------------------");

    uint32_t encM1, encM2;
    bool gotEncData = roboclaw.ReadEncoders(address, encM1, encM2);
    if(gotEncData){
      Serial.print("EncM1 : "); Serial.print(encM1);
      Serial.print("\tEncM2 : "); Serial.println(encM1);
    }

    uint32_t speedM1, speedM2;
    bool gotSpeedData = roboclaw.ReadISpeeds(address, speedM1, speedM2);
    if(gotSpeedData){
      Serial.print("SpeedM1 : "); Serial.print(speedM1);
      Serial.print("\tSpeedM2 : "); Serial.println(speedM1);
    }

    int16_t currentM1, currentM2;
    bool gotCurrentsData = roboclaw.ReadCurrents(address, currentM1, currentM2);
    if(gotCurrentsData){
      Serial.print("CurrentM1 : "); Serial.print(currentM1);
      Serial.print("mA\tCurrentM2 : "); Serial.print(currentM1);
      Serial.println("mA");
    }
    delay(500);
    
  }
  Serial.println("------------------------------------------");
}

void loop(){

  delay(500);
  displayStatus();
  
  delay(500);
  resetEnc(); moveForward();  displayStatus_();  stop();
  delay(1000);
  resetEnc(); moveReverse();  displayStatus_();  stop();
  delay(1000);
  resetEnc(); rotateCW();     displayStatus_();  stop();
  delay(1000);
  resetEnc(); rotateACW();    displayStatus_();  stop();

  displayStatus();
  
  while(true){
    // wait till reset
  }
}
