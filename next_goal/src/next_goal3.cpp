#include "next_goal.h"

point::point()
{
    ros::NodeHandle nh;

    //publish
    goal_pub = nh.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal",10);
    index_pub = nh.advertise<std_msgs::String>("/index",10);
    //subscibe
    goal_sub = nh.subscribe("/move_base_simple/goal",10,&point::goal_callback,this);
    current_sub = nh.subscribe("/amcl_pose",10,&point::amcl_callback,this);
    state_sub = nh.subscribe("/emergency_state",10,&point::state_callback,this);
    //point
    node[0]={0.49, -0.06, 1, 0};  //0.49
    node[1]={2.45, 0, 0.87, 0.49};
    node[2]={3.0, 1.34, 0.71, 0.70};
    node[3]={2.45, 3.22, 0.45, 0.89};
    node[4]={1.52, 3.65, 0, 1};  //1.52
    node[5]={0.26, 3.65, 0, 1};  //0.26
    node[6]={-1.11, 3.46, -0.47, 0.88};
    node[7]={-2.12, 2.14, 0.70, -0.71};
    node[8]={-2.08, 0.78, 0.92, -0.39};
    node[9]={-0.87, -0.06, 1, 0};  //-0.87
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
    string str1 = to_string(xp2);
    string str2 = to_string(yp2);
    pose.data = str1 + ' ' + str2;
    std::cout<<"pose : "<<pose<<std::endl;
    index_pub.publish(pose);
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
        if(d<=0.8)
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
    ros::init(argc, argv, "next_goal");
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
