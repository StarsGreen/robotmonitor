#!/bin/bash
data=""
case $1 in
	"1")
data=$(i2cdetect -y 1)
echo -e $data
	;;
	"2")
data="\nx_accel_data:  "$(i2cget -y 1 0x68 0x3b)"|"$(i2cget -y 1 0x68 0x3c)
data=$data"\ny_accel_data:"$(i2cget -y 1 0x68 0x3d)"|"$(i2cget -y 1 0x68 0x3e)
data=$data"\nz_accel_data:"$(i2cget -y 1 0x68 0x3f)"|"$(i2cget -y 1 0x68 0x40)
echo -e $data
	;;
	"3")
data="\nxa_vel_data:  "$(i2cget -y 1 0x68 0x43)"|"$(i2cget -y 1 0x68 0x44)
data=$data"\nya_vel_data:"$(i2cget -y 1 0x68 0x45)"|"$(i2cget -y 1 0x68 0x46)
data=$data"\nza_vel_data:"$(i2cget -y 1 0x68 0x47)"|"$(i2cget -y 1 0x68 0x48)
echo -e $data
	;;
esac
