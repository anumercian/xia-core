#!/bin/bash

cd ~/xia-core/proxies
sudo killall -9 proxy.py
sleep 1
./proxy.py 15000 &