# copter_model description #

A simple package for a simple Coaxial Copter.

## Installation ##

We assume that you are working on Ubuntu 18.04 and already have installed ROS Melodic. If not, follow the [ROS install guide](http://wiki.ros.org/melodic/Installation/Ubuntu)

Prepare the repository:
```
cd ~
mkdir SimpleCopter_ws
mkdir SimpleCopter_ws/src
cd ~/SimpleCopter_ws/src
catkin_init_workspace
cd ~/SimpleCopter_ws
catkin_make
```

Above commands should execute without any warnings or errors.

Clone this repository to your workspace:

```
cd ~/SimpleCopter_ws/src
git clone https://gitlab.com/miggilcas/simplecopter_package.git
```

Install depencencies:

```
cd ~/SimpleCopter_ws
rosdep install --from-paths src --ignore-src -r -y
```

Build the workspace:

```
cd ~/SimpleCopter_ws
catkin_make
```

From this moment you can use the package. Please remember that each time, when you open new terminal window, you will need to load system variables:

```
source ~/SimpleCopter_ws/devel/setup.bash
```



## How to use ##

### Launching ###
Run the following commands below.
```
roslaunch copter_model Simple_copter.launch
```
## Support
Our emails are:
	miguel.gil2000@gmail.com 	dameseck121200@gmail.com	ok.jjg.juanjose@gmail.com


## Authors and acknowledgment
This project is an university project developed by Miguel Gil Castilla, Dame Seck Diop and Juan José García Calderón.

## License
Apache-2.0 License

## Project status
This project is not finished yet.

