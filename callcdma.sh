# !bin/bash
ps w | grep pppd | grep -v grep
if [ $? -ne 0 ];then
(pppd call cdma & ) >> debuglog.txt 
fi

