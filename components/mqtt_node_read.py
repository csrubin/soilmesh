#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Mar 21 06:03:32 2019

@author: csrubin
"""

print('Importing modules...')
import paho.mqtt.client as mqtt
import time
import pandas as pd 
import csv 

topics = ['soilmesh/node1/latitude', 'soilmesh/node1/longitude', 'soilmesh/node1/year',
          'soilmesh/node1/month', 'soilmesh/node1/day', 'soilmesh/node1/hour', 'soilmesh/node1/minute'
          'soilmesh/node1/second', 'soilmesh/node1/altitude', 'soilmesh/node1/satellites']

#variables = [lat, long, year, month, day, hour, minute, second, alt, sat]

def on_message(client, userdata, message):
    print('Message received: ', str(message.payload.decode('utf-8')))
    print('Message topic: ', message.topic)
    print('')
    
    
    #for var in variables:
        
    if message.topic == topics[0]:
        lat = str(message.payload.decode('utf-8'))
        
    elif message.topic == topics[1]:
        long = str(message.payload.decode('utf-8'))
        
    elif message.topic == topics[2]:
        year = str(message.payload.decode('utf-8'))
        
    elif message.topic == topics[3]:
        month = str(message.payload.decode('utf-8'))
        
    elif message.topic == topics[4]:
        day = str(message.payload.decode('utf-8'))
    
    elif message.topic == topics[5]:
        hour = str(message.payload.decode('utf-8'))

    elif message.topic == topics[6]:
        minute = str(message.payload.decode('utf-8'))
        
    elif message.topic == topics[7]:
        second = str(message.payload.decode('utf-8'))
    
    elif message.topic == topics[8]:
        alt = str(message.payload.decode('utf-8'))
        
    elif message.topic == topics[9]:
        sat = str(message.payload.decode('utf-8'))
    
    data = [year, month, day, hour, minute, second, lat, long, alt, sat]
    
    return data
    
    with open('data.csv','rb') as f:
        writer = csv.writer(f)
        writer.writerows(data)
        
host = 'broker.hivemq.com'
port = 1883

print('Creating client')
client = mqtt.Client('test_client')

client.on_message = on_message

print('Connecting to host')
client.connect(host, port)

client.loop_start()



#while True: 
#for t in topics:
 #   client.subscribe(t)
#time.sleep(4)

client.subscribe('soilmesh/#')

client.loop_stop()
