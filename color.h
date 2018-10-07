#pragma once
#ifndef _COLOR_H
#define _COLOR_H

class Color{
	double red;
	double green;
	double blue;
public: 
	Color(){red=1;green=1;blue=1;}
	Color(double r, double g, double b){red=r;green=g;blue=b;}
	void setRed(double r){red=r;}
	void setGreen(double g){green=g;}
	void setBlue(double b){blue=b;}
	void setRGB(double r, double g, double b){red=r;green=g;blue=b;}

	double getRed() const {return red;}
	double getGreen() const {return green;}
	double getBlue() const {return blue;}

	void operator=(const Color& cl)
      {
		 this->setRed(cl.getRed());
		 this->setGreen(cl.getGreen());
		 this->setBlue(cl.getBlue());
      }
};

#endif