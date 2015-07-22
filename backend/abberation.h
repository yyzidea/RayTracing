double coma(double Px,double Py,double Hx,double Hy);
double sphere(double Py,double wavelength);
double lch(double Py);
double ych(double hc);
double curvature_t(double hc,double wavelength);
double curvature_s(double hc,double wavelength);
double cal_curv_t(double t1,double n1,double n2,double cosi1,double cosi2,double r);
double cal_curv_s(double s1,double n1,double n2,double cosi1,double cosi2,double r);
double astigmatism(double hc,double wavelength);
double distortion(double hc,char mode,double wavelength);

double coma(double Px,double Py,double Hx,double Hy){
	double l=surface[surface.size()-2]->getdata('t'),result;
	light_trace* m1=new light_trace;
	light_trace* m2=new light_trace;
	light_trace* n=new light_trace;
	fieldsample ftemp[3];

	ftemp[0].Px=0;
	ftemp[0].Py=0;
	ftemp[0].Hx=Hx;
	ftemp[0].Hy=Hy;

	ftemp[1].Px=Px;
	ftemp[1].Py=Py;
	ftemp[1].Hx=Hx;
	ftemp[1].Hy=Hy;

	ftemp[2].Px=-Px;
	ftemp[2].Py=-Py;
	ftemp[2].Hx=Hx;
	ftemp[2].Hy=Hy;

	switch(field.type){
		case 0:
			cal_ff(surface[0]->getdata('t')*tan(field.data[0]->y_field/180*M_PI),surface[0]->getdata('t')+cal_lp(),&ftemp[0],n);
			cal_ff(surface[0]->getdata('t')*tan(field.data[0]->y_field/180*M_PI),surface[0]->getdata('t')+cal_lp(),&ftemp[1],m1);
			cal_ff(surface[0]->getdata('t')*tan(field.data[0]->y_field/180*M_PI),surface[0]->getdata('t')+cal_lp(),&ftemp[2],m2);
			break;			

		case 1:
			cal_ff(field.data[0]->y_field,surface[0]->getdata('t')+cal_lp(),&ftemp[0],n);
			cal_ff(field.data[0]->y_field,surface[0]->getdata('t')+cal_lp(),&ftemp[1],m1);
			cal_ff(field.data[0]->y_field,surface[0]->getdata('t')+cal_lp(),&ftemp[2],m2);
			break;
	}

	ltrace(n,'p');
	ltrace(m1,'p');
	ltrace(m2,'p');

	n=last_trace_byp(n);
	m1=last_trace_byp(m1);
	m2=last_trace_byp(m2);

	result=0.5*(find_y(m1,l)+find_y(m2,l))-find_y(n,l);

	delete n,m1,m2;

	return result;
}

double sphere(double Py,double wavelength)
{
	double k,l1,lg,result,objl,linearfield;
	light_trace *p;

	objl=-surface[0]->getdata('t');
	
	lg=guass_imagl(objl,wavelength);

	fieldsample temp;
	temp.Px=0;
	temp.Py=Py;
	temp.Hx=0;
	temp.Hy=0;

	switch(field.type){
		case 0:
			linearfield=surface[0]->getdata('t')*tan(field.data[0]->y_field/180*M_PI);
			break;
		case 1:
			linearfield=field.data[0]->y_field;
			break;
	}

	p=new light_trace;
	switch(ini_state()){
		case 01:
			cal_if(0,&temp,p);
		case 10:

		case 11:
			cal_ff(linearfield,surface[0]->getdata('t')+cal_lp(),&temp,p);
			break;
	}
	
	//lightout(p);
	ltrace(p,wavelength,'p');
	p=last_trace_byp(p);

	result=(-p->y/p->cosb*p->cosc+p->z)-lg;
	
	delete p;

	return result;
}

double lch(double Py)
{
	return sphere(Py,FWAVE)-sphere(Py,CWAVE);
}

double ych(double hc)
{
	light_trace *p1,*p2;
	double k,yf,yc,height;
	fieldsample temp;

	temp.Px=0;
	temp.Py=0;
	temp.Hx=0;
	temp.Hy=1;

	switch(field.type){
		case 0:
			height=hc*surface[0]->getdata('t')*tan(field.data[0]->y_field/180*M_PI);
			break;
		case 1:
			height=hc*field.data[0]->y_field;
			break;
	}
	
	p1=new light_trace;
	cal_ff(height,surface[0]->getdata('t')+cal_lp(),&temp,p1); //入瞳需要普遍化
	ltrace(p1,CWAVE,'p');
	p1=last_trace_byp(p1);

	yc=find_y(p1,surface[surface.size()-2]->getdata('t'));

	p2=new light_trace;
	cal_ff(height,surface[0]->getdata('t')+cal_lp(),&temp,p2); //入瞳需要普遍化
	ltrace(p2,FWAVE,'p');
	p2=last_trace_byp(p2);

	yf=find_y(p2,surface[surface.size()-2]->getdata('t'));
	
	delete p1,p2;

	return yf-yc;
}

double curvature_t(double hc,double wavelength){
	light_trace *temp;
	fieldsample ftemp;
	int i=0;
	double t,result,height;

	ftemp.Px=0;
	ftemp.Py=0;
	ftemp.Hx=0;
	ftemp.Hy=1;

	switch(field.type){
		case 0:
			height=hc*surface[0]->getdata('t')*tan(field.data[0]->y_field/180*M_PI);
			break;
		case 1:
			height=hc*field.data[0]->y_field;
			break;
	}

	temp=new light_trace;
	cal_ff(height,surface[0]->getdata('t')+cal_lp(),&ftemp,temp);//入瞳需要普遍化
	ltrace(temp,'p');

	if(surface[0]->getdata('t')==-1){	//需要考虑入瞳在无穷远
		t=-1;
	}else{
		t=(-surface[0]->getdata('t')-temp->next->z)/temp->cosc;
		// cout<<"id=0"<<endl<<"t="<<t<<endl;
	}

	// cout<<"trace:"<<endl;
	// lightout(temp);
	for(i=1;i<surface.size()-1;i++){ 
	 	if(i==1){
	 		t=cal_curv_t(t,surface[i-1]->getn(wavelength),surface[i]->getn(wavelength),surfcosi1(temp,-cal_lp(),surface[i]->getdata('r')),surfcosi2(temp->next,surface[i]->getdata('r')),surface[i]->getdata('r'));	
	 	}else{
		 	t=cal_curv_t(t,surface[i-1]->getn(wavelength),surface[i]->getn(wavelength),surfcosi1(temp,surface[i-1]->getdata('t'),surface[i]->getdata('r')),surfcosi2(temp->next,surface[i]->getdata('r')),surface[i]->getdata('r'));//cosi1需要函数
	 	}
	 	
	 	// cout<<"id="<<i<<endl<<"\tt="<<t<<endl;
	 	if(i!=surface.size()-2){
	 		t=t-(surface[i]->getdata('t')-temp->z+temp->next->z)/temp->cosc;
	 	}
	 	// cout<<"\ttr="<<t<<endl;
		
		// cout<<endl;
		
		// cout<<"cosi1="<<surfcosi1(temp,surface[i-1]->getdata('t'),surface[i]->getdata('r'))<<endl<<"cosi2="<<surfcosi2(temp->next,surface[i]->getdata('r'))<<endl<<endl;

	 	temp=temp->next;

	 	// cout<<"trace:"<<endl;
	 	// lightout(temp);
	 	// cout<<setw(20)<<setfill('-')<<'-'<<endl;
	}

	// cout<<guass_imagl(-surface[0]->getdata('t')-surface[1]->getdata('t'),DWAVE)<<endl;

	result=temp->z+t*temp->cosc-guass_imagl(-surface[0]->getdata('t'),DWAVE);

	delete temp;

	return result;
}

double curvature_s(double hc,double wavelength){
	light_trace *temp;
	fieldsample ftemp;
	int i;
	double s,result,height;

	ftemp.Px=0;
	ftemp.Py=0;
	ftemp.Hx=0;
	ftemp.Hy=1;

	switch(field.type){
		case 0:
			height=hc*surface[0]->getdata('t')*tan(field.data[0]->y_field/180*M_PI);
			break;
		case 1:
			height=hc*field.data[0]->y_field;
			break;
	}

	temp=new light_trace;
	cal_ff(height,surface[0]->getdata('t')+cal_lp(),&ftemp,temp);//入瞳需要普遍化
	ltrace(temp,'p');

	if(surface[0]->getdata('t')==-1){	//需要考虑入瞳在无穷远
		s=-1;
	}else{
		s=(-surface[0]->getdata('t')-temp->next->z)/temp->cosc;
	}

	for(i=1;i<surface.size()-1;i++){ 
	 	if(i==1){
	 		s=cal_curv_s(s,surface[i-1]->getn(wavelength),surface[i]->getn(wavelength),surfcosi1(temp,-cal_lp(),surface[i]->getdata('r')),surfcosi2(temp->next,surface[i]->getdata('r')),surface[i]->getdata('r'));	
	 	}else{
		 	s=cal_curv_s(s,surface[i-1]->getn(wavelength),surface[i]->getn(wavelength),surfcosi1(temp,surface[i-1]->getdata('t'),surface[i]->getdata('r')),surfcosi2(temp->next,surface[i]->getdata('r')),surface[i]->getdata('r'));//cosi1需要函数
	 	}

	 	if(i!=surface.size()-2){
	 		s=s-(surface[i]->getdata('t')-temp->z+temp->next->z)/temp->cosc;
	 	}
	 	temp=temp->next;
	}
	
	result=temp->z+s*temp->cosc-guass_imagl(-surface[0]->getdata('t'),DWAVE);

	delete temp;

	return result; 
}

double cal_curv_t(double t1,double n1,double n2,double cosi1,double cosi2,double r){
    double q,f1;

    q=(n2*cosi2-n1*cosi1)*r;
    //cout<<"q="<<q<<"\tfi="<<f1<<endl;
    //cout<<"cosi1="<<cosi1<<"\tcosi2="<<cosi2<<endl;
    f1=n1*cosi1*cosi1/t1;

    return n2*cosi2*cosi2/(q+f1);
}


double cal_curv_s(double s1,double n1,double n2,double cosi1,double cosi2,double r){
	double q,f1;

    q=(n2*cosi2-n1*cosi1)*r;

    f1=n1/s1;

    return n2/(q+f1);
}

double astigmatism(double hc,double wavelength){
	return curvature_t(hc,wavelength)-curvature_s(hc,wavelength);
}

double distortion(double hc,char mode,double wavelength){
	double hg,result;
	light_trace *temp;
	fieldsample ftemp;
	double height;

	ftemp.Px=0;
	ftemp.Py=0;
	ftemp.Hx=0;
	ftemp.Hy=1;

	switch(field.type){
		case 0:
			height=hc*surface[0]->getdata('t')*tan(field.data[0]->y_field/180*M_PI);
			break;
		case 1:
			height=hc*field.data[0]->y_field;
			break;
	}

	hg=guass_imagh(-surface[0]->getdata('t'),height,wavelength);

	temp=new light_trace;
	cal_ff(height,surface[0]->getdata('t')+cal_lp(),&ftemp,temp);//入瞳需要普遍化
	ltrace(temp,'p');
	temp=last_trace_byp(temp);
	
	switch(mode){
		case 'a':
		//cout<<hg<<"=="<<endl;//
			result=find_y(temp,guass_imagl(-surface[0]->getdata('t'),wavelength))-hg;break;
		case 'r':
			result=(find_y(temp,guass_imagl(-surface[0]->getdata('t'),wavelength))-hg)/hg*100;break;
	}

	delete temp;

	return result;
}