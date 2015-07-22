light_trace* last_trace_byc(double cx,double cy);
light_trace* last_trace_byp(light_trace* temp);
int ctoi_x(double cx);
int ctoi_y(double cy);
light_trace* find_trace(light_trace* temp,int n);
double find_x(light_trace* temp,double z);
double find_y(light_trace* temp,double z);
double cal_lp(void);
double surfcosi1(light_trace *origin,double d,double r);
double surfcosi2(light_trace *origin,double r);

void getlight(double data1);
int ini_state();
int findsto();
double getpupil_l();

void clearSurface();
void clearField();
void clearWave();
void clearLight();
void clearGlass();


//for debug
void sampleout(fieldsample* temp);
void lightout(light_trace* temp);
void waveout(vector<wavelengthdata*> temp);

void waveout(vector<wavelengthdata*> *temp){
	int i;
	
	for(i=0;i<temp->size();i++){
		cout<<"id="<<i<<endl;
		cout<<"wavelength="<<(*temp)[i]->wavelength<<endl;
		cout<<"weight="<<(*temp)[i]->weight<<endl;
		cout<<endl;
	}
}

double cal_lp(void){
    int i=0;
    double lp;
    while(i<surface.size()){
	    if(surface[i++]->getdata('m')==2)
			break;
    }

    //需要补充
	//lp=surface[i-1]->getdata('t');
	return 0;
}

light_trace* last_trace_byp(light_trace* temp){
	while(temp->next!=NULL)temp=temp->next;
	return temp;
}

light_trace* find_trace(light_trace* temp,int n){
	int i;

	if(n<2){
		return temp;
	}

	for(i=0;temp->next!=NULL&&i<n-1;i++){
		temp=temp->next;
	}

	return temp;
}

light_trace* last_trace_byc(double cx,double cy){
	int i,j;

	light_trace* p;

	i=ctoi_x(cx);
	j=ctoi_y(cy);

	p=(*light[i])[j];

	while(p->next!=NULL){
		p=p->next;
	}
}

int ctoi_x(double cx){
	return floor(cx*setting.density+0.1);
}

int ctoi_y(double cy){
	return floor(cy*setting.density+0.1);
}

double find_x(light_trace* temp,double z){
	return temp->cosa/temp->cosc*(z-(temp->z))+temp->x;
}

double find_y(light_trace* temp,double z){
	return temp->cosb/temp->cosc*(z-(temp->z))+temp->y;
}

double surfcosi1(light_trace *origin,double d,double r){
	vector<double> dir(3);
	vector<double> pos(3);
	vector<double> nor(3);
	light_trace* temp;

	temp=new light_trace;
	temp->next=NULL;
	cal_pos(origin,temp,d,r);

	double x=temp->x,y=temp->y,z=temp->z;

	// cout<<"temp:"<<endl;
	
	// lightout(temp);
	dir[0]=origin->cosa;
	dir[1]=origin->cosb;
	dir[2]=origin->cosc;

	nor[0]=-r*x;
	nor[1]=-r*y;
	nor[2]=-r*z+1.0;
	
	return dot(&nor,&dir);
}

double surfcosi2(light_trace *origin,double r){
	vector<double> dir(3);
	vector<double> nor(3);

	double x=origin->x,y=origin->y,z=origin->z;

	dir[0]=origin->cosa;
	dir[1]=origin->cosb;
	dir[2]=origin->cosc;

	nor[0]=-r*x;
	nor[1]=-r*y;
	nor[2]=-r*z+1.0;

	return dot(&nor,&dir);
}

void sampleout(fieldsample* temp){
	cout<<"Hx="<<temp->Hx<<endl;
	cout<<"Hy="<<temp->Hy<<endl;
	cout<<"Px="<<temp->Px<<endl;
	cout<<"Py="<<temp->Py<<endl;
	cout<<endl;
}

void lightout(light_trace* temp){
	cout<<"x="<<temp->x<<endl;
	cout<<"y="<<temp->y<<endl;
	cout<<"z="<<temp->z<<endl;
	cout<<setprecision(10)<<"cosa="<<temp->cosa<<endl;
	cout<<setprecision(10)<<"cosb="<<temp->cosb<<endl;
	cout<<setprecision(10)<<"cosc="<<temp->cosc<<endl;
	cout<<endl;
}

void getlight(double data1){
	int i,j,state;
	light_trace* temp;
	char mode;

	state=ini_state();

	for(i=0;i<sample.size();i++){
		light.push_back(new vector<light_trace*>);
		for(j=0;j<(*sample[i]).size();j++){
			temp=new light_trace;
			switch(state){
				//sample 调入
				case 01:
					cal_if(data1,(*sample[i])[j],temp);
					mode='p';
					break;
				case 10:
					cal_fi(setting.linearfield,surface[0]->getdata('t'),data1,(*sample[i])[j],temp);//入瞳距需要普遍化
					mode='o';
					break;
				case 11:
					cal_ff(setting.linearfield,surface[0]->getdata('t')-cal_lp(),(*sample[i])[j],temp);//同上，需要普遍化
					mode='p';
					break;
			}
			ltrace(temp,mode);
			light[i]->push_back(temp);
		}
	}
}

int ini_state(){
	double tobj=surface[0]->getdata('t');
	double tpupil=getpupil_l();

	if(tobj==-1&&tpupil!=-1){
		return 01;
	}
	else if(tobj=-1&&tpupil==-1){
		return 10;
	}
	else if(tobj=-1&&tpupil!=-1){
		return 11;
	}

	return 00;
}


double getpupil_l(){
	double result;

	result=surface[findsto()]->getdata('t');
	return result;
}

int findsto(){
	int i;

	for(i=0;i<surface.size();i++){
		if(surface[i]->getdata('m')==2){
			return i;
		}
	}

	return -1;
}

void clearSurface(){
	surface.clear();
}

void clearField(){
	field.data.clear();
}

void clearWave(){
	wave.clear();
}

void clearLight(){
	light.clear();
}

void clearGlass(){
	glass.clear();
}

void addSurface(int row,surfdata* newsurf){
	int i;
	surfdata* temp;
	
	surface.push_back(newsurf);

	if(row==surface.size()-1){
		temp=surface[row];
		surface[row]=surface[row+1];
		surface[row+1]=temp;
	}else{
		for(i=surface.size()-1;i>row+1;i--){
			temp=surface[i];
			surface[i]=surface[i-1];
			surface[i-1]=temp;
		}	
	}
}

void addField(int row,fielddata* newfield){
	fielddata* temp;
	int i;

    field.data[field.data.size()-1]=newfield;

	for(i=field.data.size()-1;i>row+1;i--){
		temp=field.data[i];
		field.data[i]=field.data[i-1];
		field.data[i-1]=temp;
	}

}

void addWavelength(int row,wavelengthdata* newwavelength){
	wavelengthdata* temp;
	int i;

    wave[wave.size()-1]=newwavelength;

	for(i=wave.size()-1;i>row+1;i--){
		temp=wave[i];
		wave[i]=wave[i-1];
		wave[i-1]=temp;
	}	
}

void delSurface(int row){
	int i;
	if(row>0&&surface.size()>2){
        if(row==1&&surface[row]->getdata('m')==2&&surface[row+1]->getdata('m')!=3){
			surface[row+1]->type=1;
		}else{
			if(row==surface.size()-1&&row!=1){
				surface[row-1]=surface[row];
			}else{
				for(i=row;i<surface.size()-1;i++){
					surface[i]=surface[i+1];
				}
			}
			surface.pop_back();
		}
	}
}

void delField(int row){
	int i;
	fielddata* newfield;
    newfield=new fielddata;

    if(field.field_used>1){
		for(i=row;i<field.data.size()-1;i++){
			field.data[i]=field.data[i+1];
		}

		field.data[field.data.size()-1]=newfield;
		field.field_used--;
    }
}

void delWavelength(int row){
	int i;
	wavelengthdata* newwavelength;
    newwavelength=new wavelengthdata;

    if(setting.wave_used>1){	
		for(i=row;i<wave.size()-1;i++){
			wave[i]=wave[i+1];
		}

		wave[wave.size()-1]=newwavelength;
        setting.wave_used--;
    }
}
