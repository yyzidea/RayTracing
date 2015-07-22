void ltrace(light_trace *q,char mode);
void cal_pos(light_trace *qo,light_trace *qn,double t,double r);
void cal_ang(light_trace *qo,light_trace *qn,double r,double n1,double n2 );

void ltrace(light_trace *q,double wavelength,char mode);

void add(vector<double>* temp1,vector<double>* temp2);
double dot(vector<double>* temp1,vector<double>* temp2);
void scalarmulit(vector<double>* temp,double lambda);

//form outside
double cal_lp(void);

void sampleout(fieldsample* temp);
void lightout(light_trace* temp);

vector< vector<light_trace*>*> light;

void ltrace(light_trace *q,char mode)
{
	double r,n1,n2,t;
	light_trace *qn;
	int i;
	
	for( i = 1; i < surface.size()-1 ; i++){
		//cout<<"i="<<i<<endl;//
	 	qn=new light_trace;
	 	qn->next=NULL;
		r = surface[i]->getdata('r');
	 	n1 = surface[i-1]->getdata('n');
	 	n2 = surface[i]->getdata('n');
	 	if(i>1){
	 		t = surface[i-1]->getdata('t');
	 	}else{
	 		switch(mode){
	 			case 'p':t = -cal_lp();break;
	 			case 'o':t = surface[0]->getdata('t');
	 		}
	 	}

		cal_pos(q,qn,t,r);
		cal_ang(q,qn,r,n1,n2);

        if(qn->x==-1&&qn->y==-1&&qn->z==-1){
        	break;
        }


        q->next=qn;
        q=qn;
        // cout<<"r="<<r<<endl;//
        // lightout(q);//
	}
}

void cal_pos(light_trace *qo,light_trace *qn,double d,double r)
{
	double a,b,c,k1,k2,k;
	double x1,y1,z1;
	double x2,y2,z2;
	double delta;
	double cosa=qo->cosa,cosb=qo->cosb,cosc=qo->cosc;
	double x=qo->x,y=qo->y,z=qo->z;

	a=r*(cosa*cosa+cosb*cosb+cosc*cosc);
	b=2*r*(cosa*x+cosb*y+cosc*(z-d))-2*cosc;
	c=r*(x*x+y*y+(z-d)*(z-d))-2*z+2*d;

	if(r!=0){
		delta=b*b-4*a*c;

		if (delta>=0) {
			k1 = (-b+sqrt(delta))/(2*a);
			k2 = (-b-sqrt(delta))/(2*a);
			
			if(r>0)
	        {
				if(k1*cosc+z-d<k2*cosc+z-d)k=k1;
				else k=k2;
				qn->x=k*cosa+x;
				qn->y=k*cosb+y;
				qn->z=k*cosc+z-d;
	        }
	        else{
				if(k1*cosc+z-d>k2*cosc+z-d)k=k1;
				else k=k2;
				
				qn->x=k*cosa+x;
				qn->y=k*cosb+y;
				qn->z=k*cosc+z-d;
	        }
		}
		else qn->x=qn->y=qn->z=-1;
	}else{
		qn->x=(d-z)/cosc*cosa+x;
		qn->y=(d-z)/cosc*cosb+y;
		qn->z=0;
	}
}

void cal_ang(light_trace *qo,light_trace *qn,double r,double n1,double n2){
	double p,cosi;
	vector<double> dir(3);
	vector<double> pos(3);
	vector<double> nor(3);

	double x=qn->x,y=qn->y,z=qn->z;
	double cosa=qo->cosa,cosb=qo->cosb,cosc=qo->cosc;

	if(n1==n2){
		qn->cosa=cosa;
		qn->cosb=cosb;
		qn->cosc=cosc;
	}else{
		dir[0]=qo->cosa;
		dir[1]=qo->cosb;
		dir[2]=qo->cosc;

		pos[0]=qn->x;
		pos[1]=qn->y;
		pos[2]=qn->z;

		if(1){
			nor[0]=-r*x;
			nor[1]=-r*y;
			nor[2]=-r*z+1.0;
	    }
	    // else{
	    //  	nor[0]=r*x;
	    //  	nor[1]=r*y;
	    //  	nor[2]=r*z-1.0;
	    // }
//cout<<"r="<<r<<endl;
//cout<<setprecision(10)<<nor[0]<<" "<<nor[1]<<" "<<nor[2]<<" "<<endl;
	    cosi=dot(&nor,&dir);
	    //cout<<setprecision(10)<<"cosi="<<cosi<<endl;
	    
	    p=sqrt(n2*n2-n1*n1+n1*n1*cosi*cosi)-n1*cosi;
	    //cout<<setprecision(10)<<"p="<<p<<endl;
	    scalarmulit(&nor,p);
	    scalarmulit(&dir,n1);
	    add(&dir,&nor);
	    qn->cosa=dir[0]/n2;
	    qn->cosb=dir[1]/n2;
	    qn->cosc=dir[2]/n2;
	}
}

void add(vector<double>* temp1,vector<double>* temp2){
	int i;

	for(i=0;i<temp1->size();i++){
		(*temp1)[i]+=(*temp2)[i];
	}
}

double dot(vector<double>* temp1,vector<double>* temp2){
	int i;
	double sum=0;

	for(i=0;i<temp1->size();i++){
		sum+=(*temp1)[i]*(*temp2)[i];
	}

	return sum;
}

void scalarmulit(vector<double>* temp,double lambda){
	int i;

	for(i=0;i<temp->size();i++){
		(*temp)[i]=(*temp)[i]*lambda;
	}
}

//overload

void ltrace(light_trace *q,double wavelength,char mode)
{
	double r,n1,n2,t;
	light_trace *qn;
	int i;

	//cout<<surface[1]->getn(wavelength)<<endl;
	
	for( i = 1; i < surface.size()-1 ; i++){
	 	// cout<<"i="<<i<<endl;//
	 	qn=new light_trace;
	 	qn->next=NULL;
		r = surface[i]->getdata('r');
	 	n1 = surface[i-1]->getn(wavelength);
	 	n2 = surface[i]->getn(wavelength);
	 	if(i>1){
	 		t = surface[i-1]->getdata('t');
	 	}else{
	 		switch(mode){
	 			case 'p':t = -cal_lp();break;
	 			case 'o':t = surface[0]->getdata('t');
	 		}
	 	}

		cal_pos(q,qn,t,r);
		cal_ang(q,qn,r,n1,n2);

        if(qn->x==-1&&qn->y==-1&&qn->z==-1){
        	break;
        }
        q->next=qn;
        q=qn;
        // cout<<"r="<<r<<endl;//
        // lightout(q);//
	}
}