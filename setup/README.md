# Setup password
	$ passwd
	Enter ubuntu, new-password and verify new password

# Start Update and Upgrade
	$ sudo apt update
	$ sudo apt upgrade -y

# Disable startup scripts
	$ sudo systemctl stop magni-base
	$ sudo systemcrl stop roscore.service
	$ sudo systemctl disable magni-base
	$ sudo systemctl disable roscore.service

# Allow USER to execute all commands without password
	$ sudo visudo
	Append :
		## Allow user ubuntu to run all commands without password
		ubuntu ALL=(ALL) NOPASSWD: ALL
	Log out and log back in to be effective.

# Add USER to DIALOUT group
	$ sudo usermod -a -G dialout ubuntu
	$ getent group dialout

# Change HOSTNAME permanently
	$ hostname
	$ vi /etc/hostname
		Change hostname.
	$ sudo chmod a+rw /etc/hosts
	$ vi /etc/hosts
		Change hostname.
	Reboot.

# Install and start VNC Server
	$ sudo apt install xfce4 xfce4-goodies tightvncserver -y
	$ vncserver

# Configure pifi :
	$ pifi status
	$ vi /etc/pifi/default_ap.em
