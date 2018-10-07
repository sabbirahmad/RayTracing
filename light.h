#ifndef _LIGHT_H
#define _LIGHT_H

#include <cstring>
#include "color.h"
#include "vect.h"

class Light{
//public:
	char name[20];
	Vect position;
	Color lightColor;

public:
	Light(){
		strcpy(name,"");
		position.setXYZ(0,0,0);
	}
	Light(char nm[20], Vect pos, Color cl){
		strcpy(name,nm);
		position.setXYZ(pos);
		lightColor=cl;
	}

	Vect getPosition(){
		return position;
	}
	//~Light();
};

#endif