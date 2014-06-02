/*
 * Commander Node
 * Intelligent Ground Vehicle Challenge 2014
 * Oakland University - Rochester, Michigan
 * 
 * UBC Snowbots
 *
 */

#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Vector3.h>
#include <std_msgs/Float64.h>
#include <sensor_msgs/LaserScan.h>

#include "sb_msgs/CarCommand.h"
#include <sb_msgs/LidarNav.h>

// Global Constants
static const int SECOND = 1000000; //1 million us

// steering = 1 is left in stage so steering = -1 is right
geometry_msgs::Twist twistConvertor(sb_msgs::CarCommand car_msg);

using namespace ros;
using namespace std;

// ROS-related Constants
static const string NODE_NAME          			= "commander";
static const string CAR_PUBLISH_TOPIC				= "cmd_vel";
static const string LIDAR_SUBSCRIBE_TOPIC  	= "lidar_nav";  // lidar_nav node suggestion
static const string VISION_SUBSCRIBE_TOPIC	= "vision_nav"; // vision_nav node suggestion
static const string GPS_SUBSCRIBE_TOPIC		= "gps_nav"; // jen
static const int LOOP_FREQ = 30; // Hz

double my_data[550];
int my_count = 0;
double throttle = 0;
double steering = 0; // 1 is right, 30 shown on arduino driver (real world), -1 is left

bool stopSignFlag = false;
bool redLightFlag = false;

void Stop()
{
	throttle = 0;
	steering = 0;		
}

void vision_callback(const std_msgs::Float64ConstPtr& float64Msg)
{
  double confidence;
  confidence = float64Msg->data;
  cout << "the confidence is: "<<confidence << endl;
}


//call back for lidar directions
void ir_state_callback(const sensor_msgs::LaserScanConstPtr& msg_ptr)
{
   
}

// jen
void gps_callback(const geometry_msgs::Twist::ConstPtr& msg) 
{
	ROS_INFO("\nGot back: [Lin.x = %f, Lin.y = %f, Ang.z = %f]", msg->linear.x, msg->linear.y, msg->angular.z);
}


sb_msgs::CarCommand driver()
{
    sb_msgs::CarCommand car_msg;

    //uncomment the two lines below and populate car_msg with data 
    car_msg.throttle = throttle; //Throtle;
    car_msg.steering = steering;//Steering;
     
    // geometry_msgs::Twist twist_msg = twistConvertor(car_msg);

    return car_msg;
}


int main( int argc, char** argv )
{
 
    //ros initialization
    ros::init(argc, argv, NODE_NAME);
    ros::NodeHandle n;
    
    //subscribes to IR topic to receive data from arduino
    //lidar_class my_lidar_class;

    ros::Subscriber IR_state = n.subscribe(LIDAR_SUBSCRIBE_TOPIC, 20, ir_state_callback);
  //  ros::Subscriber Lidar_instructions = n.subscribe(LIDAR_SUBSCRIBE_TOPIC, 3, &lidar_class::callback,&my_lidar_class);
    ros::Subscriber Vision = n.subscribe(VISION_SUBSCRIBE_TOPIC, 20, vision_callback);
    ros::Subscriber GPS = n.subscribe(GPS_SUBSCRIBE_TOPIC, 20, gps_callback); // jen

    ros::Publisher car_pub = n.advertise<geometry_msgs::Twist>(CAR_PUBLISH_TOPIC, 1);
	
		

    //controls how fast it goes
    ros::Rate loop_rate(LOOP_FREQ);

    ROS_INFO("ready to go");
    usleep(3*SECOND);
       
    ROS_INFO("going");   
    
    while(ros::ok())
    {
	
        //driver is navigation function
    	
        //publshing data to robot
       // ROS_INFO("sending throttle=%f, steering=%f", twist_msg.linear.x, twist_msg.angular.z);

	//sb_msgs::CarCommand car_msg = my_lidar_class.get_car_msg();
	sb_msgs::CarCommand car_msg = driver();
        geometry_msgs::Twist twist_msg = twistConvertor(car_msg);
        car_pub.publish(twist_msg);
        
        
        //checking callbacks and sleeping to match loop rate
        ros::spinOnce();
        loop_rate.sleep();	
    }
    ROS_INFO("shutting down node");
  
    return 0;
}

geometry_msgs::Twist twistConvertor(sb_msgs::CarCommand car_msg)
{
      //Creating instances of Twist and Vector3 classes	
      geometry_msgs::Twist twistReturn;
      geometry_msgs::Vector3 linearVel;
      geometry_msgs::Vector3 angularVel;

      //Since the robot can move in only one direction (x-axis),
      //we set the other two to zero	
      linearVel.x = car_msg.throttle;
      linearVel.y = 0;
      linearVel.z = 0;

      //Since the robot can turn around z-axis only,
      //we set the other two to zero
      angularVel.x = 0;
      angularVel.y = 0;
      angularVel.z = car_msg.steering;

      twistReturn.linear = linearVel;
      twistReturn.angular = angularVel;

      return twistReturn;
}
