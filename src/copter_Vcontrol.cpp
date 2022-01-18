/*
*   Nodo que recibira velocidades lineales y posicion en Z y realizara un 
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
#include <random>

//global variables
geometry_msgs::Twist latestVel;
geometry_msgs::Point latestPos;
geometry_msgs::Vector3 control;//Angle Control Actions
geometry_msgs::Vector3 error;//Control Errors
geometry_msgs::Vector3 RefVel;
geometry_msgs::Point wp;//Waypoints

std_msgs::Float64 W;
std_msgs::Float64 zerror;

//inclusion de ruido
unsigned seed;
float std_dev;

//funciones callback
void velCallback(const geometry_msgs::Twist::ConstPtr & message)
{
 // std::default_random_engine e(seed);
  
  latestVel=*message;//meter ruido
  //std::normal_distribution<double> noise(0,std_dev);
  //latestVel.x+=noise(e);
  //latestVel.y+=noise(e);
  //latestVel.z+=noise(e);
}
//obtiene las posiciones 
void poseCallback(const geometry_msgs::Point::ConstPtr & message)
{
  // std::default_random_engine e(seed);
  //std::normal_distribution<double> noise(0,std_dev);

  latestPos =*message;//meter ruido
  //latestPose.z+=noise(e);
}
void refCallback(const geometry_msgs::Vector3::ConstPtr & message)
{
  RefVel =*message;
}
void ArefCallback(const geometry_msgs::Point::ConstPtr & message)
{
  wp =*message;
}
int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "VControl");

  
  ros::NodeHandle n;
  //Publishers
  ros::Publisher ctrl_pub = n.advertise<geometry_msgs::Vector3>("copter_control/AngleControl", 100);
  ros::Publisher Alt_ctrl_pub = n.advertise<std_msgs::Float64>("copter_control/ZControl", 100);
  //ros::Publisher error_pub = n.advertise<geometry_msgs::Vector3>("copter_control/Errors", 100);
  ros::Publisher error_pub = n.advertise<std_msgs::Float64>("copter_control/zError", 100);

  //Subscribers
  ros::Subscriber Velsub = n.subscribe("copter_model/Vel", 1000, velCallback);
  ros::Subscriber Posesub = n.subscribe("copter_model/Pose", 1000, poseCallback);

  ros::Subscriber Vrefsub = n.subscribe("copter_control/Vref", 1000, refCallback);
  ros::Subscriber Arefsub = n.subscribe("copter_control/Waypoints", 100, ArefCallback);

  //Parameters
  //Parameters
  n.param<float>("standar_deviation", std_dev, 0.0);

  ROS_INFO("Node: 'VControl' ready");
  ros::Rate loop_rate(1000);//Frecuencia de 1 KHz
  //Ganancias para el control de los angulos
  float Kx=0.05;
  float Ky=Kx;
  float Kposx=0.5;
  //Ganancias para el control de altura, tal y como está controla en menos de 10 segundos
  float Kp=15;//4;//2;
  float Kd=50;//30;//15          
  //References:
  float RefVelx=0;//20;//-5
  float RefVely=0;//controla refs negativas y positivas
  float RefAltitude=-10;//para subir añadir distancia negativas
  
  //error:
  error.x=0;
  error.y=0;
  
  //Velocidad para vencer la gravedad:
  float b=0.00000254133;
  float Vmin=2*sqrt((0.095*9.8)/(2*b));//con m=0.28->Vmin=~1469.5; si usamos m=0.095->855.97
  double currentTime=0;
  double elapsedTime=0;
  double previousTime=(double)ros::Time::now().toSec();
   while (ros::ok())
	{
  //ruido
  seed=ros::Time::now().toSec();
  n.getParam("standar_deviation", std_dev);

  currentTime=(double)ros::Time::now().toSec();
	elapsedTime=(double)(currentTime-previousTime);
  //calculamos los parametros del control:
  /* Desacoplamos el control y hacemos un control P para cada accion de control
  *  Control en velocidad: cada angulo del Swash-plate sera proporcional al error de la velocidad en x o y
  *                        La velocidad angular de los rotores sera igual para ambos pero en funcion del error con
  *                        la altura.
  */
  //Referencia de altitud
  RefAltitude=wp.z;
  //Recibimos del Control en posicion
  RefVelx=RefVel.x;//Kposx*error.x;
  
  RefVely=RefVel.y;//Kposx*error.y;
  
  //Errores
  
  zerror.data=(-RefAltitude+latestPos.z);
  //pruebo un escalon para ver si se mantiene quieto en una determinada altura
 /* if(elapsedTime<=0){
    W.data=Vmin+30;//Ka*error.z +Vmin;//lo que se suma es la velocidad que vence la gravedad
    }
  else{
    W.data=Vmin;
  }
  //Prueba para el control de velocidad:
  if(elapsedTime<=5){//comprobación de cambio repentino de velocidad
  RefVelx=20;
    }
  else{
  //RefVelx=-20;
  
  }*/
  //if (error.z==0) cnt=cnt+1;
  /*if (error.z<=0.1)*/  
  
  //Para capturar todos los datos con plotjuggler no controlamos hasta el segundo 5
  if(elapsedTime<=5){
  //Control en velocidad
  control.x=0;
  control.y=0;
  control.z=0;

  //Control en altura
  W.data=Kp*zerror.data +Kd*(latestVel.linear.z)+Vmin;//Vmin;
  
    }
  else{
  //Control en velocidad
  control.x=Kx*(-RefVelx+latestVel.linear.x);
  control.y=Ky*(RefVely-latestVel.linear.y);
  control.z=0;

  //Control en altura
  W.data=Kp*zerror.data +Kd*(latestVel.linear.z)+Vmin;//es un control PV al incluir el adelanto al retardo que se ocasiona
  //al intentar vencer la gravedad
  
  
  }
  
  //Publicamos las acciones de control y los errores:
  ctrl_pub.publish(control);
  error_pub.publish(zerror);
  Alt_ctrl_pub.publish(W);

  ros::spinOnce();

	loop_rate.sleep();
}

  return 0;
}