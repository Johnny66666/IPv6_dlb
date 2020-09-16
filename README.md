# IPv6_dlb

# Description: 
  
These are simple implementations of DLB.
We have tested ECMP,RPS,DRILL and DLB in the Network Simulator version 2(NS2).


# Usage:

## 1.Prepare a server with Ubuntu 14.04 and download NS-2.35.
This is the download url of NS-2.35: https://www.isi.edu/nsnam/ns/ns-build.html

## 2. Install the NS-2： 
	2.1 After installing the Ubuntu system, firstly update the software and libraries in the system, enter the following commands in the terminal
		sudo app-get update 		#Update source list
		sudo app-get upgrade 		#Update installed packages
		sudo app-get dist-upgrade 	#Update software

	2.2 Install several required packages, enter the following commands in the terminal:
		sudo apt-get install build-essential
		sudo apt-get install tcl8.5 tcl8.5-dev tk8.5 tk8.5-dev 			#for tcl and tk
		sudo apt-get install libxmu-dev libxmu-headers 				#for nam

	2.3 Install the prepared ns-allinone-2.35.tar.gz, enter the following command in the terminal:
		tar -zxf ns-allinone-2.35.tar.gz 		#Unzip the installation package
		cd ns-allinone-2.35 				#Enter the installation directory
		./install 					#Start installation



## 3.Patching the DLB
	Replace the switch with the files you need to modify:

	cp classifier-mpath.cc ~/ns-allinone-2.35/ns-allinone-2.35/ns-2.35/classifier/classifier-mpath.cc
	cp drop-tail.h ~/ns-allinone-2.35/ns-allinone-2.35/ns-2.35/queue/drop-tail.h
	cp drop-tail.cc ~/ns-allinone-2.35/ns-allinone-2.35/ns-2.35/queue/drop-tail.cc
	cd ~/ns-allinone-2.35/ns-2.35
	make
		
If you have any questions, please email to csu_jn@csu.edu.cn. 
