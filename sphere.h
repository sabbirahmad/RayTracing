#ifndef _SPHERE_H
#define _SPHERE_H

#include <cstring>
#include <math.h>
#include "shape.h"
#include "color.h"
#include "vect.h"
#include "ray.h"
#include "light.h"


class Sphere:public Shape{
public:
	Vect center;
	double radius;
	Color color;

public:
	Sphere();
	Sphere(char nm[20], double ac, double dc, double rc, double sc, double se, Vect c, double r, Color cl):Shape(nm,ac,dc,rc,sc,se){
		//Shape(nm,ac,dc,rc,sc,se);
		radius=r;
		center=c;
		color=cl;
	}

	Color getColor(Ray ray, double t, vector<Light> light, vector<Shape*> shape, int currentShape,int recDepthNow) const {
		Color cl=Shape::getLightIntensity(ray,t,light,shape,currentShape,color,recDepthNow);
		//return color;
		return cl;
	}
	Color getColor() const {return color;}
	double getRadius() const {return radius;}
	Vect getCenter() const {return center;}
	void setColor(Color cl){color=cl;}

	void operator=(const Sphere& sp){
		 strcpy(this->name,sp.name);
		 this->ambCoeff=sp.ambCoeff;
		 this->center=sp.center;
		 this->color=sp.color;
		 this->difCoeff=sp.difCoeff;
		 this->radius=sp.radius;
		 this->refCoeff=sp.refCoeff;
		 this->specCoeff=sp.specCoeff;
		 this->specExp=sp.specExp;
      }

	double findIntersection(Ray ray){
		Vect ori=ray.getOrigin();
		Vect d=ray.getDirection();

		Vect oc;
		oc.setXYZ(ori.subVect(center));

		double b=2*d.dotProduct(oc);
		double c=oc.dotProduct(oc) - radius*radius;

		double det=b*b-4*c;	//b^2-4*a*c  here a=1
		if(det<0){
			return -1;
		}
		else if(det==0){
			return -b/2-EPSILON;
		}
		else{
			double t1=(-b-sqrt(det))/2;
			double t2=(-b+sqrt(det))/2;
			if(t1<0 && t2<0){//no need though
				return -1;
			}
			else if(t1<0 || t2<0){
				double intersectPoint=(t1<t2?t2:t1);
				return intersectPoint;
			}
			double intersectPoint=(t1<t2?t1:t2);
			return intersectPoint-EPSILON;
		}
	}

	Vect getNormalAt(Vect point, Ray ray){
		//cout<<"shhere normal"<<endl;
		Vect normal;
		normal.setXYZ(point.subVect(center).normalize());
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
	
	//~Sphere();
};

#endif