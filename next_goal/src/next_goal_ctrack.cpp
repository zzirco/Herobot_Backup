#include "next_goal.h"

point::point()
{
    ros::NodeHandle nh;

    //publish
    goal_pub = nh.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal",10);
    //subscibe
    goal_sub = nh.subscribe("/move_base_simple/goal",10,&point::goal_callback,this);
    current_sub = nh.subscribe("/amcl_pose",10,&point::amcl_callback,this);
    state_sub = nh.subscribe("/emergency_state",10,&point::state_callback,this);
    //point
    node[0]={2.55, 3.66, 0.96, -0.24};
    node[1]={4.93, 2.76, 0.98, 0.14};
    node[2]={7.38, 3.38, 0.99, 0.11};
    node[3]={8.75, 5.02, 0.83, 0.54};
    node[4]={10.22, 6.26, 0.61, 0.78};
    node[5]={8.55, 7.53, 0.27, 0.96};
    node[6]={7.55, 8.25, -0.11, 0.99};
    node[7]={4.09, 7.41, -0.12, 0.99};
    node[8]={1.90, 5.87, -0.49, 0.86};
    node[9]={1.24, 4.87, 0.80, -0.59};
}

void point::goal_callback(const geometry_msgs::PoseStamped& msg)
{
    goal = msg;
    xp1 = goal.pose.position.x;
    yp1 = goal.pose.position.y;
    state = true;
}

void point::amcl_callback(const geometry_msgs::PoseWithCovarianceStamped& msg)
{
    amcl = msg;
    xp2 = amcl.pose.pose.position.x;
    yp2 = amcl.pose.pose.position.y;
}

void point::state_callback(const std_msgs::String& msg)
{
    emer_state = msg.data;
}
float point::calcDistance()
{
    codist = (xp1-xp2)*(xp1-xp2)+(yp1-yp2)*(yp1-yp2);
    dist = sqrt(codist);
    std::cout<<"dist : "<<dist<<std::endl;
    return dist;
}

Node point::nextGoal()
{
    float min = 10000;
    
    if(first_state == false)
    {
        for(int i=0; i<10; i++)
        {
            codistance = (node[i].x-amcl.pose.pose.position.x)*(node[i].x-amcl.pose.pose.position.x)+(node[i].y-amcl.pose.pose.position.y)*(node[i].y-amcl.pose.pose.position.y);
            distance = sqrt(codistance);
            if(distance < min)
            {
                min = distance;
                min_index = i;
            }
        }
        first_state = true;
        std::cout<<"GOOD"<<std::endl;
        return node[min_index + 1];
    }
    else
    {
        std::cout<<"min_index : "<<min_index<<std::endl;
        min_index = min_index + 1;
        if(min_index > 9)
        {    
            min_index = 0;
            return node[min_index];
        }
        else
        {
           
            return node[min_index];
        }
    }
}

void point::pub_goal()
{

    Node nextnode;
 
    nextnode = nextGoal();

    next_goal.pose.position.x = nextnode.x;
    next_goal.pose.position.y = nextnode.y;

    next_goal.pose.orientation.w = nextnode.w;
    next_goal.pose.orientation.z = nextnode.z;
 
    next_goal.header.frame_id = "map";

    goal_pub.publish(next_goal);

}

void point::isPub()
{
    std::cout<<"min_index : "<<min_index<<std::endl;
    if(state == true)
    {
        float d;
        d=calcDistance();
        if(d<=1.0)
        {
            pub_goal();
        }
        else
        {
            ;
        }
    }
    else
    {
        ;
    }
}

string point::get_state()
{
    return emer_state;
}

int main(int argc, char * argv[])
{
    ros::init(argc, argv, "next_goal_ctrack");
    ros::NodeHandle nh;
    ros::NodeHandle nh_priv("~");

    point p;

    ros::Rate  loop_rate(10);

    while(ros::ok())
    {
        p.isPub();
        loop_rate.sleep();
        ros::spinOnce();
    }

    ros::spin();
    return 0;
}
