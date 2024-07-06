使用sysfs测试，依赖echo命令
//1-8级亮度 "1"-"8"
echo "1" > /sys/class/misc/tm1650/brightness 
// 0-f "\x30-\x3f"
// 0.-f. "\x40-\x4f"
echo -e "\x32" > /sys/class/misc/tm1650/first 
echo -e "\x33" > /sys/class/misc/tm1650/second  
echo -e "\x33" > /sys/class/misc/tm1650/third 
echo -e "\x33" > /sys/class/misc/tm1650/fourth 
