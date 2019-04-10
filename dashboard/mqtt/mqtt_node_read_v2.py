#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Mar 21 06:03:32 2019

@author: csrubin
"""
import paho.mqtt.client as mqtt
import csv
import time

topic = 'node2'


def on_message(client, userdata, message):
    data = str(message.payload.decode('utf-8'))
    print('Message received: ', data)
    print('Message topic: ', message.topic)
    print('')

    data = data.split(',')
    with open('data.csv', 'a') as f:
        writer = csv.writer(f)
        writer.writerow(data)


host = 'broker.hivemq.com'
port = 1883

print('Creating client')
client = mqtt.Client('test_client')

client.on_message = on_message

print('Connecting to host')
client.connect(host, port)

client.loop_start()

while True:   
    print('Subscribing to topic.')
    client.subscribe(topic)
    print('Sleeping...')
    time.sleep(4)
    
    
    '''cont = input('Continue? (y/n):' )
    if cont.lower() == 'y':
        continue
    elif cont.lower() == 'n':
        break
    else:
        print('Not an acceptable value')'''

client.loop_stop()
