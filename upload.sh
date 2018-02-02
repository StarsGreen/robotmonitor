#!/bin/bash

case $1 in
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
	git init
	git add -A
	cur_date=$(date)
	git commit -m "$cur_date"
	git remote add remoterobot https://github.com/StarsGreen/RemoteRobot.git
	git push -u remoterobot master
	;;
esac
