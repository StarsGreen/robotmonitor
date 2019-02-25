#!/bin/bash

case $1 in
	"0")
        rm accel_data angle_vel_data sensor_data jour_data
	;;
	"1")
	nmcli dev wifi connect "Pi_Wireless" password "ZZX123321"
	echo "$1"
	echo "Conncting Pi_Wireless"
	;;
	"2")
	nmcli dev wifi connect "GreenStar" password "zzx123321"
	echo "Connecting GreenStar"
	;;
	"3")
	nmcli dev wifi connect "ChinaNet-1a2b" password "wxl15972152591"
	echo "Connecting china-net"
	;;
	"4")
	git init
	git add -A
	cur_date=$(date)
	git commit -m "$cur_date-----edition 2.0"
	git remote add robotmonitor https://github.com/StarsGreen/robotmonitor.git
	git push -u robotmonitor master
	;;
esac
