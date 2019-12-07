#!/usr/bin/env python
# license removed for brevity

import rospy
import Adafruit_BBIO.PWM as PWM
import Adafruit_BBIO.GPIO as GPIO
from std_msgs.msg import Int32MultiArray

leftPWM = "P9_21"
rightPWM = "P9_22"
leftDir = "P9_17"
rightDir = "P9_18"

def pwmControl(data):
  left = data.data[0]
  right = data.data[1]
  PWM.set_duty_cycle(leftPWM, left)
  PWM.set_duty_cycle(rightPWM, right)

def dirControl(data):
  left = data.data[0]
  right = data.data[1]
  if left == 0:
    GPIO.output(leftDir, GPIO.LOW)
  else:
    GPIO.output(leftDir, GPIO.HIGH)
  if right == 0:
    GPIO.output(rightDir, GPIO.LOW)
  else:
    GPIO.output(rightDir, GPIO.HIGH)

def listener():
  rospy.init_node('M2')
  rospy.Subscriber("duty_cycle_M2", Int32MultiArray, pwmControl)
  rospy.Subscriber("dir_M2", Int32MultiArray, dirControl)
  rospy.spin()

if __name__ == '__main__':
  GPIO.setup(leftDir, GPIO.OUT)
  GPIO.setup(rightDir, GPIO.OUT)
  GPIO.output(leftDir, GPIO.LOW)
  GPIO.output(rightDir, GPIO.HIGH)
  PWM.start(leftPIN, 100, 15000)
  PWM.start(rightPIN, 100, 15000)
  listener()
