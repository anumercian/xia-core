#!/bin/bash

if [ ! -d "android-deps/protobuf" ]; then
  svn co -r378 http://protobuf.googlecode.com/svn/trunk/ android-deps/protobuf
fi