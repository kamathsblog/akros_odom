/*
==========
ROS node for Odometry of the ROSCar (Jetracer2)
This code implements the following functionality:
  > Subscribe to linear speed from Arduino over ROSSerial
  > Measure current steering angle using servo feedback
  > Compute current state of the robot using linear speed and steering angle
  > Publish odometry
by Aditya Kamath
adityakamath.github.io
github.com/adityakamath
==========
*/

#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_broadcaster.h>

class OdomAckermann
{
public:
  OdomAckermann();
    
private:
  ros::NodeHandle nh; 
    
  ros::Publisher odom_pub;
  ros::Subscriber arduino_sub;
  tf::TransformBroadcaster odom_broadcaster;
    
  ros::Time current_time;
   
  double x, y, th, vx, vy, vth;

  void arduinoCallback(const std_msgs::Float64::ConstPtr& arduino_msg)
  {
    ROS_DEBUG("[ODM] I heard: [%f]", arduino_msg->data);
    current_time = ros::Time::now();
      
    //calculate odom - x,y,th,vx,vy,vth
      
    //since all odometry is 6DOF we need a quaternion created from yaw
    geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th); //yaw to be calculated

    //publish the transform over tf
    geometry_msgs::TransformStamped odom_tf;
    odom_tf.header.stamp    = current_time;
    odom_tf.header.frame_id = "odom";
    odom_tf.child_frame_id  = "base_link";

    odom_tf.transform.translation.x = x; //to be calculated
    odom_tf.transform.translation.y = y; //to be calculated
    odom_tf.transform.translation.z = 0.0; //unobserved by this node
    odom_tf.transform.rotation      = odom_quat;

    //send the transform
    odom_broadcaster.sendTransform(odom_tf);

    //publish the odometry message over ROS
    nav_msgs::Odometry odom;
    odom.header.stamp    = current_time;
    odom.header.frame_id = "odom";

    //set the position
    odom.pose.pose.position.x  = x; //to be calculated
    odom.pose.pose.position.y  = y; //to be calculated
    odom.pose.pose.position.z  = 0.0; //unobserved by this node
    odom.pose.pose.orientation = odom_quat;

    //set the velocity
    odom.child_frame_id        = "base_link";
    odom.twist.twist.linear.x  = arduino_msg->data; //message from rosserial
    odom.twist.twist.linear.y  = vy; //to be calculated 
    odom.twist.twist.angular.z = vth; //to be calculated
    
    //publish the message
    odom_pub.publish(odom);  
  }
};

OdomAckermann::OdomAckermann():
    x(0.0),
    y(0.0),
    th(0.0),
    vx(0.0),
    vy(0.0),
    vth(0.0)
{
    odom_pub    = nh.advertise<nav_msgs::Odometry>("/odom_node/odom", 1);
    arduino_sub = nh.subscribe("/odom_node/odom_linear", 1, &OdomAckermann::arduinoCallback, this);
      
    current_time = ros::Time::now();
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "odom_ackermann");
  OdomAckermann odom_robot;
  ROS_INFO("[ODM] Odometry driver initialized");
  ros::spin();
  return 0;
}
