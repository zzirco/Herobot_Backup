#include<ros/ros.h>
#include<stdio.h>
#include<iostream>
#include<opencv2/opencv.hpp>
#include<math.h>
#include<cmath>
#include<vector>
#include<queue>
#include<tf/tf.h>
#include<string>
#include<string.h>

#include<nav_msgs/Path.h>
#include<nav_msgs/Odometry.h>
#include<nav_msgs/OccupancyGrid.h>

#include<geometry_msgs/PointStamped.h>
#include<geometry_msgs/PoseWithCovarianceStamped.h>

#include<visualization_msgs/MarkerArray.h>

#include<ackermann_msgs/AckermannDriveStamped.h>
#include<ackermann_msgs/AckermannDrive.h>

#include<sensor_msgs/PointCloud.h>

#include<std_msgs/Int32.h>
#include<std_msgs/String.h>

using namespace std;

typedef struct Node{
    float x;
    float y;
    float w;
    float z;
}Node;

class point {
    //point
    Node node[10];
public:
    point();
    float xp1, xp2, yp1, yp2;
    float dist, codist, distance, codistance;
    bool state = false;
    bool first_state = false;
    int min_index;
    string emer_state;

    std_msgs::String pose;
    geometry_msgs::PoseStamped goal;
    geometry_msgs::PoseStamped next_goal;
    geometry_msgs::PoseStamped dum_goal;
    geometry_msgs::PoseWithCovarianceStamped amcl;

    //Publish
    ros::Publisher goal_pub;
    ros::Publisher index_pub;
    //Subscriber
    ros::Subscriber goal_sub;
    ros::Subscriber current_sub;
    ros::Subscriber state_sub;

    //Callback
    void goal_callback(const geometry_msgs::PoseStamped& msg);
    void amcl_callback(const geometry_msgs::PoseWithCovarianceStamped& msg);
    void state_callback(const std_msgs::String& msg);
    //Function
    float calcDistance();
    Node nextGoal();
    void pub_goal();
    void isPub();
    string get_state();
};

