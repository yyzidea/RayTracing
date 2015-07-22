double guass_imagl(double objl,double wavelength);
double guass_imagh(double objl,double height,double wavelength);
double cal_lg(double objl,double n1,double n2,double r);
double cal_magnif(double objl,double n1,double n2,double r,char mode);

//无穷远情况未处理
double guass_imagl(double objl,double wavelength){
	int i;
	//cout<<"ini objl="<<objl<<endl;
	for(i=1;i<surface.size()-1;i++){
		objl=cal_lg(objl,surface[i-1]->getn(wavelength),surface[i]->getn(wavelength),surface[i]->getdata('r'));
		if(i<surface.size()-2){
			objl=objl-surface[i]->getdata('t');
		}
	}
	return objl;
}

double guass_imagh(double objl,double height,double wavelength){
	int i;

	for(i=1;i<surface.size()-1;i++){
		//cout<<cal_magnif(objl,surface[i-1]->getn(wavelength),surface[i]->getn(wavelength),surface[i]->getdata('r'),'b')<<endl;
		height=cal_magnif(objl,surface[i-1]->getn(wavelength),surface[i]->getn(wavelength),surface[i]->getdata('r'),'b')*height;
		objl=cal_lg(objl,surface[i-1]->getn(wavelength),surface[i]->getn(wavelength),surface[i]->getdata('r'));
	}

	return height;
}

double cal_lg(double objl,double n1,double n2,double r){
	double q,f1;

	q=(n2-n1)*r;
	f1=n1/objl;

	return n2/(q+f1);
}

double cal_magnif(double objl,double n1,double n2,double r,char mode){
	switch(mode){
		case 'b':
			return n1*cal_lg(objl,n1,n2,r)/n2/objl;
		case 'a':
			return n1*cal_lg(objl,n1,n2,r)*cal_lg(objl,n1,n2,r)/n2/objl/objl;
		case 'c':
			return objl/cal_lg(objl,n1,n2,r);
	}
}

double guass_f(){
	int i;
	double f=surface[1]->getn(DWAVE)/((surface[1]->getn(DWAVE)-1)*surface[1]->getdata('r')),l1,l2;

	l2=f;
	for(i=2;i<surface.size()-1;i++){
		l1=l2-surface[i-1]->getdata('t');
		l2=cal_lg(l1,surface[i-1]->getn(DWAVE),surface[i]->getn(DWAVE),surface[i]->getdata('r'));
		// cout<<"id="<<i<<"\tl2="<<l2<<endl;
		f=f*l2/l1;
	}

	return f;
}

