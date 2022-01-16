#include "ros/ros.h"
#include "math.h"

#include "std_msgs/String.h"

#include "geometry_msgs/Vector3.h"
#include "geometry_msgs/Point.h"

/**
 * Este nodo es un generador de trayectorias simples para demostrar el funcionamiento 
 * de nuestro sistema siguiendo una trayectoria simple determinada por una cantidad
 * pequeña de Waypoints. 
 */
//global variables
geometry_msgs::Point Waypoint;//X Y Z references


int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "Traj_gen");

  
  ros::NodeHandle n;
  //publicara las referencias de posicion para hacer la trayectoria deseada
  ros::Publisher Traj_pub = n.advertise<geometry_msgs::Point>("copter_control/Waypoints", 100);

  ROS_INFO("Node: 'Traj_gen' ready");
  ros::Rate loop_rate(10);
  //variables para poder tener noción del tiempo
  double currentTime=0;
  double elapsedTime=0;
  double previousTime=(double)ros::Time::now().toSec();

   while (ros::ok())
	{
    //Conteo del tiempo
  currentTime=(double)ros::Time::now().toSec();
	elapsedTime=(double)(currentTime-previousTime);
  
  //vamos generando los waypoints en funcion del tiempo pasado
  if(elapsedTime<=10){//Primer punto
    Waypoint.x=0;
    Waypoint.y=0;
    Waypoint.z=0;
  }
  if(elapsedTime>=10 && elapsedTime<=20){//Primer punto
    Waypoint.x=0;
    Waypoint.y=0;
    Waypoint.z=-1;
  }
  if(elapsedTime>20 && elapsedTime<=40){//segundo
    Waypoint.x=1;
    Waypoint.y=0;
    Waypoint.z=-1;
  }
  if(elapsedTime>40 && elapsedTime<=60){//Tercero
    Waypoint.x=1;
    Waypoint.y=1;
    Waypoint.z=-1;
  }
  if(elapsedTime>60 && elapsedTime<=80){//Cuarto
    Waypoint.x=0;
    Waypoint.y=1;
    Waypoint.z=-1;
  }
  if(elapsedTime>80 && elapsedTime<=100){//Primero
    Waypoint.x=0;
    Waypoint.y=0;
    Waypoint.z=-1;
  }
  Traj_pub.publish(Waypoint);
 
  ros::spinOnce();

	loop_rate.sleep();
}

  return 0;
}
