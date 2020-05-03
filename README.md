# cs333-project1

A Linux kernel module which creates a character device allowing processes on user space to open and read a random number.

---
## NOTE: Install the module BEFORE running the application

## Installation/Uninstallation for module

- Compile the module
```shell
$ cd Project1
$ make
```
- Install the module into the kernel
```shell
$ sudo insmod Project1.ko
```

- See the random number
```shell
$ sudo-i
$ ls /sys/class/Project1_class
$ cat /dev/Project1_device
$ dmesg
```

- Uninstall the module out of the kernel
```shell
$ sudo rmmod Project1
```

## Features
- See the list of file/folder in current folder
```shell
$ ls
```
- See the list of drivers with their major numbers
```shell
$ cat /proc/devices
```

- See how the module execute
> You can use `dmesg` to show all
> To see just n lastest lines, use `| -n`
```shell
$ dmesg | tail -10
```
- Provide normal user with permissions to access device file
```shell
$ sudo chmod 666 /dev/Project1_device
```

- Check whether the device file is created or not
```shell
$ ls -la /dev/Project1_device
```

--- 
## Installation for Random Number Application
- Run the application
```shell
$ cd randnum_app
$ ./randnum_app
```

- Following the instructions on the screen~~~
