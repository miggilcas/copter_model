#include "ros/ros.h"
#include "math.h"

#include "std_msgs/String.h"

#include "geometry_msgs/Vector3.h"
#include "geometry_msgs/Point.h"

/**
 * Este nodo es un generador de trayectorias simple para demostrar el funcionamiento 
 * de nuestro sistema siguiendo una trayectoria muy sencilla determinada por una cantidad
 * pequeña de Waypoints. 
 * Realizaremos 4 tipos de experimentos, llamados por los diferentes launch:
 *    +Hovering
 *    +Square
 *    +2 altitudes Square
 *    +4 altitudes Square
 */
//global variables
geometry_msgs::Point Waypoint;//X Y Z references
int n_exp;

int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "Traj_gen");

  
  ros::NodeHandle n;
  //publicara las referencias de posicion para hacer la trayectoria deseada
  ros::Publisher Traj_pub = n.advertise<geometry_msgs::Point>("copter_control/Waypoints", 100);

  //Parameters
  n.param<int>("experiment", n_exp, 0); // se definirá en el launch

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
  switch(n_exp){
  case 0://Hovering
    if(elapsedTime<10){//Punto de partida
    Waypoint.x=0;
    Waypoint.y=0;
    Waypoint.z=0;
  }
  else{//Primer punto y último
    Waypoint.x=0;
    Waypoint.y=0;
    Waypoint.z=-5;
  }
  break;
  //vamos generando los waypoints en funcion del tiempo pasado
  case 1: //cuadrado de 1 metro de altura y 1 metro de lado
  if(elapsedTime<=10){//Punto inicial
    Waypoint.x=0;
    Waypoint.y=0;
    Waypoint.z=0;
  }
  if(elapsedTime>10 && elapsedTime<=20){//Primer vertice
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
  break;

  case 2://cuadrado con dos vértices más elevados
    if(elapsedTime<=10){//Punto inicial
    Waypoint.x=0;
    Waypoint.y=0;
    Waypoint.z=0;
  }
  if(elapsedTime>=10 && elapsedTime<=20){//Primer vertice
    Waypoint.x=0;
    Waypoint.y=0;
    Waypoint.z=-5;
  }
  if(elapsedTime>20 && elapsedTime<=40){//segundo
    Waypoint.x=2;
    Waypoint.y=0;
    Waypoint.z=-5;
  }
  if(elapsedTime>40 && elapsedTime<=60){//Tercero
    Waypoint.x=2;
    Waypoint.y=2;
    Waypoint.z=-7;
  }
  if(elapsedTime>60 && elapsedTime<=80){//Cuarto
    Waypoint.x=0;
    Waypoint.y=2;
    Waypoint.z=-7;
  }
  if(elapsedTime>80 && elapsedTime<=100){//Primero
    Waypoint.x=0;
    Waypoint.y=0;
    Waypoint.z=-5;
  }
  break;
  case 3://cuatro vértices de alturas distintas todas
    if(elapsedTime<=10){//Punto de partida
    Waypoint.x=0;
    Waypoint.y=0;
    Waypoint.z=0;
  }
  if(elapsedTime>=10 && elapsedTime<=20){//Primer punto
    Waypoint.x=0;
    Waypoint.y=0;
    Waypoint.z=-5;
  }
  if(elapsedTime>20 && elapsedTime<=40){//segundo
    Waypoint.x=3;
    Waypoint.y=0;
    Waypoint.z=-10;
  }
  if(elapsedTime>40 && elapsedTime<=60){//Tercero
    Waypoint.x=3;
    Waypoint.y=3;
    Waypoint.z=-3;
  }
  if(elapsedTime>60 && elapsedTime<=80){//Cuarto
    Waypoint.x=0;
    Waypoint.y=3;
    Waypoint.z=-8;
  }
  if(elapsedTime>80 && elapsedTime<=100){//Primero
    Waypoint.x=0;
    Waypoint.y=0;
    Waypoint.z=-2;
  }
  break;
  }

  Traj_pub.publish(Waypoint);
 
  ros::spinOnce();

	loop_rate.sleep();
}

  return 0;
}
