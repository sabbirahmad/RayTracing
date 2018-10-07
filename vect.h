#pragma once
#ifndef _VECT_H
#define _VECT_H

#include <math.h>

class Vect{
	double x;
	double y;
	double z;

public: 
	Vect(){x=0;y=0;z=0;}
	Vect(double a, double b, double c){x=a;y=b;z=c;}
	//Vect(const Vect& v){x=v.x;y=v.y;z=v.z;}
	
	double getX() const {return x;}
	double getY() const {return y;}
	double getZ() const {return z;}

	void setX(double a) {x=a;}
	void setY(double b) {y=b;}
	void setZ(double c) {z=c;}
	void setXYZ(double a,double b,double c) {x=a;y=b;z=c;}
	void setXYZ(Vect v) {x=v.x; y=v.y; z=v.z;}

	void operator=(const Vect& v){
		  this->setX(v.getX());
		  this->setY(v.getY());
		 this->setZ(v.getZ());
    }

	double magnitude(){
		double mag=sqrt(x*x + y*y + z*z);
		return mag;
	}

	Vect normalize(){
		double mag=this->magnitude();
		Vect norm(x/mag, y/mag, z/mag);
		return norm;
	}

	double dotProduct(Vect v){
		double dp;
		dp=x*v.x + y*v.y + z*v.z;
		return dp;
	}

	Vect crossProduct(Vect v){
		Vect cp((y*v.z-z*v.y),(z*v.x-x*v.z),(x*v.y-y*v.x));
		return cp;
	}

	Vect negative(){
		Vect neg(-x,-y,-z);
		return neg;
	}

	Vect addVect(Vect v){
		Vect add(x+v.x, y+v.y, z+v.z);
		return add;
	}

	Vect subVect(Vect v){
		Vect sub(x-v.x, y-v.y, z-v.z);
		return sub;
	}

	Vect multVect(Vect v){
		Vect mult(x*v.x, y*v.y, z*v.z);
		return mult;
	}

	Vect multVectScaler(double sc){
		Vect v(x*sc, y*sc, z*sc);
		return v;
	}

	double pointToPointDistance(Vect v){
		double d=(x-v.x)*(x-v.x)+(y-v.y)*(y-v.y)+(z-v.z)*(z-v.z);
		return sqrt(d);
	}
};

#endif