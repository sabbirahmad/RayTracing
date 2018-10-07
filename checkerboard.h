#ifndef _CHECKERBOARD_H
#define _CHECKERBOARD_H

#include <cstring>
#include "shape.h"
#include "color.h"
#include "vect.h"
#include "ray.h"
#include <iostream>
using namespace std;

class Checkerboard:public Shape{
//public:
	int boxSize;
	Color colorOne;
	Color colorTwo;
	double distance;
	Vect normal;

public:
	Checkerboard(){boxSize=20;colorOne.setRGB(1,1,1); colorTwo.setRGB(0,0,0); normal.setXYZ(0,1,0);distance=0;};
	Checkerboard(char nm[20], double ac, double dc, double rc, double sc, double se, Color cl1, Color cl2):Shape(nm,ac,dc,rc,sc,se){
		boxSize=20;
		colorOne=cl1;
		colorTwo=cl2;
		normal.setXYZ(0,1,0);
		distance=0;
	}

	/*
	virtual Color getColor(int c) const {
		if(c==1)
			return colorOne;
		else{
			return colorTwo;
		}
	}*/
	Color getColor(Ray ray, double t, vector<Light> light, vector<Shape*> shape, int currentShape,int recDepthNow) const {
		/*Vect rayDirection=ray.getDirection();
		Vect rayOrigin=ray.getOrigin();

		double px=rayOrigin.getX()+t*rayDirection.getX();
		double py=rayOrigin.getY()+t*rayDirection.getY();
		double pz=rayOrigin.getZ()+t*rayDirection.getZ();
		*/

		Vect point;
		point.setXYZ(Shape::getCoordFromRay(ray,t));
		double px=point.getX();
		double py=point.getY();
		double pz=point.getZ();

		int xBox=abs((int) floor((px/boxSize)));
		int zBox=abs((int) floor((pz/boxSize)));

		if((xBox+zBox)%2==0){
			Color cl=Shape::getLightIntensity(ray,t,light,shape,currentShape,colorOne,recDepthNow);
			return cl;
			//return colorOne;
		}
		else{
			Color cl=Shape::getLightIntensity(ray,t,light,shape,currentShape,colorTwo,recDepthNow);
			return cl;
			//return colorTwo;
		}
	}

	double getDistance() const {return distance;}
	
	Color getColorOne() const {return colorOne;}
	Color getColorTwo() const {return colorTwo;}

	void setColorOne(Color cl1) {colorOne=cl1;}
	void setColorTwo(Color cl2) {colorTwo=cl2;}

	void operator=(const Checkerboard& ck){
		strcpy(this->name,ck.name);
		this->boxSize=ck.boxSize;
		this->ambCoeff=ck.ambCoeff;
		this->difCoeff=ck.difCoeff;
		this->refCoeff=ck.refCoeff;
		this->specCoeff=ck.specCoeff;
		this->specExp=ck.specExp;
		//this->setColorOne(ck.getColor(1));
		//this->setColorTwo(ck.getColor(2));
		this->setColorOne(ck.getColorOne());
		this->setColorTwo(ck.getColorTwo());
		this->normal=ck.normal;
		this->distance=ck.distance;
	}

	double findIntersection(Ray ray){ //return t value of O+td of ray
		//cout<<"checkerboard intersection"<<endl;
		Vect rayDirection=ray.getDirection();
		double intersect=rayDirection.dotProduct(normal);
		if(intersect==0){ //ray parallel to plane
			return -1;
		}

		double a=ray.getOrigin().dotProduct(normal) + distance;
		double b=rayDirection.dotProduct(normal);

		double t=(-1*a)/b;
		if(t<0){
			return -1;
		}
		else{ 
			return t-EPSILON;
		}
	}

	Vect getNormalAt(Vect point, Ray ray){
		return normal;
	}

	Ray getReflectedRayAt(Ray ray, Vect point){
		Vect norm,refRayDir;
		norm.setXYZ(getNormalAt(point,ray));
		refRayDir.setXYZ(ray.getDirection().subVect(norm.multVectScaler(2*ray.getDirection().dotProduct(norm))));
		Ray reflectedRay(point,refRayDir);

		return reflectedRay;
	}

	bool isRefractive(){
		return false;
	}

	Ray getRefractedRay(Ray ray, Vect point){
		return Ray(Vect(0,0,0),Vect(0,0,0));
	}

	double getRefractiveIndex(){
		return 0;
	}
	
	//~Checkerboard();
};

#endif