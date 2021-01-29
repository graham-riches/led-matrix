"""
    @file
    @brief
    @author
    @details
   
"""

import socket

if __name__ == '__main__':
    socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    socket.connect(('192.168.1.40', 1234))
    data = socket.recv(128)
    print(data.decode('utf-8'))
