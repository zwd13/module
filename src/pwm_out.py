#!/usr/bin/env python
# license removed for brevity

import rospy
import Adafruit_BBIO.PWM as PWM
from std_msgs.msg import Float32

leftPIN = "P8_19"
rightPIN = "P9_16"

def callback_left(data):
  left = data.data
  PWM.set_duty_cycle(leftPIN, left)

def callback_right(data):
  right = data.data
  PWM.set_duty_cycle(rightPIN, right)

def listener():
  rospy.init_node('pwm_out')
  rospy.Subscriber("pwm_left", Float32, callback_left)
  rospy.Subscriber("pwm_right", Float32, callback_right)
  rospy.spin()

if __name__ == '__main__':
  PWM.start(leftPIN, 100, 15000)
  PWM.start(rightPIN, 100, 15000)
  listener()
