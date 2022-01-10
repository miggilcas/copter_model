/*
*   Nodo que recibirá velocidades lineales y posicion en Z y realizara un 
*   control desacoplado por simpleza a través de un P.
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
geometry_msgs::Twist latestVel;
geometry_msgs::Point latestPos;
geometry_msgs::Vector3 control;//Angle Control Actions
geometry_msgs::Vector3 error;//Angle Control Actions

std_msgs::Float64 W;

void velCallback(const geometry_msgs::Twist::ConstPtr & message)
{
  latestVel=*message;
}
void poseCallback(const geometry_msgs::Point::ConstPtr & message)
{
  latestPos =*message;
}
int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "Control");

  
  ros::NodeHandle n;
  
  ros::Publisher ctrl_pub = n.advertise<geometry_msgs::Vector3>("copter_model/AngleControl", 100);
  ros::Publisher Alt_ctrl_pub = n.advertise<std_msgs::Float64>("copter_model/ZControl", 100);
  ros::Publisher error_pub = n.advertise<geometry_msgs::Vector3>("copter_model/Errors", 100);

  ros::Subscriber Velsub = n.subscribe("copter_model/Vel", 1000, velCallback);
  ros::Subscriber Posesub = n.subscribe("copter_model/Pose", 1000, poseCallback);
 
  ROS_INFO("Node: 'Control' ready");
  ros::Rate loop_rate(1000);//1000 veces por segundo
  float K=10;
  float Kp=2;
  float Kd=15;          
  //References:
  float RefVelx=1;
  float RefVely=1;
  float RefAltitude=-20;//para subir un metro
  //error:
  error.x=0;
  error.y=0;
  
  //Velocidad para vencer la gravedad:
  float b=0.00000254133;
  float Vmin=2*sqrt((0.28*9.8)/(2*b));//1469.5;
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
  control.x=K*(RefVelx-latestVel.linear.x);
  control.y=K*(RefVely-latestVel.linear.y);
  control.z=0;

  //Altitud
  error.z=(-RefAltitude+latestPos.z);
  //pruebo un escalon para ver si se mantiene quieto en una determinada altura
 /* if(elapsedTime<=30){
    W.data=Vmin+30;//Ka*error.z +Vmin;//lo que se suma es la velocidad que vence la gravedad
    }
  else{
    W.data=Vmin;
  }*/
  W.data=Kp*error.z +Kd*(latestVel.linear.z)+Vmin;
  //if(W.data<=1469.5)W.data=1469.5;
  //Publicamos las acciones de control y los errores:
  ctrl_pub.publish(control);
  error_pub.publish(error);
  Alt_ctrl_pub.publish(W);

  ros::spinOnce();

	loop_rate.sleep();
}

  return 0;
}