# Setup your Raspberry Pi 3

## ROS Kinetic Kame on Ubuntu 16.04 (LXDE)
Use Ubiquity Robotics's Raspberry Pi robot development image for it comes with pre-installed ROS Kinetic.
An SD Card Image with Ubuntu 16.04 for Raspberry Pi 3 can be downloaded from [Ubiquity Robotics Downloads](https://downloads.ubiquityrobotics.com/).
You can flash the image onto the SD card using [Etcher](https://www.balena.io/etcher/) or tool of your choice.

Plug the Raspberry Pi with a networked device, boot it up & SSH into it (for headless setup)

`username:ubuntu`
`password:ubuntu`

or connect it to screen & keyboard, log in and get started with the setup.

### Setup password
	$ passwd
Enter `ubuntu`, new-password and verify new password

### Allow USER to execute all commands without password
	$ sudo visudo
Append :

	## Allow user ubuntu to run all commands without password
	ubuntu ALL=(ALL) NOPASSWD: ALL

Log out and log back in to be effective.

### Add USER to DIALOUT group
	$ sudo usermod -a -G dialout ubuntu
	$ getent group dialout

### Update and Upgrade
	$ sudo apt update
	$ sudo apt upgrade -y

### Disable startup scripts
	$ sudo systemctl stop magni-base
	$ sudo systemctl stop roscore.service
	$ sudo systemctl disable magni-base
	$ sudo systemctl disable roscore.service

### Change HOSTNAME (optional)
	$ hostname
	$ vi /etc/hostname
	$ sudo chmod a+rw /etc/hosts
	$ vi /etc/hosts
Will be effective after reboot.

### Install tmux
	$ sudo apt install tmux

### Install VNC server
	$ sudo apt install xfce4 xfce4-goodies tightvncserver -y
	
Reboot.
