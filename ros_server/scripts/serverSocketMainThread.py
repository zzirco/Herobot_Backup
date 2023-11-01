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
import serverConnectionSubThread


class ServerSocketMainThread(threading.Thread):
    def __init__(self, HOST, PORT):
        threading.Thread.__init__(self)  # 부모클래스의 생성자 호출(threading.Thread)

        self.HOST = HOST
        self.PORT = PORT

        self.chatBotServerSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.chatBotServerSocket.bind((self.HOST, self.PORT))
        self.chatBotServerSocket.listen(1)

        self.clientConnectionList = []
        self.connectionSubThreadList = []

    def run(self):
        try:
            while True:
                print("Waiting Client Connection")  # 클라이언트 접속대기
                clientConnection, clientAddress = self.chatBotServerSocket.accept()  # 클라이언트 접속
                self.clientConnectionList.append(clientConnection)  # 접속한 클라이언트의 소켓 정보를 리스트에 추가

                print("connect at ", clientAddress)

                connectionSubThread = serverConnectionSubThread.ServerConnectionSubThread(self.connectionSubThreadList,
                                                                                          self.clientConnectionList,
                                                                                          clientConnection,
                                                                                          clientAddress)
                connectionSubThread.start()  # 접속한 해당 클라이언트의 서브스레드를 시작
                self.connectionSubThreadList.append(connectionSubThread)  # 시작된 서브 스레드 리스트에 추가

        except:
            print("serverThread error")
