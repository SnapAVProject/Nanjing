# !bin/bash

#cd /home/development/lxyz/smartpackage/update
#unzip -o -o Qt.zip
#./smartpackage

#killall portal-arm
#./portal-arm -qws &
#./hardware &
#./smartpackage
cd /usr/lxyz
rm portal-arm
unzip -o file/Qt.zip
sleep 15
chmod 755 portal-arm
rm file/Qt.zip
reboot
