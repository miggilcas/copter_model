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

### Data visualization with PlotJuggler ###
After launching, you can see the results of yout experiments running the following commands:

Start PlotJuggler:

```
rosrun plotjuggler plotjuggler
```

In case your image lacks this tool you can install it by typing:

```
sudo apt-get update
sudo apt-get install ros-melodic-plotjuggler-ros
```
From menu bar select Streaming > Start: ROS_Topic_Streamer. In pop-up menu that will appear choose /pose from available topic names and press ok.

By now you have the data in-stream.

By default you won't see any graphic, so you need a Layout. To load a layout press File->Layout (First icon)

![Image text](https://github.com/miggilcas/copter_model/blob/main/Images/plotjuggler_tutorial1.png)

Then select the folder: Layouts plotjuggler

If you want to see some examples we have done before, you can load them pressing File->Data:
![Image text](https://github.com/miggilcas/copter_model/blob/main/Images/plotjuggler_tutorial2.png)

In the folder called "Data plotjuggler" you can find those examples. 
To see the data plot in the layout, load the data, load the layout like is explained and finally select the time and press ok:
![Image text](https://github.com/miggilcas/copter_model/blob/main/Images/plotjuggler_tutorial3.png)

#### Adding gaussian noise ####
To add gaussian noise in your experiments you can modificate the parameter "/standar_deviation" typing:

```
rosparam set /standar_deviation <value>
```

To see the actual value of this parameter type:

```
rosparam get /standar_deviation
```

## Support
Our emails are:
	miguel.gil2000@gmail.com 	dameseck121200@gmail.com	ok.jjg.juanjose@gmail.com


## Authors and acknowledgment
This project is an university project developed by Miguel Gil Castilla, Dame Seck Diop and Juan Jos?? Garc??a Calder??n.

## License
Apache-2.0 License

## Project status
This project is not finished yet.

