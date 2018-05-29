#include<string>
#include<ros/ros.h>
#include<boost/asio.hpp>
#include<boost/bind.hpp>
#include<math.h>
#include"std_msgs/String.h"
#include<iostream>
#include<fstream>

//#include<sensor_msgs/JointState.h>
//#include<tf/transform_broadcaster.h>
//#include<nav_msg/Odometry.h>
using namespace std;
using namespace boost::asio;
unsigned char txData[3]={0x11,0xaa,0x55};
char rxData[4];
float stm32_fdata;
int   stm32_data;
int main(int argc,char ** argv)
{
	ros::init(argc,argv,"serial_port");
	ros::NodeHandle n;
	ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter",1000);
	//	std::ofstream out("test.txt");

	ros::Rate loop_rate(10);
	io_service iosev;
	serial_port sp(iosev, "/dev/ttyUSB0");
	sp.set_option(serial_port::baud_rate(115200));
	sp.set_option(serial_port::flow_control());
	sp.set_option(serial_port::parity());
	sp.set_option(serial_port::stop_bits());
	sp.set_option(serial_port::character_size(8));

	//write( sp,buffer("123\n",4) );
	while( n.ok()){
		//write(sp,buffer(txData,3));
	
		read(sp,buffer(rxData));
		ROS_INFO("rxData = %c%c%c%c \n",rxData[0],rxData[1],rxData[2],rxData[3]);
		stm32_data = (rxData[0]<<24 | rxData[1] <<16 | rxData[2]<<8 | rxData[3]);
		stm32_fdata = *((float *) & stm32_data);
		
		std_msgs::String msg;
		std::stringstream ss;

		ROS_INFO(" stm32_fdata = %f\n",stm32_fdata);
		loop_rate.sleep();
	}
	iosev.run();
	return 0;
}
