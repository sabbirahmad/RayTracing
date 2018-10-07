#ifndef _CYLINDER_H
#define _CYLINDER_H

#include <cstring>
#include "shape.h"
#include "color.h"
#include <iostream>
using namespace std;

class Cylinder:public Shape{
//public:
	double xCenter;
	double zCenter;
	double radius;
	double yMin;
	double yMax;
	Color color;
public:
	Cylinder();
	Cylinder(char nm[20], double ac, double dc, double rc, double sc, double se, double xC, double zC, double r, double yMn, double yMx, Color cl):Shape(nm,ac,dc,rc,sc,se){
		xCenter=xC;
		zCenter=zC;
		radius=r;
		yMin=yMn;
		yMax=yMx;
		color=cl;
	}

	Color getColor(Ray ray, double t, vector<Light> light, vector<Shape*> shape, int currentShape,int recDepthNow) const {
		Color cl=Shape::getLightIntensity(ray,t,light,shape,currentShape,color,recDepthNow);
		return cl;
		//return color;
	}
	Color getColor() const {return color;}
	double getXCenter() const {return xCenter;}
	double getZCenter() const {return zCenter;}
	double getRadius() const {return radius;}
	double getYMin() const {return yMin;}
	double getYMax() const {return yMax;}
	void setColor(Color cl) {color=cl;}

	void operator=(const Cylinder& cl){
		strcpy(this->name,cl.name);
		this->ambCoeff=cl.ambCoeff;
		this->difCoeff=cl.difCoeff;
		this->refCoeff=cl.refCoeff;
		this->specCoeff=cl.specCoeff;
		this->specExp=cl.specExp;
		this->xCenter=cl.xCenter;
		this->zCenter=cl.zCenter;
		this->radius=cl.radius;
		this->yMin=cl.yMin;
		this->yMax=cl.yMax;
		this->color=cl.color;
	}
	double findIntersection(Ray ray){
		double t=-1;
		double dx=ray.getDirection().getX();
		double dy=ray.getDirection().getY();
		double dz=ray.getDirection().getZ();
		double ox=ray.getOrigin().getX();
		double oy=ray.getOrigin().getY();
		double oz=ray.getOrigin().getZ();

		double a=dx*dx+dz*dz;
		double b=2*(dx*(ox-xCenter)+dz*(oz-zCenter));
		double c=(ox-xCenter)*(ox-xCenter)+(oz-zCenter)*(oz-zCenter)-radius*radius;

		double det=b*b-4*a*c;

		if(a==0 || det<0){
			t=-1;
		}
		else{
			double t1=(-b-sqrt(det))/(2*a);
			double t2=(-b+sqrt(det))/(2*a);
			/*if(t1<0 && t2<0){//no need though
				return -1;
			}
			else if(t1<0 || t2<0){
				t=(t1<t2?t2:t1);
			}
			else{
				t=(t1<t2?t1:t2);
			}*/
			double y1=oy+t1*dy;
			double y2=oy+t2*dy;

			if((yMin<=y1 && y1<=yMax) && (yMin<=y2 && y2<=yMax)){
				t=(t1<t2?t1:t2);
			}
			else if(yMin<=y1 && y1<=yMax){
				t=t1;
			}
			else if(yMin<=y2 && y2<=yMax){
				t=t2;
			}
		}
		
		//upper lid
		double t1=(yMax-oy)/dy;
		if(t1>=0){
			Vect P;
			P.setXYZ(ray.getOrigin().addVect(ray.getDirection().multVectScaler(t1)));
			double dis=P.pointToPointDistance(Vect(xCenter,yMax,zCenter));
			if(dis<=radius){
				if(t!=-1){
					t=t1<t?t1:t;
				}
				else{
					t=t1;
				}
			}

		}

		//lower lid

		double t2=(yMin-oy)/dy;
		if(t2>=0){
			Vect P;
			P.setXYZ(ray.getOrigin().addVect(ray.getDirection().multVectScaler(t2)));
			double dis=P.pointToPointDistance(Vect(xCenter,yMin,zCenter));
			if(dis<=radius){
				if(t!=-1){
					t=t2<t?t2:t;
				}
				else{
					t=t2;
				}
			}

		}
		

		return t-EPSILON;
	}

	Vect getNormalAt(Vect point, Ray ray){
		double py=point.getY();
		double absDiff1=abs(py-yMax);
		double absDiff2=abs(py-yMin);

		if(absDiff1<0.005){
			//cout<<"yMax"<<endl;
			return Vect(0,1,0);
		}
		else if(absDiff2<0.005){
			//cout<<"yMin"<<endl;
			return Vect(0,-1,0);
		}

		Vect norm(point.getX()-xCenter,0,point.getZ()-zCenter);
		norm.normalize();
		return norm;
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
	
	//~Cylinder();
};


#endif