#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Int32MultiArray.h>
#include <iostream>
#include <cmath>
#include "GPIO.h"

using namespace std;

float pi = 3.1415;
float L = 0.171; //distance between wheels
int leftDIR = P9_17;
int rightDIR = P9_18;

class Motor{
  public:
    Motor(ros::NodeHandle& n);
  private:
    void MotorControl(const geometry_msgs::Twist& twist);
    ros::NodeHandle n;
    ros::Publisher pub;
    ros::Subscriber sub;
    std_msgs::Int32MultiArray period;
};

Motor::Motor(ros::NodeHandle& n)
{
  pub = n.advertise<std_msgs::Int32MultiArray>("pwm", 10);
  sub = n.subscribe("cmd_vel", 10, &Motor::MotorControl, this);
  period.data.push_back(100);
  period.data.push_back(100);
}


void Motor::MotorControl(const geometry_msgs::Twist& twist)
{
  float left_wheel_data = twist.linear.x - twist.angular.z;
  float right_wheel_data = twist.linear.x + twist.angular.z;
  
  int leftPeriod = min(100, (int)abs(left_wheel_data));
  int rightPeriod = min(100, (int)abs(right_wheel_data));
  
  period.data.clear();
  period.data.push_back(100-leftPeriod);
  period.data.push_back(100-rightPeriod);
  pub.publish(period);

  if(left_wheel_data >= 0)
  {
    gpio_set_value(leftDIR, LOW);
  }
  else
  {
    gpio_set_value(leftDIR, HIGH);
  }
  if(right_wheel_data >= 0)
  {
    gpio_set_value(rightDIR, HIGH);
  }
  else
  {
    gpio_set_value(rightDIR, LOW);
  }
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "motor");

  gpio_export(leftDIR);
  gpio_set_dir(leftDIR, OUTPUT_PIN);
  gpio_set_value(leftDIR, LOW);

  gpio_export(rightDIR);
  gpio_set_dir(rightDIR, OUTPUT_PIN);
  gpio_set_value(rightDIR, HIGH);

  ros::NodeHandle n;
  Motor startMotor(n);
  ros::spin();

  gpio_set_value(leftDIR, LOW);
  gpio_set_value(rightDIR, HIGH);
  return 0;
}
