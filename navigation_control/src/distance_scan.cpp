#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "std_msgs/Float32.h"


const unsigned int num_readings = 5;
double ranges[num_readings];
int count;

void num_cb(std_msgs::Float32 msg) {
    ranges[count] = msg.data;
    ROS_INFO_STREAM("Abstand: " << msg.data);
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "sensor_publisher");
    ros::NodeHandle nh;

    ros::Publisher sens_pub = nh.advertise<sensor_msgs::LaserScan>("sensor_data", 100);
    ros::Subscriber distance_sub = nh.subscribe("nums", 10, num_cb);

    
    double sens_frequency = 50.0;
    double intensities[num_readings];


    ros::Rate rate(10.0);

    while (nh.ok()) {

        ros::Time scan_time = ros::Time::now();

        sensor_msgs::LaserScan scan;
        scan.header.stamp = scan_time;
        scan.header.frame_id = "base_laser";
        scan.angle_min = 0;
        scan.angle_max = 0;
        scan.angle_increment = 0;
        scan.time_increment = 0;
        scan.range_min = 0.0;
        scan.range_max = 100.0;

        scan.ranges.resize(num_readings);
        scan.intensities.resize(num_readings);

        for (unsigned int i = 0; i < num_readings; ++i) {
            scan.ranges[i] = ranges[i];
            scan.intensities[i] = 100;
        }

        sens_pub.publish(scan);
        ++count;
        if (count == num_readings - 1) {
            count = 0;
        }
        ros::spinOnce();
        rate.sleep();


    }



}