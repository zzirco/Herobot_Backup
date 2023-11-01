import rospy
from roslibpy import Topic, Ros

ros = Ros('localhost', 9090)
ros.run()

# 토픽에 대한 콜백 함수 정의
def callback(message):
    print(message['data'])

# 토픽 구독
topic = Topic(ros, '/emergency_state', 'std_msgs/String')
topic.subscribe(callback)

rospy.spin()
