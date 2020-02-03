#!/usr/bin/env python
import rospy
from std_msgs.msg import String

def talker():
	my_talker = rospy.Publisher('the_talker', String, queue_size=1)
	rospy.init_node('my_node', anonymous=True)
	rate = rospy.Rate(1)
	while not rospy.is_shutdown():
		the_talk = "hi, hello, bro..!"
		my_talker.publish(the_talk)
		rospy.loginfo("I just said hi, hello to my bro.")
		rate.sleep()

if __name__ == '__main__':
	try:
		talker()
	except rospy.ROSInteruptException:
		pass
