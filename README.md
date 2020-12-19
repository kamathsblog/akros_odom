# akros_odom

### Description
ROS node for odometry of the AKROS Ackermann platform. This node provides the following functionalities
* odom_ackermann
  * Reads the linear speed of the car using a hall effect sensor and a magnetic ring on the drive shaft. The hall effect sensor is connected to an Arduino and the computed linear speed is published via rosserial. This node subscribes to the linear speed
  * Calculates linear and angular velocities of the car using the subscribed linear speed and the subscribed steering angle from the drive node. Publishes a ROS odom message
  * TO DO: Also fuse orientation from the MPU6050 IMU
* odom_differential (TO DO)
  * TODO: Computer linear/angular velocities using the MPU6050 IMU
This node require rosserial for the Ackermann platform. If the IMU is being used, RTIMULib and the corresponding ROS packages need to be installed

### Configuration
There is no configuration file for this node

### Subscribers
The node subscribes to the `/odom_node/odom_linear` topic,  which is published over rosserial from the Arduino

### Publishers
The node publishes to the `/odom_node/odom` topic, of type Odom.

### Launch
No launch file is provided, yet.
