#include <ros.h>
#include <RoboClaw.h>

#include <std_msgs/Int16.h>
#include <std_msgs/Int32.h>
#include <std_msgs/UInt16.h>
#include <std_msgs/UInt32.h>
#include <std_msgs/UInt8MultiArray.h>

uint32_t timeout = 10000;
RoboClaw roboclaw(&Serial1, timeout);

#define address 0x80
#define baudRate 38400

float Kp1 = 3.7123, Ki1 = 0.3608, Kd1 = 0.6457;
float Kp2 = 3.7123, Ki2 = 0.3608, Kd2 = 0.6457;
uint32_t qpps1 = 6000, qpps2 = 6000;
uint32_t accel;

#define baudRate_ROSserial 38400

ros::NodeHandle nh;

#define accel_rostopic "/drive/accel"
#define speedcmdM1_rostopic "/right_wheel/cmd_vel"
#define speedcmdM2_rostopic "/left_wheel/cmd_vel"

#define encM1_rostopic "/right_wheel/enc"
#define encM2_rostopic "/left_wheel/enc"
#define speedM1_rostopic "/right_wheel/vel"
#define speedM2_rostopic "/left_wheel/vel"
#define currentM1_rostopic "/right_motor/current"
#define currentM2_rostopic "/left_motor/current"
#define error_rostopic "/roboclaw/error_status"
#define voltage_rostopic "/roboclaw/measured_voltage"
#define temperature_rostopic "/roboclaw/measured_temperature"
#define encSpeedStatus_rostopic "roboclaw/enc_vel/status_flags"


void cb_accel(const std_msgs::UInt32& msg){
	accel = msg.data;
}

void cb_speedM1(const std_msgs::Int32& msg){
	roboclaw.SpeedAccelM1(address, accel, msg.data);
}

void cb_speedM2(const std_msgs::Int32& msg){
	roboclaw.SpeedAccelM2(address, accel, msg.data);
}

ros::Subscriber<std_msgs::UInt32> _accel(accel_rostopic, &cb_accel);
ros::Subscriber<std_msgs::Int32> _speedcmdM1(speedcmdM1_rostopic, &cb_speedM1);
ros::Subscriber<std_msgs::Int32> _speedcmdM2(speedcmdM2_rostopic, &cb_speedM2);

std_msgs::UInt32 encM1;
std_msgs::UInt32 encM2;

ros::Publisher _encM1(encM1_rostopic, &encM1);
ros::Publisher _encM2(encM2_rostopic, &encM2);

std_msgs::UInt32 speedM1;
std_msgs::UInt32 speedM2;

ros::Publisher _speedM1(speedM1_rostopic, &speedM1);
ros::Publisher _speedM2(speedM2_rostopic, &speedM2);

std_msgs::UInt8MultiArray encSpeedStatus;
ros::Publisher _encSpeedStatus(encSpeedStatus_rostopic, &encSpeedStatus);

std_msgs::Int16 currentM1;
std_msgs::Int16 currentM2;

ros::Publisher _currentM1(currentM1_rostopic, &currentM1);
ros::Publisher _currentM2(currentM2_rostopic, &currentM2);

std_msgs::UInt32 error;
std_msgs::UInt16 voltage;
std_msgs::UInt16 temperature;

ros::Publisher _error(error_rostopic, &error);
ros::Publisher _voltage(voltage_rostopic, &voltage);
ros::Publisher _temperature(temperature_rostopic, &temperature);

void setup(){
	roboclaw.begin(baudRate);
	roboclaw.SetM1VelocityPID(address, Kp1, Ki1, Kd1, qpps1);
	roboclaw.SetM2VelocityPID(address, Kp2, Ki2, Kd2, qpps2);
	roboclaw.SetEncM1(address, 0); roboclaw.SetEncM2(address, 0);

	nh.getHardware()->setBaud(baudRate_ROSserial);

	nh.initNode();

	encSpeedStatus.layout.dim = (std_msgs::MultiArrayDimension *)
	malloc(sizeof(std_msgs::MultiArrayDimension)*2);
	encSpeedStatus.layout.dim[0].label = "status_flags";
	encSpeedStatus.layout.dim[0].size = 4;
	encSpeedStatus.layout.dim[0].stride = 1;
	encSpeedStatus.layout.data_offset = 0;
	encSpeedStatus.data = malloc(sizeof(int)*8);
	encSpeedStatus.data_length = 4;
  
	nh.advertise(_error);
	nh.subscribe(_accel);
	nh.subscribe(_speedcmdM1);
	nh.subscribe(_speedcmdM2);
	nh.advertise(_encM1);
	nh.advertise(_encM2);
	nh.advertise(_speedM1);
	nh.advertise(_speedM2);
	nh.advertise(_encSpeedStatus);
	nh.advertise(_currentM1);
	nh.advertise(_currentM2);
	nh.advertise(_voltage);
	nh.advertise(_temperature);

	while(!nh.connected()){
		nh.spinOnce();
	}
}

void loop(){

	bool validError;
	uint32_t err = roboclaw.ReadError(address, &validError);
	if(validError){
		error.data = err;
		_error.publish(&error);
	}

	nh.spinOnce();

	uint8_t statusEncM1; bool validEncM1;
	uint32_t encM1_ = roboclaw.ReadEncM1(address, &statusEncM1, &validEncM1);
	if(validEncM1){
		encM1.data = encM1_;
		_encM1.publish(&encM1);
	}

	uint8_t statusEncM2; bool validEncM2;
	uint32_t encM2_ = roboclaw.ReadEncM2(address, &statusEncM2, &validEncM2);
	if(validEncM2){
		encM2.data = encM2_;
		_encM2.publish(&encM2);
	}

	uint8_t statusSpeedM1; bool validSpeedM1;
	uint32_t speedM1_ = roboclaw.ReadSpeedM1(address, &statusSpeedM1, &validSpeedM1);
	if(validSpeedM1){
		speedM1.data = speedM1_;
		_speedM1.publish(&speedM1);
	}

	uint8_t statusSpeedM2; bool validSpeedM2;
	uint32_t speedM2_ = roboclaw.ReadSpeedM2(address, &statusSpeedM2, &validSpeedM2);
	if(validSpeedM2){
		speedM2.data = speedM2_;
		_speedM2.publish(&speedM2);
	}

	encSpeedStatus.data[0] = statusEncM1; encSpeedStatus.data[1] = statusEncM2;
	encSpeedStatus.data[2] = statusSpeedM1; encSpeedStatus.data[3] = statusSpeedM2;
	_encSpeedStatus.publish(&encSpeedStatus);

	int16_t currentM1_, currentM2_;
	roboclaw.ReadCurrents(address, currentM1_, currentM2_);
	currentM1.data = currentM1_; currentM2.data = currentM2_;
	_currentM1.publish(&currentM1); _currentM2.publish(&currentM2);

	uint16_t temp_;
	roboclaw.ReadTemp(address, temp_);
	temperature.data = temp_;
	_temperature.publish(&temperature);

	bool validVoltage;
	uint16_t voltage_ = roboclaw.ReadMainBatteryVoltage(address, &validVoltage);
	if(validVoltage){
		voltage.data = voltage_;
		_voltage.publish(&voltage);
	}
}
