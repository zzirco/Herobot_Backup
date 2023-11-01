#!/usr/bin/env python

# Author: christoph.roesmann@tu-dortmund.de
# Modifier: Donghee Han, hdh7485@kaist.ac.kr

import rospy
from ackermann_msgs.msg import AckermannDriveStamped
from std_msgs.msg import String

def emergency_callback(msg):
  global state
  state = msg.data

def cmd_callback(data):

  msg = AckermannDriveStamped()

  if(state != "emergency"):  
    msg.header.stamp = rospy.Time.now()
    msg.header.frame_id = frame_id
    msg.drive.steering_angle = data.drive.steering_angle
    msg.drive.speed = data.drive.speed
  else: #state == emergency
    msg.header.stamp = rospy.Time.now()
    msg.header.frame_id = frame_id
    msg.drive.steering_angle = 0
    msg.drive.speed = 0
      
  pub.publish(msg)

if __name__ == '__main__': 
  try:
    
    rospy.init_node('cmd_vel_to_ackermann_drive')

    #subscribe 
    safety_controller_topic = rospy.get_param('~safety_controller_topic', '/ackermann_cmd_from_sc')
    emergency_topic = rospy.get_param('~emergency_topic', '/emergency_state')

    #publish
    ackermann_cmd_topic = rospy.get_param('~ackermann_cmd_topic', '/ackermann_cmd')

    wheelbase = rospy.get_param('~wheelbase', 1.0)
    frame_id = rospy.get_param('~frame_id', 'odom')
    message_type = rospy.get_param('~message_type', 'ackermann_drive') # ackermann_drive or ackermann_drive_stamped

    #subscribe
    rospy.Subscriber(safety_controller_topic, AckermannDriveStamped, cmd_callback, queue_size=1)
    rospy.Subscriber(emergency_topic, String, emergency_callback, queue_size=1)

    #publish
    pub = rospy.Publisher(ackermann_cmd_topic, AckermannDriveStamped, queue_size=1)
    
    rospy.loginfo("Node 'cmd_vel_to_ackermann_drive' started.\nListening to %s, publishing to %s. Frame id: %s, wheelbase: %f", "/cmd_vel", ackermann_cmd_topic, frame_id, wheelbase)
    rospy.spin()
    
  except rospy.ROSInterruptException:
    pass
