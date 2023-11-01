// c++
#include <pthread.h>
#include <chrono>
#include <cmath>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <string.h>

// ROS
#include <actionlib/server/simple_action_server.h>
#include <geometry_msgs/Point.h>
#include <image_transport/image_transport.h>
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <std_msgs/Header.h>
#include <std_msgs/String.h>
#include <ackermann_msgs/AckermannDriveStamped.h>
#include <ackermann_msgs/AckermannDrive.h>

// OpenCv
#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

// darknet_ros_msgs
#include <darknet_ros_msgs/BoundingBox.h>
#include <darknet_ros_msgs/BoundingBoxes.h>
#include <darknet_ros_msgs/CheckForObjectsAction.h>
#include <darknet_ros_msgs/ObjectCount.h>

// Darknet.
#ifdef GPU
#include "cublas_v2.h"
#include "cuda_runtime.h"
#include "curand.h"
#endif

using namespace std;

class box {
    
public:
    box();
    float width;
    float height;
    float ratio;
    float threshold = 1.0;
    string object_name;

    darknet_ros_msgs::BoundingBox bounding_box;
    std_msgs::String state;
    ackermann_msgs::AckermannDriveStamped speed;
    ackermann_msgs::AckermannDriveStamped output;

    //Publish
    ros::Publisher state_pub;
    ros::Publisher speed_pub;
    //Subscriber
    ros::Subscriber output_sub;
    ros::Subscriber box_sub;

    //Callback
    void output_callback(const ackermann_msgs::AckermannDriveStamped& msg);
    void box_callback(const darknet_ros_msgs::BoundingBoxes& msg);
    //Function
    void pub_speed();
};
