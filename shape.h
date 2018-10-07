#ifndef _SHAPE_H
#define _SHAPE_H

#include <cstring>
#include "color.h"
#include "ray.h"
#include "light.h"
#include <iostream>
#include "vect.h"
using namespace std;

extern const double EPSILON=0.005;

class Shape{
//protected:
public:
	char name[20];
	double ambCoeff;
	double difCoeff;
	double refCoeff;
	double specCoeff;
	double specExp;

public:
	Shape(){
		strcpy(name,"");
		ambCoeff=0;
		difCoeff=0;
		refCoeff=0;
		specCoeff=0;
		specExp=0;
	}
	Shape(char nm[20], double ac, double dc, double rc, double sc, double se){
		//cout<<"in SHapeSphere"<<endl;
		strcpy(name,nm);
		ambCoeff=ac;
		difCoeff=dc;
		refCoeff=rc;
		specCoeff=sc;
		specExp=se;
		//cout<<ambCoeff<<" "<<difCoeff<<" "<<specCoeff<<endl;
	}
	char* getName(){return name;}
	virtual double findIntersection(Ray ray) {cout<<"base ";return -1;}
	virtual Color getColor(Ray ray, double t, vector<Light> light, vector<Shape*> shape,int currentShape, int recDepthNow) const {
		Color c;
		return c;
	}
	//virtual double getLightIntensity(Ray ray, double t, vector<Light> light, vector<Shape*> shape) {return -1;}

	Vect getCoordFromRay(Ray ray, double t) const {
		Vect rayDirection=ray.getDirection();
		Vect rayOrigin=ray.getOrigin();

		double px=rayOrigin.getX()+t*rayDirection.getX();
		double py=rayOrigin.getY()+t*rayDirection.getY();
		double pz=rayOrigin.getZ()+t*rayDirection.getZ();

		return Vect(px,py,pz);
	}

	Color getLightIntensity(Ray ray, double t, vector<Light> light, vector<Shape*> shape, int currentShape, Color pixColor, int recDepthNow) const {
		Shape* coloredShape;
		coloredShape=shape[currentShape];

		double red=pixColor.getRed();
		double green=pixColor.getGreen();
		double blue=pixColor.getBlue();

		double redIn,greenIn,blueIn;

		Vect point;
		point.setXYZ(Shape::getCoordFromRay(ray,t-EPSILON));

		int shapeN=shape.size();

		//double totalLighting=1.0*(this->ambCoeff);

		redIn=red*(coloredShape->ambCoeff);
		greenIn=green*(coloredShape->ambCoeff);
		blueIn=blue*(coloredShape->ambCoeff);

		int noLights=light.size();
		for(int i=0;i<noLights;i++){
			bool noIntersection=false;
			Vect lightPos;
			Light lig=light[i];
			lightPos.setXYZ(lig.getPosition());
			Vect lightRayDirecton;
			lightRayDirecton.setXYZ(lightPos.subVect(point));
			//Vect lightRayOrigin;
			//lightRayOrigin.setXYZ(correctEpsilon(lightRayDirecton,point));
			Ray lightRay(point,lightRayDirecton);
			//Ray lightRay(lightRayOrigin,lightRayDirecton); //new point origin

			//intersection with lightray... if intersect then so lighting for this light
			Shape* sh;
			double minIntersect=999999;
			double intersect;
			int index=-1;
			for(int object=0;object<shapeN;object++){
				//cout<<"shape "<<object<<endl;
				sh=shape[object];
				//cout<<"shape assigned"<<object<<endl;
				intersect=sh->findIntersection(lightRay);
				//cout<<"intersect: "<<intersect<<endl;
				if(intersect<0){
					continue;
				}
				if(intersect<minIntersect){
					minIntersect=intersect;
					index=object;
					//cout<<"minIntersect: "<<intersect<<endl;
				}
			}
			if(index==-1){//no intersection
				//calculate lighting
				noIntersection=true;
			}
			else{
				Vect inPoint=Shape::getCoordFromRay(lightRay,minIntersect);
				double inDis=point.pointToPointDistance(inPoint); //distance of intersection-point to point on light object
				double lightDis=point.pointToPointDistance(lightPos);

				if(inDis>lightDis){//calculate lighting
					noIntersection=true;
				}

			}
			if(noIntersection){//if no intersection calculate lighting: ray hits light
				//lambert
				//cout<<"noIntersection"<<endl;
				double lambert=lightRay.getDirection().dotProduct(coloredShape->getNormalAt(point,ray));//only normal is different among shapes
				lambert=lambert<=0?0:lambert;
	
				//phong-blinn
				Vect h;
				h.setXYZ(lightRay.getDirection().addVect(ray.getDirection().negative()));
				h.setXYZ(h.normalize());

				
				Vect norm;
				norm.setXYZ(coloredShape->getNormalAt(point,ray).normalize());
				double phong=h.dotProduct(norm);
				phong=phong<=0?0:phong;

				/*if(currentShape==1)
				{	
					cout<<"triangle: "<<endl;
					if((lambert<=0 || phong<=0)){
						cout<<"less"<<endl;
					}
				}
				*/

				//cout<<ambCoeff<<" "<<difCoeff<<" "<<specCoeff<<endl;
				
				//double lightingVal= 1.0*(coloredShape->difCoeff)*lambert + 1.0*(coloredShape->specCoeff)*pow(phong,coloredShape->specExp);
				double difLightingVal= 1.0*(coloredShape->difCoeff)*lambert;
				double specLightingVal= 1.0*(coloredShape->specCoeff)*pow(phong,coloredShape->specExp);

				/*
				redIn+=red*(lightingVal/(noLights*1.0));
				greenIn+=green*(lightingVal/(noLights*1.0));
				blueIn+=blue*(lightingVal/(noLights*1.0));
				*/

				redIn+=(red*(difLightingVal/(noLights*1.0)));
				greenIn+=(green*(difLightingVal/(noLights*1.0)));
				blueIn+=(blue*(difLightingVal/(noLights*1.0)));

				redIn+=(specLightingVal/(noLights*1.0));
				greenIn+=(specLightingVal/(noLights*1.0));
				blueIn+=(specLightingVal/(noLights*1.0));
				

				//totalLighting+=(lightingVal/(noLights*1.0));
			}
		}
		
		
		//reflection color
		double R=1;
		double redRef=0;
		double greenRef=0;
		double blueRef=0;
		Color refColor(0,0,0);
		Color refracColor(0,0,0);
		Vect refOrigin;
		Ray reflectedRay=coloredShape->getReflectedRayAt(ray,point);
		//refOrigin.setXYZ(correctEpsilon(reflectedRay.getDirection(),point));//new linese
		//reflectedRay.setRay(refOrigin,reflectedRay.getDirection());//new lines

		bool reflection=false;
		bool refraction=false;
		//check intersection for reflected ray

		if(recDepthNow>1){
			//cout<<"recdepthnow: "<<recDepthNow<<endl;
			Shape* sh;
			double minIntersect=999999;
			double intersect;
			int index=-1;
			int object;
			for(object=0;object<shapeN;object++){
				if(object==currentShape){
					continue;
				}
				//cout<<"shape "<<object<<endl;
				sh=shape[object];
				//cout<<"shape assigned"<<object<<endl;
				intersect=sh->findIntersection(reflectedRay);
				//cout<<"intersect: "<<intersect<<endl;
				if(intersect<0){
					continue;
				}
				if(intersect<minIntersect){
					minIntersect=intersect;
					index=object;
					/*if(index==currentShape){
						cout<<"reflected intersects same obj: "<<index<<endl;
					}*/
					//cout<<"minIntersect: "<<intersect<<endl;
				}
			}
			
			if(index!=-1){//intersection occurred
				//cout<<"intersection reflected"<<endl;
				reflection=true;
				/*if(currentShape==0){
					cout<<"current: "<<currentShape<<"index: "<<index<<endl;
				}*/
				sh=shape[index];
				refColor=sh->getColor(reflectedRay,minIntersect,light,shape,index,recDepthNow-1);	
			}


			//refraction
			if(coloredShape->isRefractive()){

				//cout<<"refraction"<<endl;
				
				double n1=1;
				double n2=coloredShape->getRefractiveIndex();
				//cout<<"n2: "<<n2<<endl;
				double R0=pow((n1-n2)/(n1+n2),2);
				
				double cos0i=0;
				double cos0f=0;
				Ray refractedRay=coloredShape->getRefractedRay(ray,point);
				cos0i=abs(coloredShape->getCos0i());
				cos0f=abs(coloredShape->getCos0f());
				//cout<<cos0i<<"\t"<<cos0f<<endl;
				if(n1<=n2){
					R=R0+(1-R0)*pow((1-cos0i),5);
				}
				else{
					R=R0+(1-R0)*pow((1-cos0f),5);
				}

				//cout<<"R: "<<R<<endl;
				//check refracted ray intersection

				minIntersect=999999;
				index=-1;
				for(int object=0;object<shapeN;object++){
					if(object==currentShape){
						continue;
					}
					//cout<<"shape "<<object<<endl;
					sh=shape[object];
					//cout<<"shape assigned"<<object<<endl;
					intersect=sh->findIntersection(refractedRay);
					//cout<<"intersect: "<<intersect<<endl;
					if(intersect<0){
						continue;
					}
					if(intersect<minIntersect){
						minIntersect=intersect;
						index=object;
						/*if(index==currentShape){
							cout<<"refracted intersects same obj: "<<index<<endl;
						}*/
						//cout<<"minIntersect: "<<intersect<<endl;
					}
				}
				if(index!=-1){//intersection occurred
					refraction=true;
					//cout<<"refraction "<<endl;
					sh=shape[index];
					refracColor=sh->getColor(refractedRay,minIntersect,light,shape,index,recDepthNow-1);	
				}
			}
		}

		//if(reflection==true || refraction==true){
		/*if(R!=1){
			cout<<"R: "<<R<<endl;
		}*/
			/*R+=0.4;
			redRef=R*((coloredShape->refCoeff) * refColor.getRed()+redIn) + (1-R)*refracColor.getRed() ;
			greenRef=R*((coloredShape->refCoeff) * refColor.getGreen()+greenIn) + (1-R)*refracColor.getGreen();
			blueRef=R*((coloredShape->refCoeff) * refColor.getBlue()+blueIn) + (1-R)*refracColor.getBlue();*/

			double redC=R*((coloredShape->refCoeff) * refColor.getRed()) + (1-R)*refracColor.getRed();
			double greenC=R*((coloredShape->refCoeff) * refColor.getGreen()) + (1-R)*refracColor.getGreen();
			double blueC=R*((coloredShape->refCoeff) * refColor.getBlue()) + (1-R)*refracColor.getBlue();
        
        redRef=(coloredShape->refCoeff) * redC;
        greenRef=(coloredShape->refCoeff) * greenC;
        blueRef=(coloredShape->refCoeff) * blueC;
		//}

		//return totalLighting;
		return Color(redIn+redRef,greenIn+greenRef,blueIn+blueRef);
		//return Color(redRef,greenRef,blueRef);
	}

	virtual Vect getNormalAt(Vect point, Ray ray){
		return Vect(0,0,0);
	}

	virtual Ray getReflectedRayAt(Ray ray, Vect point){
		return Ray(Vect(0,0,0),Vect(0,0,0));
	}

	Vect correctEpsilon(Vect direction, Vect point) const {
		Vect dir;
		dir.setXYZ(direction.normalize());
		Vect newPoint;
		newPoint.setXYZ(point.addVect(dir.multVectScaler(EPSILON)));

		return newPoint;
	}

	virtual bool isRefractive(){
		return false;
	}

	virtual Ray getRefractedRay(Ray ray, Vect point){
		return Ray(Vect(0,0,0),Vect(0,0,0));
	}

	virtual double getRefractiveIndex(){
		return 0;
	}

	virtual double getCos0i(){
		return 0;
	}

	virtual double getCos0f(){
		return 0;
	}

	//~Shape();
};

#endif