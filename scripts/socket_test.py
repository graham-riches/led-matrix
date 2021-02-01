"""
    @file
    @brief
    @author
    @details
   
"""

import socket
import json
import time

if __name__ == '__main__':
    socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    socket.connect(('192.168.1.40', 1234))
    for i in range(20):
        lights_on = {'x': i, 'y': i, 'r': 255, 'g': 0, 'b': 0}
        message = '{}\n'.format(json.dumps(lights_on))
        socket.send(message.encode())
