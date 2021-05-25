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
    socket.connect(('10.0.0.201', 1234))

    text = {'total_intervals': 6, 
            'high_interval_ms': 7000, 
            'low_interval_ms': 3000, 
            'warmup_interval_ms': 420000, 
            'cooldown_interval_ms': 300000, 
            'repeat_times': 6}
    message = '{}\n'.format(json.dumps(text))
    socket.send(message.encode())

