#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Mar 21 06:03:32 2019

@author: csrubin
"""

print('Importing modules...')
import paho.mqtt.client as mqtt
import time

def on_message(client, userdata, message):
    print('Message received: ', str(message.payload.decode('utf-8')))
    print('Message topic: ', message.topic)
    print('')

topic = 'capstone_test'
host = 'broker.hivemq.com'
port = 1883

print('Creating client')
client = mqtt.Client('test_client')

client.on_message = on_message

print('Connecting to host')
client.connect(host, port)

client.loop_start()

print('Subscribing to topic')
client.subscribe(topic)

#print('Publishing message')
#client.publish(topic, 'TESTING...1, 2, 3...')

while True: 
    client.subscribe(topic)
    time.sleep(4)