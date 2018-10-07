#pragma once
#ifndef _RAY_H
#define _RAY_H

#include <math.h>
#include "vect.h"

class Ray{
	Vect origin;
	Vect direction;

public: 
	Ray(){origin.setXYZ(0,0,0);direction.setXYZ(1,0,0);}
	Ray(Vect o, Vect d){origin.setXYZ(o);direction.setXYZ(d.normalize());}
	//Vect(const Vect& v){x=v.x;y=v.y;z=v.z;}
	
	Vect getOrigin() const {return origin;}
	Vect getDirection() const {return direction;}

	void setRay(Vect ori, Vect dir){	
		origin.setXYZ(ori);
		direction.setXYZ(dir);
	}
};

#endif