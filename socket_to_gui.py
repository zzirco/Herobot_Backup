#!/usr/bin/env python

import socket
import string
import rospy
import time
from roslibpy import Topic, Ros

HOST = '192.168.229.125'
PORT1 = 50009

sock1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock1.bind((HOST, PORT1))
sock1.listen(1)

ros = Ros('localhost', 9090)
ros.run()

def xy_callback(message):
    global xy_state 
    xy_state = message['data']

def emergency_callback(message):
    global emergency_state 
    emergency_state = message['data']


emergency_topic = Topic(ros, '/emergency_state', 'std_msgs/String')
emergency_topic.subscribe(emergency_callback)

xy_topic = Topic(ros, '/index', 'std_msgs/String')
xy_topic.subscribe(xy_callback)



while True:

    conn1, addr1 = sock1.accept()

    while True:
        conn1.send(xy_state.encode() + ' ' + emergency_state.encode())
        time.sleep(0.01)


rospy.spin()
