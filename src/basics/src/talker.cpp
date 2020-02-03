#include <ros/ros.h>
#include <std_msgs/String.h>

int main(int argc, char **argv){
	
	ros::init(argc, argv, "talker");
	ros::NodeHandle my_node;
	ros::Publisher my_talker = my_node.advertise<std_msgs::String>("the_talker",1);
	ros::Rate loop_rate(2);

	while(ros::ok()){
		std_msgs::String the_talk;
		the_talk.data = "hi, hello, bro..!";
		my_talker.publish(the_talk);
		ROS_INFO("I just said hi, hello to my bro.");
		ros::spinOnce();
		loop_rate.sleep();
	}

	return 0;
}
