#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Apr  2 12:15:16 2019

@author: csrubin
"""

import paho.mqtt.client as mqtt
import csv
import time

topics = ['node1', 'node2', 'node3']


def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to broker.")
        global connected                #Use global variable
        connected = True                #Signal connection

    else:
        print("Connection failed.")


def on_message(client, userdata, message):
    '''When a message is received for a subsribed topics
    the data is displayed, split into a list, and written
    to a .csv file based on the the topic.'''

    data = str(message.payload.decode('utf-8'))
    print('Message topic: ', message.topic)
    print('Message received: ', data)
    print('')
    print('')

    data = data.split(',')

    with open(message.topic.upper()+'.csv', 'a') as f:
        writer = csv.writer(f)
        writer.writerow(data)

# MQTT declarations
connected = False
host = 'm16.cloudmqtt.com'
port = 10628
user = 'imwienrb'
pw = 'HltL0CNq12sz'

# Creating instance and starting connection
print('Creating client...')
client = mqtt.Client('soilmesh')
client.username_pw_set(user, password=pw)    #set username and password
client.on_connect= on_connect                #attach function to callback
client.on_message= on_message

print('Connecting to host...')
client.connect(host, port=port)

print('MQTT loop start.')
client.loop_start()

# Wait for connection
while connected != True:
    time.sleep(0.1)

# Subsribe to topics
for topic in topics:
    print('Subscribing to topic: %s' % topic)
    client.subscribe(topic)

# Loop indefinitely, waiting for messages
try:
    while True:
        time.sleep(1)

# Exit loop if KeyboardInterrupt
except KeyboardInterrupt:
    print()
    print('Disconnected and exited MQTT loop.')
    client.disconnect()
    client.loop_stop()

'''
while True:
    #client.loop()
    for topic in topics:
        print('Subscribing to topic: %s' % topic)
        client.subscribe(topic)
        time.sleep(4)

    #time.sleep(3)
    cont = input('Continue? (y/n):')
    if cont.lower() == 'y':
        continue
    elif cont.lower() == 'n':
        break09
    else:
        print('Not an acceptable value')
'''
