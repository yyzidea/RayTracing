#include <stdio.h>
#include <math.h>
#include <string.h>
#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <iomanip>

#define FWAVE 0.48613270
#define DWAVE 0.58756180
#define CWAVE 0.65627250
#define MULTI 1000000000

using namespace std;

//from fileIO.h

void readfile(string filename);
void getglass(string filename);
double getnd(string glassname);
double getvd(string glassname);
double cal_glassn(string glassname,double wavelength);
double getid(string glassname);

class wavelengthdata
{
public:
	double wavelength;
	double weight;

	wavelengthdata();
};

wavelengthdata::wavelengthdata(){
	wavelength=0.55;
	weight=1;
}

class surfdata{
public:
    double curv;
    double thickness;
    string glassname;
    double sDiameter;
    int type;
    
    surfdata();

    double getdata(char mode){
        switch(mode){
            case 'r':return curv;
            case 't':return thickness;
            case 'v':return getvd(glassname);
            case 'n':return getnd(glassname);
            case 'd':return sDiameter;
            case 'm':return type;
            default:
				cout<<"Illegal Mode!"<<endl;
				return 0;
        }
    }

    double getn(double wavelength){
		return cal_glassn(glassname,wavelength);
	}
};

surfdata::surfdata(){
	curv=0;
	glassname="AIR";
	sDiameter=0;
	thickness=0;
	type=0;
}

class fielddata{
public:
	double x_field;
	double y_field;
	double weight;
	double vdx;
	double vdy;
	double vcx;
	double vcy;
	double van;

	fielddata();
};

fielddata::fielddata(){
	x_field=y_field=vdx=vdy=vcx=vcy=van=0;
	weight=0;
}

class fieldsetting
{
public:
	int type;
	vector<fielddata*> data;
	int field_used;
};

class glassdata{
public:
	string name;
	double Nd;
	double Vd;
	double A0;
	double A1;
	double A2;
	double A3;
	double A4;
	double A5;
};

class general_setting{
public:
	double hp;
	double density;
	double linearfield;
	int wave_used;
	int primary_wave;
	string glass_catalog;
};

//from iniParameter.h

class fieldsample{
public:
	double Px;
	double Py;
	double Hx;
	double Hy;
};

//from lighttrace.h

class light_trace{
public:
	double x,y,z,cosa,cosb,cosc;
    light_trace *next;
};
