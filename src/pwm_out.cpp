#include <ros/ros.h>
#include <std_msgs/Int32MultiArray.h>
#include <iostream>
#include <cmath>
#include <unistd.h>
#include "GPIO.h"

using namespace std;

int period = 100; //us
int duty_left = 100;
int duty_right = 100;
unsigned int leftPWM = P9_12;
unsigned int rightPWM = P9_18;

void DutyControl(const std_msgs::Int32MultiArray& array){
  duty_left = array.data[0];
  duty_right = array.data[1];
}

void pwm(){
  gpio_set_value(leftPWM, HIGH);
  usleep(duty_left);
  gpio_set_value(leftPWM, LOW);
  usleep(period-duty_left);
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "pwm_out");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("pwm", 10, DutyControl);

  gpio_export(leftPWM);
  gpio_set_dir(leftPWM, OUTPUT_PIN);
  gpio_set_value(leftPWM, HIGH);

  gpio_export(rightPWM);
  gpio_set_dir(rightPWM, OUTPUT_PIN);
  gpio_set_value(rightPWM, HIGH);

  while (ros::ok())
  {
    pwm();
    ros::spinOnce();
  }

  gpio_set_value(leftPWM, HIGH);
  gpio_set_value(rightPWM, HIGH);
  return 0;
}
