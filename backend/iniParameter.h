void cal_if(double W,fieldsample* ftemp,light_trace* temp);
void cal_fi(double y,double lequal,double u,fieldsample* ftemp,light_trace* temp);
void cal_ff(double y,double lip,fieldsample* ftemp,light_trace* temp);

void getsample(char mode,double density,int type,double Hx,double Hy);
double ylim(double step,double x);

vector< vector<fieldsample*>* > sample;

void getsample(char mode,double density,int type,double Hx,double Hy){
	int i,j;
	double step;
	double h=setting.hp;
	fieldsample* temp;
	
	switch(mode){
		case 's':
			switch(type){
				case 0:
					step=1/density;
					for(i=-density;i<=density;i++){
						sample.push_back(new vector<fieldsample*>);
						for(j=-ylim(step,i*step);j<=ylim(step,i*step);j++){
							temp=new fieldsample();
							temp->Px=i*step;
							temp->Py=j*step;
							temp->Hx=Hx;
							temp->Hy=Hy;
							sample[sample.size()-1]->push_back(temp);
						}
					}break;
				case 1:
				case 2:
				case 3:
				default: 
					cout<<"Illegal Type"<<endl;
			}
			break;
		case 'h':
		case 'd':
		default:
			cout<<"Illegal Mode"<<endl;
	}
}

double ylim(double step,double x){
	return floor(sqrt(1-x*x)/step);
} 

//当物体和入瞳都为有限远的时候。以入瞳与光轴的交点为坐标原点，
void cal_ff(double y,double lip,fieldsample* ftemp,light_trace *temp){
	double x_1,x_2,x_3;
	double length;
	double Px=ftemp->Px,Py=ftemp->Py,Hx=ftemp->Hx,Hy=ftemp->Hy;
	double h=setting.hp;

//calculate the length
	x_1=lip;
	x_2=Px*h-Hx*y;
	x_3=Py*h-Hy*y;
	length=sqrt(pow(x_1,2)+pow(x_2,2)+pow(x_3,2));
//calculate the direction
	temp->cosa=x_2/length;
	temp->cosb=x_3/length;
	temp->cosc=x_1/length;
//calculate the position
	temp->x=Px*h;
	temp->y=Py*h;
	temp->z=0;
	temp->next=NULL;
}

//当物体为无穷远的时候。
void cal_if(double W,fieldsample* ftemp,light_trace *temp){
	double a,b;
	double h=setting.hp;
	double Px=ftemp->Px,Py=ftemp->Py,Hx=ftemp->Hx,Hy=ftemp->Hy;
//calculate the direction
	a=Hx*tan(W);b=Hy*tan(W);
	temp->cosa=a/sqrt(1+pow(a,2)+pow(b,2));
	temp->cosb=b/sqrt(1+pow(a,2)+pow(b,2));
	temp->cosc=sqrt(1-pow(temp->cosa,2)-pow(temp->cosb,2));
//calculate the position
	temp->x = Px * h;
	temp->y = Py * h;
	temp->z =0;
	temp->next=NULL;
}

//当瞳面在无穷远的情况
void cal_fi(double y,double lequal,double u,fieldsample* ftemp,light_trace* temp){
	double length;
	double x_1,x_2,x_3;
	double l;
	double Px=ftemp->Px,Py=ftemp->Py,Hx=ftemp->Hx,Hy=ftemp->Hy;
	double hequal=lequal*tan(u);

//calculate the length
    x_1=lequal;
	x_2=Px*hequal-Hx*y;
	x_3=Py*hequal-Hy*y;
	length=sqrt(pow(x_1,2)+pow(x_2,2)+pow(x_3,2));
//calculate the direction
	temp->cosa=x_2/length;  //alpha
	temp->cosb=x_3/length;  //beta
	temp->cosc=x_1/length;  //gama
//calculate the position
	temp->x=Hx*y;
	temp->y=Hy*y;
	temp->z=0;
	temp->next=NULL;
}