/*
*   Nodo que recibira posiciones x e y para mandar referencias de velocidad 
*   al control de velocidad.
*   Se subscribira al topic de los waypoints generados por el generador
*   simple de trayectorias.
*
*/

#include "ros/ros.h"
#include <math.h>

#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Vector3.h"
#include "std_msgs/Float64.h"

#include <iostream>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <sstream>

//global variables
geometry_msgs::Point latestPos;
geometry_msgs::Vector3 control;//Angle Control Actions
geometry_msgs::Vector3 error;//Position Errors
geometry_msgs::Vector3 RefVel;//Velocity Reference
geometry_msgs::Point wp;//Waypoints



void poseCallback(const geometry_msgs::Point::ConstPtr & message)
{
  latestPos =*message;
}
void wpCallback(const geometry_msgs::Point::ConstPtr & message)
{
  wp =*message;
}
int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "PControl");

  
  ros::NodeHandle n;
  
  ros::Publisher VelRef_pub = n.advertise<geometry_msgs::Vector3>("copter_control/Vref", 100);
  ros::Publisher error_pub = n.advertise<geometry_msgs::Vector3>("copter_control/Errors", 100);

  
  ros::Subscriber Posesub = n.subscribe("copter_model/Pose", 1000, poseCallback);
  ros::Subscriber wpsub = n.subscribe("copter_control/Waypoints", 100, wpCallback);
  ROS_INFO("Node: 'PControl' ready");
  ros::Rate loop_rate(100);//100 Hz
  //Ganancias 
  float Kpos=0.5;
  //Ganancias para el control de altura
  float Kp=4;//2;
  float Kd=30;//15          
  //References:
  float Refx=-1,Refy=0.5,Refz,errorx,cnt;
  //error:
  error.x=0;
  error.y=0;
  
  double currentTime=0;
  double elapsedTime=0;
  double previousTime=(double)ros::Time::now().toSec();
   while (ros::ok())
	{
  currentTime=(double)ros::Time::now().toSec();
	elapsedTime=(double)(currentTime-previousTime);
  //calculamos los parametros del control:
  /* Desacoplamos el control y hacemos un control P para cada accion de control
  *  Control en velocidad: cada angulo del Swash-plate sera proporcional al error de la velocidad en x o y
  *                        La velocidad angular de los rotores sera igual para ambos pero en funcion del error con
  *                        la altura.
  */
  //Referencias
  Refx=wp.x;
  Refy=wp.y;
  //Errores
  error.x=(Refx-latestPos.x);
  error.y=(Refy-latestPos.y);
    
  //Control en posicion
  RefVel.x=Kpos*error.x;
  //Saturamos la velocidad para evitar desestabilizar el sistema
  if (RefVel.x<=-15) RefVel.x=-15;
  if (RefVel.x>=15) RefVel.x=15;
  
  
  RefVel.y=Kpos*error.y;
  if (RefVel.y<=-15) RefVel.y=-15;
  if (RefVel.y>=15)  RefVel.y=15;
  
  //Publicamos las referencias para el control en velocidad y los errores en posicion:
  VelRef_pub.publish(RefVel);
  error_pub.publish(error);

  ros::spinOnce();

	loop_rate.sleep();
}

  return 0;
}