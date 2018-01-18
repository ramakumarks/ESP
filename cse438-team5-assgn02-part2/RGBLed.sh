#!/bin/bash
assigngpio(){

case "$1" in
	0)echo -n "11" >/sys/class/gpio/export
	  echo -n "32" >/sys/class/gpio/export
	  echo -n "out" >/sys/class/gpio/gpio11/direction
	  echo -n "out" >/sys/class/gpio/gpio32/direction
	  echo -n "0" >/sys/class/gpio/gpio11/value
	  echo -n "0" >/sys/class/gpio/gpio32/value;;
	1)echo -n "12" >/sys/class/gpio/export
	   echo -n "28" >/sys/class/gpio/export
	   echo -n "45" >/sys/class/gpio/export
	   echo -n "out" >/sys/class/gpio/gpio12/direction
	    echo -n "out" >/sys/class/gpio/gpio28/direction
	    echo -n "out" >/sys/class/gpio/gpio45/direction
	   echo -n 0 >/sys/class/gpio/gpio12/value
	    echo -n 0 >/sys/class/gpio/gpio28/value
	    echo -n 0 >/sys/class/gpio/gpio45/value;;
      2)echo -n "13" >/sys/class/gpio/export
	   echo -n "34" >/sys/class/gpio/export
	   echo -n "77" >/sys/class/gpio/export
	   echo -n "out" >/sys/class/gpio/gpio13/direction
	    echo -n "out" >/sys/class/gpio/gpio34/direction
	   echo -n 0 >/sys/class/gpio/gpio13/value
	    echo -n 0 >/sys/class/gpio/gpio34/value
	    echo -n 0 >/sys/class/gpio/gpio77/value;;
	3)echo -n "14" >/sys/class/gpio/export
	   echo -n "16" >/sys/class/gpio/export
	   echo -n "76" >/sys/class/gpio/export
	  echo -n "64" >/sys/class/gpio/export
	   echo -n "out" >/sys/class/gpio/gpio14/direction
	    echo -n "out" >/sys/class/gpio/gpio16/direction
	   echo -n 0 >/sys/class/gpio/gpio14/value 
	   echo -n 0 >/sys/class/gpio/gpio16/value
	    echo -n 0 >/sys/class/gpio/gpio76/value
	    echo -n 0 >/sys/class/gpio/gpio64/value;;
        4)echo -n "6" >/sys/class/gpio/export
	   echo -n "36" >/sys/class/gpio/export
	   echo -n "out" >/sys/class/gpio/gpio6/direction
	    echo -n "out" >/sys/class/gpio/gpio36/direction 
	   echo -n 0 >/sys/class/gpio/gpio6/value
	    echo -n 0 >/sys/class/gpio/gpio36/value;;
	5)echo -n "0" >/sys/class/gpio/export
	   echo -n "18" >/sys/class/gpio/export
	   echo -n "66" >/sys/class/gpio/export
	   echo -n "out" >/sys/class/gpio/gpio0/direction
	    echo -n "out" >/sys/class/gpio/gpio18/direction
	   echo -n 0 >/sys/class/gpio/gpio0/value 
	   echo -n 0 >/sys/class/gpio/gpio18/value
	    echo -n 0 >/sys/class/gpio/gpio66/value;;
	6)echo -n "1" >/sys/class/gpio/export
	   echo -n "20" >/sys/class/gpio/export
	   echo -n "68" >/sys/class/gpio/export
	   echo -n "out" >/sys/class/gpio/gpio1/direction
	    echo -n "out" >/sys/class/gpio/gpio20/direction
	   echo -n 0 >/sys/class/gpio/gpio1/value 
	   echo -n 0 >/sys/class/gpio/gpio20/value
	    echo -n 0 >/sys/class/gpio/gpio68/value;;
	7)echo -n "38" >/sys/class/gpio/export
	   echo -n "out" >/sys/class/gpio/gpio38/direction 
	    echo -n 0 >/sys/class/gpio/gpio38/value;;
	8)echo -n "40" >/sys/class/gpio/export
	   echo -n "out" >/sys/class/gpio/gpio40/direction 
	    echo -n 0 >/sys/class/gpio/gpio40/value;;
	9)echo -n "4" >/sys/class/gpio/export
	   echo -n "22" >/sys/class/gpio/export
	   echo -n "70" >/sys/class/gpio/export
	   echo -n "out" >/sys/class/gpio/gpio4/direction
	    echo -n "out" >/sys/class/gpio/gpio22/direction
	   echo -n 0 >/sys/class/gpio/gpio4/value 
	   echo -n 0 >/sys/class/gpio/gpio22/value
	    echo -n 0 >/sys/class/gpio/gpio70/value;;
	10)echo -n "26" >/sys/class/gpio/export
	   echo -n "10" >/sys/class/gpio/export
	   echo -n "74" >/sys/class/gpio/export
	   echo -n "out" >/sys/class/gpio/gpio26/direction
	    echo -n "out" >/sys/class/gpio/gpio10/direction
	   echo -n 0 >/sys/class/gpio/gpio26/value
	    echo -n 0 >/sys/class/gpio/gpio10/value
	    echo -n 0 >/sys/class/gpio/gpio74/value;;
	11)echo -n "5" >/sys/class/gpio/export
	   echo -n "24" >/sys/class/gpio/export
	   echo -n "44" >/sys/class/gpio/export
	  echo -n "72" >/sys/class/gpio/export
	   echo -n "out" >/sys/class/gpio/gpio5/direction
	    echo -n "out" >/sys/class/gpio/gpio24/direction
	  echo -n "out" >/sys/class/gpio/gpio44/direction
	   echo -n 0 >/sys/class/gpio/gpio5/value 
	   echo -n 0 >/sys/class/gpio/gpio24/value
	    echo -n 0 >/sys/class/gpio/gpio44/value
	    echo -n 0 >/sys/class/gpio/gpio72/value;;
	 12)echo -n "15" >/sys/class/gpio/export
	   echo -n "42" >/sys/class/gpio/export
	   echo -n "out" >/sys/class/gpio/gpio15/direction
	    echo -n "out" >/sys/class/gpio/gpio42/direction 
	   echo -n 0 >/sys/class/gpio/gpio15/value
	    echo -n 0 >/sys/class/gpio/gpio42/value;;
	13)echo -n "7" >/sys/class/gpio/export
	   echo -n "30" >/sys/class/gpio/export
	   echo -n "46" >/sys/class/gpio/export
	   echo -n "out" >/sys/class/gpio/gpio7/direction
	    echo -n "out" >/sys/class/gpio/gpio30/direction
	   echo -n "out" >/sys/class/gpio/gpio46/direction
	   echo -n 0 >/sys/class/gpio/gpio7/value
	    echo -n 0 >/sys/class/gpio/gpio30/value
	    echo -n 0 >/sys/class/gpio/gpio46/value;;
	 *)echo "no value set";;

esac
}
LED(){

case "$1" in
	0)echo -n "$2" >/sys/class/gpio/gpio11/value
	  echo -n "$2" >/sys/class/gpio/gpio32/value;;
	1)echo -n "$2" >/sys/class/gpio/gpio12/value
	  echo -n "$2" >/sys/class/gpio/gpio28/value;;
	2)echo -n "$2" >/sys/class/gpio/gpio13/value
	  echo -n "$2" >/sys/class/gpio/gpio34/value;;
	3)echo -n "$2" >/sys/class/gpio/gpio14/value
	  echo -n "$2" >/sys/class/gpio/gpio16/value;;
	4)echo -n "$2" >/sys/class/gpio/gpio6/value
	  echo -n "$2" >/sys/class/gpio/gpio36/value;;
	5)echo -n "$2" >/sys/class/gpio/gpio0/value
	  echo -n "$2" >/sys/class/gpio/gpio18/value;;
	6)echo -n "$2" >/sys/class/gpio/gpio1/value
	  echo -n "$2" >/sys/class/gpio/gpio20/value;;
	7)echo -n "$2" >/sys/class/gpio/gpio38/value;;
	8) echo -n "$2" >/sys/class/gpio/gpio40/value;;
	9)echo -n "$2" >/sys/class/gpio/gpio4/value
	  echo -n "$2" >/sys/class/gpio/gpio22/value;;
	10)echo -n "$2" >/sys/class/gpio/gpio10/value
	   echo -n "$2" >/sys/class/gpio/gpio26/value;;
	11)echo -n "$2" >/sys/class/gpio/gpio5/value
	  echo -n "$2" >/sys/class/gpio/gpio24/value;;
	12)echo -n "$2" >/sys/class/gpio/gpio15/value
	  echo -n "$2" >/sys/class/gpio/gpio42/value;;
	13)echo -n "$2" >/sys/class/gpio/gpio7/value
	  echo -n "$2" >/sys/class/gpio/gpio30/value;;
        *)echo "no value set";;
esac

}



green(){
count=0
while [ $count -le 26 ]
do 
 count=$(( $count + 1 ))

 LED "$1" 1
 usleep $c1
 LED "$1" 0
 usleep $c2
done
}
red(){
count=0
while [ $count -le 26 ]
do 
 count=$(( $count + 1 ))

 LED "$1" 1
 usleep $c1
 LED "$1" 0
 usleep $c2
done
}
blue(){
 count=0
while [ $count -le 26 ]
do 
 count=$(( $count + 1 ))

 LED "$1" 1
 usleep $c1
 LED "$1" 0
 usleep $c2
done
}
RG(){
count=0
while [ $count -le 26 ]
do 
 count=$(( $count + 1 ))
 LED "$1" 1
 LED "$2" 1
 usleep $c1
 LED "$1" 0
 LED "$2" 0
 usleep $c2
done
}
RB(){
 count=0
while [ $count -le 26 ]
do 
 count=$(( $count + 1 ))
 LED "$1" 1
 LED "$2" 1
 usleep $c1
 LED "$1" 0
 LED "$2" 0
 usleep $c2
done
}
BG(){
 count=0
while [ $count -le 26 ]
do 
 count=$(( $count + 1 ))
 LED "$1" 1
 LED "$2" 1
 usleep $c1
 LED "$1" 0
 LED "$2" 0
 usleep $c2
done
}
RGB(){
count=0
while [ $count -le 26 ]
do 
 count=$(( $count + 1 ))
 LED "$1" 1
 LED "$2" 1
 LED "$3" 1
 usleep $c1
 LED "$1" 0
 LED "$2" 0
 LED "$3" 0
 usleep $c2
done
}


r=1 
g=2
b=3
duty_cycle=70
cycle_duration=20000
assigngpio $g
assigngpio $r
assigngpio $b
red $r

#c1=$(bc <<< "scale=2;($duty_cycle/100)*($cycle_duration)")
#c1=${c1%.*}
#c2=$(($cycle_duration-$c1))
c1=$(($duty_cycle*200))
c2=$((20000-$c1))

while :
do
 red $r
 green $g
 blue $b
 RG $r $g
 BG $b $g
 RB $r $b
 RGB $r $g $b
done

