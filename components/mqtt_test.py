#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Mar 18 11:07:11 2019

@author: csrubin
"""
import paho.mqtt.client as mqtt
import time

def on_message(client, userdata, message):
    print('Message received: ', str(message.payload.decode('utf-8')))
    print('Message topic: ', message.topic)
    print('Message qos: ', message.qos)
    print('Message retain flag: ', message.retain)


topic = 'capstone_test'
host = 'broker.mqttdashboard.com'
port = 1883

print('Creating client')
client = mqtt.Client('test_client')

client.on_message = on_message

print('Connecting to host')
client.connect(host, port)

client.loop_start()

print('Subscribing to topic')
client.subscribe(topic)

print('Publishing message')
client.publish(topic, 'TESTING...1, 2, 3...')

time.sleep(4)

client.loop_stop()