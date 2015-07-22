//basic library
#include "headers.h"

//calculate part
#include "fileIO.h"
#include "guass.h"
#include "iniParameter.h"
#include "lighttrace.h"

//useful function
#include "extra.h"

//process data
#include "abberation.h"



int main(){
	int i,j,choice;
	string file_zmx;
	string file_agf;
	double density,hx,hy,w,h;
	light_trace* p;
	
	file_zmx="LENS.ZMX";
	file_agf="GLASS.AGF";
/*
	cout<<"Please input zmx filename:"<<endl;
	cin>>file_zmx;
*/
	readfile(file_zmx);

/*	cout<<"Please input agf filename:"<<endl;
	cin>>file_agf;
*/
	getglass(file_agf);

	if(field.type=1)setting.linearfield=field.data[0]->y_field;
/*
	cout<<"The exists field is:"<<endl;
	cout<<setfill(' ')<<setw(10)<<"id";
	cout<<setfill(' ')<<setw(15)<<"y-field"<<endl;
	
	cout<<"Please input the field that you want to choose:"<<endl;
	cin>>choic
*/

/*
	cout<<"Please input the field parameter:"<<endl;
	cout<<"hx=\t";
	cin>>hx;
	cout<<"hy=\t";
	cin>>hy;
	cout<<"w=\t";
	cin>>w;

	cout<<"Please input the sample density:"<<endl;
	cin>>density;
*/
	
	hx=0;
	hy=1;
	w=3;
	setting.density=3;	

	getsample('s',setting.density,0,hx,hy);

	//getlight(w/180*M_PI);

	cout<<"hello"<<endl;

	surfdata temp;

	addSurface(1,&temp);

	waveout(&wave);
	cout<<setting.primary_wave<<endl;

	cout<<"n="<<cal_glassn("H-K9L",DWAVE)<<endl;

	cout<<"lg="<<guass_imagl(-200,DWAVE)<<endl;
	cout<<"hg="<<guass_imagh(-200,3,DWAVE)<<endl;
	cout<<"f="<<guass_f()<<endl;


	cout<<setw(40)<<setfill('=')<<"="<<endl;
	cout<<"球差和位置色差:"<<endl;
	cout<<setw(40)<<setfill(' ')<<" "<<endl;
	cout<<"孔径大小\t"<<"全孔径\t\t\t"<<"0.707孔径"<<endl;
	cout<<"球差\t\t"<<sphere(1,DWAVE)<<"\t\t"<<sphere(0.707,DWAVE)<<endl;
	cout<<"位置色差\t"<<lch(1)<<"\t\t"<<lch(0.707)<<endl;
	cout<<setw(40)<<setfill('=')<<"="<<endl<<endl;

	cout<<setw(40)<<setfill('=')<<"="<<endl;
	cout<<"子午慧差:"<<endl;
	cout<<setw(40)<<setfill(' ')<<" "<<endl;
	cout<<"\t\t\t"<<"全孔径\t\t\t"<<"0.707孔径"<<endl;
	cout<<"全视场\t\t"<<coma(0,1,0,1)<<"\t\t"<<coma(0,0.707,0,1)<<endl;
	cout<<"0.707视场\t"<<coma(0,1,0,0.707)<<"\t\t"<<coma(0,0.707,0,0.707)<<endl;

	cout<<setw(40)<<setfill('-')<<"-"<<endl;
	cout<<"弧矢慧差:"<<endl;
	cout<<setw(40)<<setfill(' ')<<" "<<endl;
	cout<<"\t\t\t"<<"全孔径\t\t\t"<<"0.707孔径"<<endl;
	cout<<"全视场\t\t"<<coma(1,0,0,1)<<"\t\t"<<coma(0.707,0,0,1)<<endl;
	cout<<"0.707视场\t"<<coma(1,0,0,0.707)<<"\t\t"<<coma(0.707,0,0,0.707)<<endl;
	cout<<setw(40)<<setfill('=')<<"="<<endl<<endl;

	cout<<setw(40)<<setfill('=')<<"="<<endl;
	cout<<"倍率色差:"<<endl;
	cout<<setw(40)<<setfill(' ')<<" "<<endl;
	cout<<"全视场\t\t\t"<<"0.707视场"<<endl;
	cout<<ych(1)<<"\t"<<ych(0.707)<<endl;
	cout<<setw(40)<<setfill('=')<<"="<<endl<<endl;

	cout<<setw(40)<<setfill('=')<<"="<<endl;
	cout<<"子午场曲:"<<endl;
	cout<<setw(40)<<setfill(' ')<<" "<<endl;
	cout<<"全视场\t\t"<<"0.707视场"<<endl;
	cout<<curvature_t(1,DWAVE)<<"\t\t"<<curvature_t(0.707,DWAVE)<<endl;
	cout<<setw(40)<<setfill('-')<<"-"<<endl;
	cout<<"弧矢场曲:"<<endl;
	cout<<setw(40)<<setfill(' ')<<" "<<endl;
	cout<<"全视场\t\t"<<"0.707视场"<<endl;
	cout<<curvature_s(1,DWAVE)<<"\t\t"<<curvature_s(0.707,DWAVE)<<endl;
	cout<<setw(40)<<setfill('-')<<"-"<<endl;
	cout<<"像散:"<<endl;
	cout<<setw(40)<<setfill(' ')<<" "<<endl;
	cout<<"全视场\t\t"<<"0.707视场"<<endl;
	cout<<astigmatism(1,DWAVE)<<'\t'<<astigmatism(0.707,DWAVE)<<endl;
	cout<<setw(40)<<setfill('=')<<"="<<endl<<endl;

	cout<<setw(40)<<setfill('=')<<"="<<endl;
	cout<<"畸变:"<<endl;
	cout<<setw(40)<<setfill(' ')<<" "<<endl;
	cout<<"孔径大小\t"<<"全视场\t\t\t"<<"0.707视场"<<endl;
	cout<<"线畸变\t\t"<<distortion(1,'a',DWAVE)<<"\t\t"<<distortion(0.707,'a',DWAVE)<<endl;
	cout<<"相对畸变\t"<<distortion(1,'r',DWAVE)<<"%\t\t"<<distortion(0.707,'r',DWAVE)<<"%"<<endl;
	cout<<setw(40)<<setfill('=')<<"="<<endl<<endl;


	system("pause");
}


