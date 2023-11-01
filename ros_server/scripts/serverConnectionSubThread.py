#!/usr/bin/env python2
# -*- coding: utf-8 -*- 
# Software License Agreement (BSD License)
#
# Copyright (c) 2008, Willow Garage, Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above
#    copyright notice, this list of conditions and the following
#    disclaimer in the documentation and/or other materials provided
#    with the distribution.
#  * Neither the name of Willow Garage, Inc. nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# Revision $Id$

## Simple talker demo that published std_msgs/Strings messages
## to the 'chatter' topic

import rospy
from std_msgs.msg import String
import socket, threading
import numpy as np
global money



class ServerConnectionSubThread(threading.Thread):
    def __init__(self, connectionSubThreadList, clientConnectionList, clientConnection, clientAddress):
        threading.Thread.__init__(self)  # 스레드를 클래스로 정의 할 때는 __init__메소드는 부모 클래스의 생성자를 반드시 호출 해야함

        self.connectionSubThreadList = connectionSubThreadList
        self.clientConnectionList = clientConnectionList
        self.clientConnection = clientConnection
        self.clientAddress = clientAddress
        self.daemon = True; # 백그라운드 스레드(메인스레드 종료시 종료)

    def run(self):
        try:
            count=1
            start_node=1
            receive_node=1
            text = "The estimated price = "
            global money
            while True:
                data = self.clientConnection.recv(1024).decode()  # 연결된 클라이언트의 메세지를 기다린다. #이 부분을 publish
		if data != "check\n":
                    num = int(data)
                
                if not data:  # 만약 클라이언트의 서버접속 해제시 recv()는 0을 리턴한다.
                    print("client_disconnect : ", self.clientConnection)
                    break

                if count==1 and data !="check\n":
                    start_node = num
                    start=data
                    print("Start : ", start)
                    startPub=rospy.Publisher('Start', String, queue_size=10)
                    startPub.publish(start)
                    count=0

                elif count==0 and data !="check\n":
                    receive_node = num
                    receive=data 
                    print("Receive : ", receive)
                    revPub=rospy.Publisher('Receive', String,  queue_size=10)
                    revPub.publish(receive)
                    count=1
                    cash = (receive_node-start_node)*100
                    money = text+str(cash+3000)+"\n"
                    self.clientConnection.sendall(money.encode())      

               
                elif (count==1 or count==0) and data == "check\n":
                    self.clientConnection.sendall("The taxi has departed\n".encode())
                    state = "1"
                    statePub = rospy.Publisher("State", String, queue_size=10)
                    statePub.publish(state)
                    
                
        except:
            self.clientConnectionList.remove(self.clientConnection)
            self.connectionSubThreadList.remove(self)
            exit(0)

        self.clientConnectionList.remove(self.clientConnection)
        self.connectionSubThreadList.remove(self)
