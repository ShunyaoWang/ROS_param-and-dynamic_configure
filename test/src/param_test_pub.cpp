#include "ros/ros.h"
#include "std_msgs/String.h"
#include "test/testConfig.h"
#include "dynamic_reconfigure/server.h"
#include <sstream>

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
std::string param1;
bool param2;
double param3;

void drCallback(test::testConfig &config, uint32_t level){
	param1 = config.str_param;
	param2 = config.bool_param;
	param3 = config.double_param;
}

int main(int argc, char *argv[])
{

	ros::init(argc, argv, "param_test");

	/**
	 * NodeHandle is the main access point to communications with the ROS system.
	 * The first NodeHandle constructed will fully initialize this node, and the last
	 * NodeHandle destructed will close down the node.
	 */
	ros::NodeHandle n;
	ros::NodeHandle nh("~");

	dynamic_reconfigure::Server<test::testConfig> server;
	dynamic_reconfigure::Server<test::testConfig>::CallbackType f;
	f = boost::bind(&drCallback, _1, _2);
	server.setCallback(f);

	ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
	// std::string param1;
	// bool param2;
	// double param3;
	nh.param("str_param",param1,std::string("default"));
	nh.param("bool_param",param2,false);
	nh.param("double_param",param3,0.0);
	ros::Rate loop_rate(10);

	/**
	 * A count of how many messages we have sent. This is used to create
	 * a unique string for each message.
	 */
	int count = 0;
	while (ros::ok())
	{
		/**
		 * This is a message object. You stuff it with data, and then publish it.
		 */
		// nh.param("param1",param1,std::string("default"));
		// nh.param("param2",param2,false);
		// nh.param("param3",param3,0.0);
		std_msgs::String msg;

		std::stringstream ss;
		ss << "hello world " << count;
		msg.data = ss.str();

		ROS_INFO("%s", msg.data.c_str());
		printf("param1 is %s\n",param1.c_str());
		printf("param2 is %d\n",param2);
		printf("param3 is %f\n",param3);
		/**
		 * The publish() function is how you send messages. The parameter
		 * is the message object. The type of this object must agree with the type
		 * given as a template parameter to the advertise<>() call, as was done
		 * in the constructor above.
		 */
		chatter_pub.publish(msg);

		ros::spinOnce();

		loop_rate.sleep();
		++count;
	}

	return 0;
}