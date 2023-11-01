#!/usr/bin/env python
# -*- coding: utf-8 -*-

import socket
import string
import rospy
import time
from roslibpy import Topic, Ros

HOST = 'localhost'
PORT1 = 60000
PORT2 = 60001

# 소켓1 생성 7777번 포트
sock1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock1.bind((HOST, PORT1))
sock1.listen(1)

# 소켓2 생성 8888번 포트
sock2 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock2.bind((HOST, PORT2))
sock2.listen(1)

print('Server Conected')

ros = Ros('localhost', 9090)
ros.run()

# 토픽에 대한 콜백 함수 정의
def callback(message):
    global state 
    state = message['data']


# 토픽 구독
topic = Topic(ros, '/emergency_state', 'std_msgs/String')
topic.subscribe(callback)



while True:

    conn1, addr1 = sock1.accept()
    print('Connected 1')
    
    conn2, addr2 = sock2.accept()
    print('Connected 2')

    while True:
        if(state=="emergency"):
            message1 = "e"
            message2 = "call"
        elif(state=="normal"):
            message1 = "d"
            message2 = "a"
        conn1.send(message1.encode())
        conn2.send(message2.encode())
        time.sleep(1)


rospy.spin()
