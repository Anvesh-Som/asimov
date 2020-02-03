#include <ros/ros.h>
#include <std_msgs/String.h>

void the_listener(const std_msgs::String::ConstPtr& the_thing_heard){

	ROS_INFO("Heard what : [%s]", the_thing_heard->data.c_str());
}

int main(int argc, char **argv){

	ros::init(argc, argv, "listener");
	ros::NodeHandle my_node;
	ros::Subscriber my_listener = my_node.subscribe("the_talker", 1, the_listener);
	ros::spin();
	return 0;
}
