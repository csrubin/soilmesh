#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import paho.mqtt.client as mqtt
import csv
import time

topic = 'node_receive'



def on_message(client, userdata, message):
    data = str(message.payload.decode('utf-8'))
    print('Message received: ', data)
    print('Message topic: ', message.topic)
    print('')

    data = data.split(',')

    with open(message.topic+'.csv', 'a') as f:
        writer = csv.writer(f)
        writer.writerow(data)     



host = 'broker.hivemq.com'
port = 1883

print('Creating client')
client = mqtt.Client('soilmesh')

client.on_message = on_message

print('Connecting to host')
client.connect(host, port)

client.loop_start()

while True:
    
    state = input('off/on:' )
    if state.lower() == 'off':
        client.publish(topic,'OFF')
        print('Turning light off...')
    elif state.lower() == 'on':
        client.publish(topic,'ON')
        print('Turning light on...')
    else:
        print('Not an acceptable value')
    
    time.sleep(1)
    
    cont = input('Continue? (y/n):')

    if cont.lower() == 'y':
        continue
    elif cont.lower() == 'n':
        break
    else:
        print('Not an acceptable value')
        
    client.subscribe(topic)
    time.sleep(3)

client.loop_stop()
