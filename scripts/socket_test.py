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
    while True:
        user_string = input()
        text = {'message': user_string, 'x': 0, 'y': 0, 'r': 255, 'g': 0, 'b': 0}
        message = '{}\n'.format(json.dumps(text))
        socket.send(message.encode())
