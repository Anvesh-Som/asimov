#!/usr/bin/env python
import rospy
from std_msgs.msg import String

def the_listener(the_talk):
	the_heard_talk = the_talk.data
	rospy.loginfo("Heard what : %s", the_heard_talk)

def the_controller(): # or the_looper/spinner
	rospy.init_node('my_node', anonymous=True)
	rospy.Subscriber("the_talker", String, the_listener)
	rospy.spin()

if __name__ == '__main__':
	the_controller()
