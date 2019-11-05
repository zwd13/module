#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Int32MultiArray.h>
#include <iostream>
#include <cmath>

using namespace std;

float pi = 3.1415;
float L = 0.171; //distance between wheels
int LeftDir = 0;
int rightDir = 0;

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
    cout<<"Left Forward:"<<left_wheel_data;
  }
  else
  {
    cout<<"Left Back:"<<left_wheel_data;
  }
  if(right_wheel_data >= 0)
  {
    cout<<"Right Forward:"<<right_wheel_data;
  }
  else
  {
    cout<<"Right Back:"<<right_wheel_data;
  }
  cout<<endl;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "motor");
  ros::NodeHandle n;
  Motor startMotor(n);
  ros::spin();
  return 0;
}
