#include "ros/ros.h"
#include <signal.h>
#include <termios.h>
#include "stdio.h"

#include<ackermann_msgs/AckermannDriveStamped.h>
#include<ackermann_msgs/AckermannDrive.h>

#define KEYCODE_R 0x64 // D
#define KEYCODE_L 0x61 // A
#define KEYCODE_U 0x77 // W
#define KEYCODE_D 0x73 // S
#define KEYCODE_Q 0x71 // Q


int kfd=0;
struct termios cooked, raw;
ackermann_msgs::AckermannDriveStamped cmd_vel;
ackermann_msgs::AckermannDriveStamped cmd_vel_;


int main(int argc, char** argv)
{
    ros::init(argc, argv, "keyboard_teleop");
    ros::NodeHandle nh;
    ros::Publisher cmd_pub;
    ros::Publisher cmd_pub_;
    cmd_pub = nh.advertise<ackermann_msgs::AckermannDriveStamped>("/ackermann_cmd_keyboard",10);
    cmd_pub_ = nh.advertise<ackermann_msgs::AckermannDriveStamped>("/drive",10);

    double linear=0;
    double angular=0;
    double l_scale=0.3;
    double a_scale=0.2;
    double final_linear;
    double final_angular;

    nh.param("scale_angular", a_scale, a_scale);
    nh.param("scale_linear", l_scale, l_scale);

    char c;
    bool input=false;

    tcgetattr(kfd, &cooked);
    memcpy(&raw, &cooked, sizeof(struct termios));
    raw.c_lflag &=~ (ICANON | ECHO);
    raw.c_cc[VEOL]=1;
    raw.c_cc[VEOF]=2;
    tcsetattr(kfd, TCSANOW, &raw);

    std::cout << "reading from key board" << std::endl;
    std::cout << "----------------------------" << std::endl;

    for(;;){
        if(read(kfd, &c,1) < 0){
            std::cout << "read : " << std::endl;
        }

//        linear=angular=0;
        std::cout << "value : " << std::endl;

        if(c == KEYCODE_L){
            std::cout << "LEFT" << std::endl;
            angular=1.0;
            input=true;
        }
        else if(c==KEYCODE_R){
            std::cout << "RIGHT" << std::endl;
            angular=-1.0;
            input=true;
        }
        else if(c==KEYCODE_U){
            std::cout << "UP" << std::endl;
            linear=1.0;
	    angular=0.0;
            input=true;
        }
        else if(c==KEYCODE_D){
            std::cout << "DOWN" << std::endl;
            linear=-1.0;
	    angular=0.0;
            input=true;
        }
        else if(c==KEYCODE_Q){
            std::cout << "QUIT" << std::endl;
            input=false;
            return -1;
        }
        else{
            std::cout << "error" << std::endl;
        }

        final_linear=l_scale*linear;
        final_angular=a_scale*angular;

        cmd_vel.drive.steering_angle=final_angular;
        cmd_vel.drive.speed=final_linear;
        cmd_vel_.drive.steering_angle=final_angular;
        cmd_vel_.drive.speed=final_linear;

        if(input==true){
            std::cout << "output : " << final_linear << ", " << final_angular << std::endl;
            input=false;
        }

        cmd_pub.publish(cmd_vel);
        cmd_pub_.publish(cmd_vel_);




    }

    return(0);

}
