#!/bin/bash
current_PID=$$
echo "`date "+%Y-%m-%d %T"` : PID is $current_PID" >> debuglog.txt
while true
do
  PROSS=`ps aux | grep portal-arm | grep -v grep | awk '{print $2}'`
  if [ $PROSS ]; then
        echo "`date "+%Y-%m-%d %T"` : portal-arm  $PROSS programm  is running" >> debuglog.txt
  else
	echo "`date "+%Y-%m-%d %T"` : portal-arm the programm  has dead" >> debuglog.txt
  fi
  sleep 1m
done
