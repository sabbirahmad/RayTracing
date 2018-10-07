#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include <cstring>
#include "shape.h"
#include "color.h"
#include "vect.h"

class Triangle:public Shape{
//public:
	Vect a;
	Vect b;
	Vect c;
	Color color;
	double refractiveIndex;
	double cos0i;
	double cos0f;
public:
	Triangle();
	Triangle(char nm[20], double ac, double dc, double rc, double sc, double se, Vect x, Vect y, Vect z, Color cl, double rI):Shape(nm,ac,dc,rc,sc,se){
		refractiveIndex=rI;
		a=x;
		b=y;
		c=z;
		color=cl;
		//cout<<"triangle cl: "<<color.getRed()<<" "<<color.getGreen()<<" "<<color.getBlue()<<endl;
		cos0i=0;
		cos0i=0;
	}

	Color getColor(Ray ray, double t, vector<Light> light, vector<Shape*> shape, int currentShape,int recDepthNow) const {
		Color cl=Shape::getLightIntensity(ray,t,light,shape,currentShape,color,recDepthNow);
		/*if(recDepthNow==2){
			cout<<"triangle cl: "<<cl.getRed()<<" "<<cl.getGreen()<<" "<<cl.getBlue()<<endl;
		}*/
		return cl;
		//return color;
	}
	Color getColor() const {return color;}
	Vect getA() const {return a;}
	Vect getB() const {return b;}
	Vect getC() const {return c;}
	void setColor(Color cl) {color=cl;}

	void operator=(const Triangle& tr){
		strcpy(this->name,tr.name);
		this->ambCoeff=tr.ambCoeff;
		this->difCoeff=tr.difCoeff;
		this->refCoeff=tr.refCoeff;
		this->specCoeff=tr.specCoeff;
		this->specExp=tr.specExp;
		this->refractiveIndex=tr.refractiveIndex;
		this->a=tr.a;
		this->b=tr.b;
		this->c=tr.c;
		this->color=tr.color;
	}

	Vect getNormal(){
		Vect ab,ac;
		ab.setXYZ(a.subVect(b));
		ac.setXYZ(a.subVect(c));
		Vect n;
		n.setXYZ(ab.crossProduct(ac).normalize());
		return n;
	}

	double findIntersection(Ray ray){
		//find intersection
		Vect rayDirection=ray.getDirection();
		Vect normal;
		normal.setXYZ(getNormal());
		double intersect=rayDirection.dotProduct(normal);
		if(intersect==0){//ray parallel to plane
			return -1;
		}

		double distance=-1*(c.dotProduct(normal));
		double aVal=ray.getOrigin().dotProduct(normal) + distance;
		double bVal=rayDirection.dotProduct(normal);

		double t=(-1*aVal)/bVal;

		if(t<0){
			return -1;
		}
		else{
			Vect p;
			p.setXYZ(ray.getOrigin().addVect(rayDirection.multVectScaler(t))); //get point p

			Vect ab,bc,ca,ap,bp,cp;
			ab.setXYZ(a.subVect(b));
			bc.setXYZ(b.subVect(c));
			ca.setXYZ(c.subVect(a));
			ap.setXYZ(a.subVect(p));
			bp.setXYZ(b.subVect(p));
			cp.setXYZ(c.subVect(p));

			double cr1=ab.crossProduct(ap).dotProduct(normal);
			double cr2=bc.crossProduct(bp).dotProduct(normal);
			double cr3=ca.crossProduct(cp).dotProduct(normal);

			if((cr1>=0 && cr2>=0 && cr3>=0) || (cr1<=0 && cr2<=0 && cr3<=0))
			{
				return t-EPSILON;
			}
			return -1;
		}
	}

	Vect getNormalAt(Vect point, Ray ray){
		Vect norm;
		norm.setXYZ(getNormal().normalize());
		double val=norm.dotProduct(ray.getDirection().negative());
		if(val<0){
			return norm.negative();
		}
		//cout<<"triangle norm"<<endl;
		return norm;
	}

	Ray getReflectedRayAt(Ray ray, Vect point){
		Vect norm,refRayDir;
		norm.setXYZ(getNormalAt(point,ray));
		refRayDir.setXYZ(ray.getDirection().subVect(norm.multVectScaler(2*ray.getDirection().dotProduct(norm))));
		Ray reflectedRay(point,refRayDir);

		return reflectedRay;
	}

	Ray getRefractedRay(Ray ray, Vect point){
		double n1=1;
		double n2=this->refractiveIndex;
		Vect norm;
		norm.setXYZ(getNormalAt(point,ray));

		double cos=ray.getDirection().dotProduct(norm);

		Vect fPara, fPerp;
		Vect temp=ray.getDirection().subVect(norm.multVectScaler(cos));
		fPara.setXYZ(temp.multVectScaler(n1/n2));

		double val=sqrt(1.0-(n1/n2)*(n1/n2)*(1-cos*cos));
		fPerp.setXYZ(norm.multVectScaler(val));
		fPerp.setXYZ(fPerp.negative());

		Vect f;
		f.setXYZ(fPerp.addVect(fPara));
		f.setXYZ(f.normalize());

		//Vect newOri=Shape::correctEpsilon(f,point);

		//calculate 0i and 0f
		this->cos0i=ray.getDirection().negative().dotProduct(norm);
		this->cos0f=f.dotProduct(norm.negative());
			
		//Ray refractedRay(newOri,f);
		Ray refractedRay(point,f);

		return refractedRay;
	}

	bool isRefractive(){
		return true;
	}

	double getRefractiveIndex(){
		return this->refractiveIndex;
	}

	double getCos0i(){
		return this->cos0i;
	}

	double getCos0f(){
		return this->cos0f;
	}
	//~Triangle();
};

#endif
