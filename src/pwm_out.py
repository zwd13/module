#!/usr/bin/env python
# license removed for brevity

import rospy
import Adafruit_BBIO.PWM as PWM
from std_msgs.msg import Int32MultiArray

leftPIN = "P9_21"
rightPIN = "P9_22"

def callback(data):
  left = data.data[0]
  right = data.data[1]
  PWM.set_duty_cycle(leftPIN, left)
  PWM.set_duty_cycle(rightPIN, right)

def listener():
  rospy.init_node('pwm_out')
  rospy.Subscriber("pwm", Int32MultiArray, callback)
  rospy.spin()

if __name__ == '__main__':
  PWM.start(leftPIN, 100, 15000)
  PWM.start(rightPIN, 100, 15000)
  listener()
