#include "emergency.h"

box::box()
{
    ros::NodeHandle nh;

    //publish
    state_pub = nh.advertise<std_msgs::String>("/emergency_state",10);
    //speed_pub = nh.advertise<ackermann_msgs::AckermannDriveStamped>("/ackermann_cmd",10);
    //subscibe
    output_sub = nh.subscribe("/ackermann_cmd_from_sc",10,&box::output_callback,this);
    box_sub = nh.subscribe("/darknet_ros/bounding_boxes",10,&box::box_callback,this);
}
void box::output_callback(const ackermann_msgs::AckermannDriveStamped& msg)
{
    output = msg;
}
void box::box_callback(const darknet_ros_msgs::BoundingBoxes& msg)
{
    bounding_box = msg.bounding_boxes[0];
    object_name = bounding_box.Class;
    width = bounding_box.xmax - bounding_box.xmin;
    height = bounding_box.ymax - bounding_box.ymin;
    ratio = width/height;
}

void box::pub_speed()
{
    if(object_name=="person" && ratio >= threshold)
    {
        cout<<"emergency"<<endl;
        state.data = "emergency";

	    speed.drive.acceleration = 0;
        speed.drive.jerk = 0;
        speed.drive.speed = 0;
        speed.drive.steering_angle = 0;
        speed.drive.steering_angle_velocity = 0;        

        //speed_pub.publish(speed);
    }
    else
    {
        cout<<"normal"<<endl;
        state.data = "normal";
    }
        
    state_pub.publish(state);
}

int main(int argc, char * argv[])
{
    ros::init(argc, argv, "emergency");
    ros::NodeHandle nh;
    ros::NodeHandle nh_priv("~");

    box b;

    ros::Rate  loop_rate(10);

    while(ros::ok())
    {
        b.pub_speed();
        loop_rate.sleep();
        ros::spinOnce();
    }

    ros::spin();
    return 0;
}
