#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<GLUT/glut.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

#include "bitmap_image.hpp"
#include "shape.h"
#include "sphere.h"
#include "checkerboard.h"
#include "cylinder.h"
#include "triangle.h"
#include "light.h"
#include "color.h"
#include "vect.h"
#include "ray.h"

using namespace std;

#define BLACK 0, 0, 0
#define M_PI 3.1415923

/****************************************************************
/Function Prototype
*****************************************************************/
void checkIntersection();
void drawGrid(int draw);
void captureScreen();
void parseSdl(string fileName);
void checkObjects();
void createOpenGLWorld();
void generateImage();
void initializeShape();

/********************************************************
/Camera Variables
********************************************************/
Vect up;
Vect camPosition;
Vect look;

/********************************************************
/Variables
********************************************************/
GLUquadric* IDquadric=gluNewQuadric();
int canDrawGrid;
int pixels=768;
int const screenWidth=512;
int const screenHeight=512;
double fovAngle=70;
int recDepth;
//Color tracedImage[pixels][pixels];
/********************************************************
/Objects
********************************************************/
vector<Checkerboard> checkerBoard;
vector<Sphere> sphere;
vector<Cylinder> cylinder;
vector<Triangle> triangle;
vector<Light> light;

vector<Shape*> shape;

/********************************************************
/Functions
********************************************************/
void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(BLACK, 0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	/**********************************
	Camera
	**********************************/
	gluLookAt(camPosition.getX(),camPosition.getY(),camPosition.getZ(),		look.getX(),look.getY(),look.getZ(),		up.getX(),up.getY(),up.getZ());

	//NOTE: the camera still CONSTANTLY looks at the center
	// cameraAngle is in RADIAN, since you are using inside COS and SIN
	
	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);

	//grid
	glPushMatrix();{
		//glTranslatef(0,0,0);
		drawGrid(canDrawGrid);
	}glPopMatrix();

	/*************************************************************
	Objects draw start
	**************************************************************/
	createOpenGLWorld();

	/*************************************************************
	Objects draw end
	**************************************************************/
	
	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void init(){
	/************************
	camera initialization
	************************/
	//codes for initialization
	canDrawGrid=0;

	camPosition.setXYZ(0,20,200);
	look.setXYZ(0,0,0);
	up.setXYZ(0,1,0);
	
	//clear the screen
	glClearColor(BLACK, 0);

	
	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);
	
	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(fovAngle,	1,	0.1,	10000.0);
	
}

void animate(){
	//MISSING SOMETHING? -- YES: add the following
	glutPostRedisplay();	//this will call the display AGAIN
}

void keyboardListener(unsigned char key, int x,int y){

	Vect dir;//new added
	Vect normAlong;
	Vect normForward;
	Vect normUp;
	Vect newcamPosition;
	Vect newLook;
	Vect newUp;
	Vect along;
	double dirMag;
	double angle;
	double angleChange=0.05;

	switch(key){

		case '1':	
			dir.setXYZ(camPosition.subVect(look));//added
			dirMag=dir.magnitude();//addded

			along.setXYZ(dir.crossProduct(up));//added
			along.setXYZ(along.negative());//added

			normAlong.setXYZ(along.normalize());//added
			normForward.setXYZ(dir.normalize());//added

			angle=180*3.1416/180+angleChange;

			newLook.setXYZ(camPosition.addVect(normForward.multVectScaler(dirMag*cos(angle))));//added
			newLook.setXYZ(newLook.addVect(normAlong.multVectScaler(dirMag*sin(angle))));//added
			look.setXYZ(newLook);//added

			break;

		case '2':	
			dir.setXYZ(camPosition.subVect(look));//added
			dirMag=dir.magnitude();//addded

			along.setXYZ(dir.crossProduct(up));//added
			along.setXYZ(along.negative());//added

			normAlong.setXYZ(along.normalize());//added
			normForward.setXYZ(dir.normalize());//added

			angle=180*3.1416/180-angleChange;

			newLook.setXYZ(camPosition.addVect(normForward.multVectScaler(dirMag*cos(angle))));//added
			newLook.setXYZ(newLook.addVect(normAlong.multVectScaler(dirMag*sin(angle))));//added
			look.setXYZ(newLook);//added

			break;

		case '3':				
			dir.setXYZ(camPosition.subVect(look));//added
			dirMag=dir.magnitude();//addded
			
			along.setXYZ(dir.crossProduct(up));//added
			along.setXYZ(along.negative());//added

			normForward.setXYZ(dir.normalize());//added
			normUp.setXYZ(up.normalize());//added

			angle=180*3.1416/180+angleChange;

			newLook.setXYZ(camPosition.addVect(normForward.multVectScaler(dirMag*cos(angle))));//added
			newLook.setXYZ(newLook.addVect(normUp.multVectScaler(dirMag*sin(angle))));//added
			look.setXYZ(newLook);//added

			dir.setXYZ(camPosition.subVect(look));//added

			up.setXYZ(dir.crossProduct(along));//added
			up.setXYZ(up.normalize());//added

			break;

		case '4':				
			dir.setXYZ(camPosition.subVect(look));//added
			dirMag=dir.magnitude();//addded

			along.setXYZ(dir.crossProduct(up));//added
			along.setXYZ(along.negative());//added
			normForward.setXYZ(dir.normalize());//added
			normUp.setXYZ(up.normalize());//added

			angle=180*3.1416/180-angleChange;

			newLook.setXYZ(camPosition.addVect(normForward.multVectScaler(dirMag*cos(angle))));//added
			newLook.setXYZ(newLook.addVect(normUp.multVectScaler(dirMag*sin(angle))));//added
			look.setXYZ(newLook);//added
			
			dir.setXYZ(camPosition.subVect(look));//added
			up.setXYZ(dir.crossProduct(along));//added
			up.setXYZ(up.normalize());//added

			break;

		case '5':
			dir.setXYZ(camPosition.subVect(look));//added
			dirMag=dir.magnitude();//addded

			along.setXYZ(dir.crossProduct(up));//added
			along.setXYZ(along.negative());//added
			normAlong.setXYZ(along.normalize());//added
			normUp.setXYZ(up.normalize());//added

			angle=0*3.1416/180+angleChange;

			newUp.setXYZ(camPosition.addVect(normUp.multVectScaler(dirMag*cos(angle))));//added
			newUp.setXYZ(newUp.addVect(normAlong.multVectScaler(dirMag*sin(angle))));//added

			up.setXYZ(newUp.subVect(camPosition));//added
			up.setXYZ(up.normalize());//added

			break;

		case '6':	
			dir.setXYZ(camPosition.subVect(look));//added
			dirMag=dir.magnitude();//addded

			along.setXYZ(dir.crossProduct(up));//added
			along.setXYZ(along.negative());//added
			normAlong.setXYZ(along.normalize());//added
			normUp.setXYZ(up.normalize());//added

			angle=0*3.1416/180-angleChange;

			newUp.setXYZ(camPosition.addVect(normUp.multVectScaler(dirMag*cos(angle))));//added
			newUp.setXYZ(newUp.addVect(normAlong.multVectScaler(dirMag*sin(angle))));//added
			
			up.setXYZ(newUp.subVect(camPosition));//added
			up.setXYZ(up.normalize());//added

			break;

		case '8':	//toggle grids
			canDrawGrid = 1 - canDrawGrid;
			break;

		case 'r':
			canDrawGrid = 1;
			
			camPosition.setXYZ(0,50,200);
			look.setXYZ(0,0,0);
			up.setXYZ(0,1,0);

			
			break;
		case 27:	//ESCAPE KEY -- simply exit
			exit(0);
			break;

		case 'c':
			checkIntersection();
			break;

		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){
	double speed=5;

	Vect dir;//new added
	Vect norm;
	Vect newcamPosition;
	Vect newLook;
	Vect along;
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			dir.setXYZ(camPosition.subVect(look));//new added
			norm.setXYZ(dir.normalize());//added

			newLook.setXYZ(look.addVect(norm.multVectScaler(speed)));//added
			look.setXYZ(newLook);//added

			newcamPosition.setXYZ(camPosition.addVect(norm.multVectScaler(speed)));//added
			camPosition.setXYZ(newcamPosition);//added

			break;

		case GLUT_KEY_UP:		// up arrow key
			dir.setXYZ(camPosition.subVect(look));//new added
			norm.setXYZ(dir.normalize());//added

			newLook.setXYZ(look.subVect(norm.multVectScaler(speed)));//added
			look.setXYZ(newLook);//added

			newcamPosition.setXYZ(camPosition.subVect(norm.multVectScaler(speed)));//added
			camPosition.setXYZ(newcamPosition);//added

			break;

		case GLUT_KEY_RIGHT:
			dir.setXYZ(camPosition.subVect(look));//new added
			along.setXYZ(dir.crossProduct(up));//added

			norm.setXYZ(along.normalize());//added

			newcamPosition.setXYZ(camPosition.subVect(norm.multVectScaler(speed)));//added
			camPosition.setXYZ(newcamPosition);//added
		
			newLook.setXYZ(look.subVect(norm.multVectScaler(speed)));//added
			look.setXYZ(newLook);//added

			break;
		case GLUT_KEY_LEFT:
			dir.setXYZ(camPosition.subVect(look));//new added
			along.setXYZ(dir.crossProduct(up));//added
			
			norm.setXYZ(along.normalize());//added

			newcamPosition.setXYZ(camPosition.addVect(norm.multVectScaler(speed)));//added
			camPosition.setXYZ(newcamPosition);//added
			
			newLook.setXYZ(look.addVect(norm.multVectScaler(speed)));//added
			look.setXYZ(newLook);//added

			break;

		case GLUT_KEY_PAGE_UP:
			norm.setXYZ(up.normalize());//added
			
			newcamPosition.setXYZ(camPosition.addVect(norm.multVectScaler(speed)));//added
			camPosition.setXYZ(newcamPosition);//added

			newLook.setXYZ(look.addVect(norm.multVectScaler(speed)));//added
			look.setXYZ(newLook);//added
			
			break;
		case GLUT_KEY_PAGE_DOWN:
			norm.setXYZ(up.normalize());//added

			newcamPosition.setXYZ(camPosition.subVect(norm.multVectScaler(speed)));//added
			camPosition.setXYZ(newcamPosition);//added

			newLook.setXYZ(look.subVect(norm.multVectScaler(speed)));//added
			look.setXYZ(newLook);//added
	
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}

void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				//captureScreen();
				generateImage();
			}
			break;

		case GLUT_RIGHT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				captureScreen();
			}
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}

int main(int argc, char **argv){
	//parse sdl file
	//string fileName="Spec.txt";//sdl file name
    string fileName="//Users//ahmadsabbir//Desktop//opengl//RayTracing//RayTracing//RayTracing//Spec.txt";
	parseSdl(fileName);
	//checkObjects();

	glutInit(&argc,argv);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(600, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("RAY TRACING");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	//ADD keyboard listeners:
	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);

	//ADD mouse listeners:
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}

void drawGrid(int draw){
	int i;

	//WILL draw grid IF the "canDrawGrid" is true:

	if(draw == 1){
		glColor3f(0.3, 0.3, 0.3);	//grey
		glBegin(GL_LINES);{
			for(i=-10;i<=10;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -100, 0);
				glVertex3f(i*10,  100, 0);

				//lines parallel to X-axis
				glVertex3f(-100, i*10, 0);
				glVertex3f( 100, i*10, 0);

			}
		}glEnd();
		// draw the two AXES
		//glColor3f(1, 1, 1);	//100% white
		glColor3f(0.3, 0.3, 0.3);
		glBegin(GL_LINES);{
			//Y axis
			glVertex3f(0, -150, 0);	// intentionally extended to -150 to 150, no big deal
			glVertex3f(0,  150, 0);

			//X axis
			glVertex3f(-150, 0, 0);
			glVertex3f( 150, 0, 0);
		}glEnd();
	}
}

void parseSdl(string fileName){
	cout<<"SDL File: "<<endl;
	ifstream fin;
	fin.open(fileName);
	if(fin.fail()){
        cout<<fileName<<" file not found!"<<endl;
	}

	char str[100];
	int pix;
	fin >> str >> recDepth;
	//fin >> str >> pixels;
	fin >> str >> pix;

	cout << "recDepth" << " " <<recDepth<<endl;
	cout << "pixels" << " " <<pix<<endl;
	pixels=pix;


	int i;
	double cl1[3], cl2[3], c[3], x[3], y[3], z[3];
	double pos[3];
	char nm[20];
	double ac, dc, rc, sc, se, r, rI, xC, zC, yMn, yMx;
	while(!fin.eof()){
		//cout<< "first of while"<<endl;
		fin >> str;
		//cout<<"firstWhile: "<<str<<endl;
		if(strcmp(str,"objStart")==0){
			fin >> str;
			if(strcmp(str,"CHECKERBOARD")==0){
				strcpy(nm,str);
				//cout<<"nm: "<<nm<<endl;
				fin >> str;
				for(i=0;i<3;i++){
					fin >> cl1[i];
				}
				Color clr1(cl1[0],cl1[1],cl1[2]);
				//cout<<clr1.getRed()<<" "<<clr1.getGreen()<<" "<<clr1.getBlue()<<endl;
				fin >> str;
				for(i=0;i<3;i++){
					fin >> cl2[i];
				}
				Color clr2(cl2[0],cl2[1],cl2[2]);
				//cout<<clr2.getRed()<<" "<<clr2.getGreen()<<" "<<clr2.getBlue()<<endl;
				fin >> str >> ac;
				fin >> str >> dc;
				fin >> str >> rc;
				fin >> str >> sc;
				fin >> str >> se;
				fin >> str;
				
				Checkerboard cB(nm,ac,dc,rc,sc,se,clr1,clr2);
				checkerBoard.push_back(cB);
				/*Shape* sh;
				sh = &cB;
				shape.push_back(sh);
				/*
				Vect origin(-1,-1,-1);
				Vect direction(1,1,1);
				Ray ray(origin,direction);

				double val=sh->findIntersection(ray);
				cout<<"val: "<<val<<endl;
				
				shape.push_back(sh);
				Shape* spx;
				spx=shape[0];
				Vect origin(-1,-1,-1);
				Vect direction(1,1,1);
				Ray ray(origin,direction);

				double val=spx->findIntersection(ray);
				cout<<"val: "<<val<<endl;
				*/

				cout<<"checkerboard"<<endl;
			}
			else if(strcmp(str,"SPHERE")==0){
				strcpy(nm,str);
				fin >> str;
				for(i=0;i<3;i++){
					fin >> c[i];
				}
				Vect cen(c[0],c[1],c[2]);
				fin >> str >> r;
				fin >> str;
				for(i=0;i<3;i++){
					fin >> cl1[i];
				}

				Color cl(cl1[0],cl1[1],cl1[2]);
				fin >> str >> ac;
				fin >> str >> dc;
				fin >> str >> rc;
				fin >> str >> sc;
				fin >> str >> se;
				fin >> str;

				Sphere sp(nm,ac,dc,rc,sc,se,cen,r,cl);
				//cout<<"sp.amb "<<sp.ambCoeff<<endl;
				sphere.push_back(sp);
				/*Shape* sh;
				sh = &sp;
				shape.push_back(sh);*/
				cout<<"sphere"<<endl;

			}
			else if(strcmp(str,"CYLINDER")==0){
				strcpy(nm,str);
				fin >> str >> xC;
				fin >> str >> zC;
				fin >> str >> r;
				fin >> str >> yMn;
				fin >> str >> yMx;
				fin >> str;
				for(i=0;i<3;i++){
					fin >> cl1[i];
				}
				Color cl(cl1[0],cl1[1],cl1[2]);
				fin >> str >> ac;
				fin >> str >> dc;
				fin >> str >> rc;
				fin >> str >> sc;
				fin >> str >> se;
				fin >> str;

				Cylinder clyn(nm,ac,dc,rc,sc,se,xC,zC,r,yMn,yMx,cl);
				cylinder.push_back(clyn);
				/*Shape* sh;
				sh = &clyn;
				shape.push_back(sh);*/
				cout<<"cylinder"<<endl;
			}
			else if(strcmp(str,"TRIANGLE")==0){
				strcpy(nm,str);
				fin >> str;
				for(i=0;i<3;i++){
					fin >> x[i];
				}
				fin >> str;
				for(i=0;i<3;i++){
					fin >> y[i];
				}
				fin >> str;
				for(i=0;i<3;i++){
					fin >> z[i];
				}
				Vect vx(x[0],x[1],x[2]);
				Vect vy(y[0],y[1],y[2]);
				Vect vz(z[0],z[1],z[2]);
				fin >> str;
				for(i=0;i<3;i++){
					fin >> cl1[i];
				}
				Color cl(cl1[0],cl1[1],cl1[2]);
				fin >> str >> ac;
				fin >> str >> dc;
				fin >> str >> rc;
				fin >> str >> sc;
				fin >> str >> se;
				fin >> str >> rI;
				fin >> str;

				Triangle tr(nm,ac,dc,rc,sc,se,vx,vy,vz,cl,rI);
				triangle.push_back(tr);
				/*Shape* sh;
				sh = &tr;
				shape.push_back(sh);*/
				cout<<"triangle"<<endl;
			}
		}
		else if(strcmp(str,"light")==0){
			strcpy(nm,str);
			//fin >> str;
			for(i=0;i<3;i++){
				fin >> pos[i];
			}
			Vect p(pos[0],pos[1],pos[2]);
			Color cl(1,1,1);
			Light lt(nm,p,cl);
			light.push_back(lt);
			cout<<"light"<<endl;
		}
		//cout<< "last of while"<<endl;
	}

	fin.close();
	cout<<"SDL File end."<<endl;

	initializeShape();

	cout<<endl;
}

void checkObjects(){
	printf("Check Objects:\n");
	printf("Checkerboard: %d\n",checkerBoard.size());
	printf("Sphere: %d\n",sphere.size());
	printf("Cylinder: %d\n",cylinder.size());
	printf("Triangle: %d\n",triangle.size());
	printf("light: %d\n",light.size());
	printf("Checking complete\n\n");
}

void initializeShape(){
	int i;
	int checkerBoardN=checkerBoard.size();
	for(i=0;i<checkerBoardN;i++){
		Checkerboard ck=checkerBoard[i];
		shape.push_back(&checkerBoard[i]);
	}

	int sphereN=sphere.size();
	for(i=0;i<sphereN;i++){
		Sphere sp=sphere[i];
		shape.push_back(&sphere[i]);
		//cout<<sphere[i].ambCoeff<<endl;
	}
	
	int cylinderN=cylinder.size();
	for(i=0;i<cylinderN;i++){
		Cylinder cl=cylinder[i];
		shape.push_back(&cylinder[i]);
	}
	
	int triangleN=triangle.size();
	for(i=0;i<triangleN;i++){
		Triangle tr=triangle[i];
		shape.push_back(&triangle[i]);
	}
	
	//shape.push_back(&sphere[4]);
}

void createOpenGLWorld(){
	//y-axis
	glColor3f(0, 1, 0);
	glBegin(GL_LINES);{
		glVertex3f(0, -300, 0);
		glVertex3f(0,  300, 0);
	}glEnd();
	//x-axis
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);{
		glVertex3f(-300, 0, 0);
		glVertex3f(300, 0, 0);
	}glEnd();
	glColor3f(0, 0, 1);
	glBegin(GL_LINES);{
		glVertex3f(0, 0, -300);
		glVertex3f(0, 0, 300);
	}glEnd();
	
	/*
	glPushMatrix();{
		glRotatef(90,-1,0,0);
		gluCylinder(IDquadric,0.3,0.3,500,20,20);
	}glPopMatrix();
	*/

	//generate checkerboard

	float red1,green1,blue1;
	float red2,green2,blue2;
	int i,j;
	int k;
	int ckBoardN=checkerBoard.size();
	for(k=0;k<ckBoardN;k++){
		i=j=0;
		Checkerboard ck=checkerBoard[k];
		//int box=200;
		int box=50;
		int size=20;
		float px,py,pz;
		
		//red1=ck.getColor(1).getRed();green1=ck.getColor(1).getGreen();blue1=ck.getColor(1).getBlue();
		//red2=ck.getColor(2).getRed();green2=ck.getColor(2).getGreen();blue2=ck.getColor(2).getBlue();
		red1=ck.getColorOne().getRed();green1=ck.getColorOne().getGreen();blue1=ck.getColorOne().getBlue();
		red2=ck.getColorTwo().getRed();green2=ck.getColorTwo().getGreen();blue2=ck.getColorTwo().getBlue();
		py=0;
		px=pz=-size*(box/2);
		glPushMatrix();{
			for(i=0;i<box;i++){
				for(j=0;j<box;j++){
					if((i+j)%2==0){
						glColor3f(red1,green1,blue1);
					}
					else{
						glColor3f(red2,green2,blue2);
					}
					glBegin(GL_QUADS);{
						glVertex3f(px,py,pz);
						glVertex3f(px,py,pz+size);
						glVertex3f(px+size,py,pz+size);
						glVertex3f(px+size,py,pz);
					}glEnd();
					px+=size;
				}
				px=-size*(box/2);
				pz+=size;
			}
		}glPopMatrix();

	}
	//check checkerboard intersection
	/*Vect origin(-1,-1,-1);
	Vect direction(-2,1,1);
	Ray ray(origin,direction);

	double t=ck.findIntersection(ray);
	cout<<"intersection: "<<t<<endl;
	*/

	//generate spheres

	GLint slices=50;
 	GLint stacks=50;
	int sphereN=sphere.size();
	for(i=0;i<sphereN;i++){
		Sphere sp=sphere[i];
		glPushMatrix();{
			glColor3f(sp.getColor().getRed(),sp.getColor().getGreen(),sp.getColor().getBlue());
			glTranslatef(sp.getCenter().getX(),sp.getCenter().getY(),sp.getCenter().getZ());
			gluSphere(IDquadric,sp.getRadius(),slices,stacks);
		}glPopMatrix();
	}
	
	//generate cylinder
	int cylinderN=cylinder.size();
	for(i=0;i<cylinderN;i++){
		Cylinder cl=cylinder[i];
		red1=cl.getColor().getRed();
		green1=cl.getColor().getGreen();
		blue1=cl.getColor().getBlue();
		glPushMatrix();{
			glColor3f(red1,green1,blue1);
			glRotatef(90,-1,0,0);
			glTranslatef(cl.getXCenter(),cl.getZCenter(),cl.getYMin());
			gluCylinder(IDquadric,cl.getRadius(),cl.getRadius(),cl.getYMax()-cl.getYMin(),slices,stacks);
		}glPopMatrix();
		glPushMatrix();{
			glColor3f(red1,green1,blue1);
			glRotatef(90,-1,0,0);
			glTranslatef(cl.getXCenter(),cl.getZCenter(),cl.getYMin());
			gluDisk(IDquadric,0,cl.getRadius(),slices,stacks);
		}glPopMatrix();
		glPushMatrix();{
			glColor3f(red1,green1,blue1);
			glRotatef(90,-1,0,0);
			glTranslatef(cl.getXCenter(),cl.getZCenter(),cl.getYMax());
			gluDisk(IDquadric,0,cl.getRadius(),slices,stacks);
		}glPopMatrix();
	}

	//generate triangle
	int triangleN=triangle.size();
	for(i=0;i<triangleN;i++){
		Triangle tr=triangle[i];
		glPushMatrix();{
			glColor3f(tr.getColor().getRed(),tr.getColor().getGreen(),tr.getColor().getBlue());
			glBegin(GL_TRIANGLES);{
				glVertex3f(tr.getA().getX(),tr.getA().getY(),tr.getA().getZ());
				glVertex3f(tr.getB().getX(),tr.getB().getY(),tr.getB().getZ());
				glVertex3f(tr.getC().getX(),tr.getC().getY(),tr.getC().getZ());
			}glEnd();
		}glPopMatrix();
	}

	//show lights
	int lightN=light.size();
	for(i=0;i<lightN;i++){
		Light lt=light[i];
		glColor3f(1,1,1);
		glPushMatrix();{
			glTranslatef(lt.getPosition().getX(),lt.getPosition().getY(),lt.getPosition().getZ());
			glutSolidCube(1);
		}glPopMatrix();
	}
}

void checkIntersection(){
	int i;
	Vect origin(-1,-1,-1);
	Vect direction(1,1,1);
	Ray ray(origin,direction);

	Checkerboard ck=checkerBoard[0];
	double t=ck.findIntersection(ray);
	cout<<"Checkerboard: "<<endl<<t<<endl;

	cout<<"Sphere:"<<endl;
	int sphereN=sphere.size();
	for(i=0;i<sphereN;i++){
		Sphere sp=sphere[i];
		double val=sp.findIntersection(ray);
		cout<<i<<": "<<val<<endl;
	}
}

void captureScreen(){

	printf("Generating image... \n");

	unsigned int i;
	unsigned int j;

	unsigned char red;
	unsigned char green;
	unsigned char blue;

	GLint x;
	GLint y;
	GLsizei width=1;
	GLsizei height=1;
	GLenum format=GL_RGB;
	GLenum type=GL_UNSIGNED_BYTE;
	GLubyte *data = new GLubyte[3 * width * height];
	
	//glReadPixels(x,y,width,height,format,type,data);
	
	//printf("%d, %d, %d",data[0],data[1],data[2]);


	GLint startX=0;
	GLint startY=0;

	GLint limitX=pixels;
	GLint limitY=pixels;

	bitmap_image image(limitX,limitY);

	for(x=startX;x<startX+limitX;x++){
		for(y=startY;y<startY+limitY;y++){
			glReadPixels(x,y,width,height,format,type,data);
			i=(int)x;
			j=(int)y;
			red=(unsigned char)data[0];
			green=(unsigned char)data[1];
			blue=(unsigned char)data[2];
			image.set_pixel(x-startX,(limitY-1)-(y-startY),red,green,blue);
		}
	}
	//image.vertical_flip();
	image.save_image("screen.bmp");

	printf("done!!\n");
}

void generateImage(){
	printf("Generating image... \n");
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	bitmap_image image(pixels,pixels);

	int i,j;
	Vect camUp,camDir,camRight,camPos;
	Vect tempRight,tempUp,tempDir;
	Vect imagePoint;
	Vect rayDir;

	camPos.setXYZ(camPosition);
	camUp.setXYZ(up.normalize());
	camDir.setXYZ(look.subVect(camPos).normalize());
	camRight.setXYZ(camDir.crossProduct(camUp).normalize());

	double n_i;
	double n_j;

	int shapeN=shape.size();
	double viewPlane;

	/*Vect vt;
	vt.setXYZ(camPos.addVect(look.subVect(camPos)));
	cout<<vt.getX()<<" "<<vt.getY()<<" "<<vt.getZ()<<endl;
	*/
	/*
	Vect point1=GetOGLPos(0,0);
	Vect point2=GetOGLPos(0,511);

	double dis=point1.pointToPointDistance(point2);
	cout<<"dis: "<<dis<<endl;
	
	viewPlane=(dis/2.0)/(tan((fovAngle*M_PI)/360.0));
	cout<<"viewPlane: "<<viewPlane<<endl;
	*/
	//width/(tan(cam.viewAngle*PI/360))+150;
	viewPlane=(screenWidth/2.0)/(tan((fovAngle*M_PI)/360.0));
	//cout<<"viewPlane: "<<viewPlane<<endl;
	//viewPlane=0.7;
	//int pcount=0;
	for(i=0;i<pixels;i++){
		for(j=0;j<pixels;j++){
			//ray generation

			//double uc=-width+2*i*width/(double)imgSize;
            //double vr=-height+2*j*height/(double)imgSize;

			//n_i=i/width-0.5;
			//n_j=j/height-0.5;

			n_i=-(screenWidth/2.0)+i*screenWidth/(double)pixels;
			n_j=-(screenWidth/2.0)+j*screenWidth/(double)pixels;


			tempRight.setXYZ(camRight.multVectScaler(n_i));
			tempUp.setXYZ(camUp.multVectScaler(n_j));
			//tempDir.setXYZ(camPos.addVect(camDir));
			tempDir.setXYZ(camPos.addVect(camDir.multVectScaler(viewPlane)));

			imagePoint.setXYZ(tempRight.addVect(tempUp));
			imagePoint.setXYZ(imagePoint.addVect(tempDir));

			rayDir.setXYZ(imagePoint.subVect(camPos));
			Ray ray(camPos,rayDir);
			Shape* sh;
			//check all object intersection
			double minIntersect=999999;
			double intersect;
			int index=-1;
			for(int object=0;object<shapeN;object++){
				//cout<<"shape "<<object<<endl;
				sh=shape[object];
				//cout<<"shape assigned"<<object<<endl;
				intersect=sh->findIntersection(ray);
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
			//Shape* sh;
			if(index==-1){
				red=(unsigned char)(0);
				green=(unsigned char)(0);
				blue=(unsigned char)(0);
			}
			else{
				sh=shape[index];
				//double intensity=sh->getLightIntensity(ray,minIntersect,light,shape);
				
				/*if(intensity!=0){
					//cout<<"intensity: "<<intensity<<endl;
					pcount++;
				}
				*/

				Color color=sh->getColor(ray,minIntersect,light,shape,index,recDepth);
				red=(unsigned char)(255*color.getRed());
				green=(unsigned char)(255*color.getGreen());
				blue=(unsigned char)(255*color.getBlue());
			}
			image.set_pixel(i,pixels-1-(j),red,green,blue);
		}
	}
	//cout<<"pc Count: "<<pcount<<endl;
	image.save_image("//Users//ahmadsabbir//Desktop//opengl//RayTracing//RayTracing//RayTracing//output.bmp");
	
	printf("done!!\n");
}
