general_setting setting;
vector<glassdata*> glass;
vector<surfdata*> surface;
fieldsetting field;
vector<wavelengthdata*> wave;

void readfile(string filename){
    ifstream in_zmx(filename.c_str());
    string temp,bash;
    surfdata *newsurf;
    int i,flag,begin;
    istringstream sin;

    flag=1;
    while(in_zmx.good()&&!in_zmx.eof()){
        if(flag){
        	getline(in_zmx,temp);
        }
		flag=1;

		if(temp.substr(0,4)=="GCAT"){
			istringstream sin(temp);
			sin>>bash;
			sin>>setting.glass_catalog;
		}

		if(temp.substr(0,4)=="FTYP"){
			istringstream sin(temp);
			sin>>bash;
			sin>>field.type;
		}
		
	//retrieve all field data
		if(temp.substr(0,4)=="XFLN"){
			istringstream sin(temp);
			sin>>bash;
			for(i=0;sin.good();i++){
				field.data.push_back(new fielddata);
				sin>>field.data[i]->x_field;
			}
		}

		if(temp.substr(0,4)=="YFLN"){
			istringstream sin(temp);
			sin>>bash;
			for(i=0;sin.good();i++){
				
				sin>>field.data[i]->y_field;
			}
		}

		if(temp.substr(0,4)=="FWGN"){
			istringstream sin(temp);
			sin>>bash;
			for(i=0;sin.good();i++){
				
				sin>>field.data[i]->weight;
			}
		}

		if(temp.substr(0,4)=="VDXN"){
			istringstream sin(temp);
			sin>>bash;
			for(i=0;sin.good();i++){
				
				sin>>field.data[i]->vdx;
			}
		}

		if(temp.substr(0,4)=="VDYN"){
			istringstream sin(temp);
			sin>>bash;
			for(i=0;sin.good();i++){
				
				sin>>field.data[i]->vdy;
			}
		}

		if(temp.substr(0,4)=="VCXN"){
			istringstream sin(temp);
			sin>>bash;
			for(i=0;sin.good();i++){
				
				sin>>field.data[i]->vcx;
			}
		}

		if(temp.substr(0,4)=="VCYN"){
			istringstream sin(temp);
			sin>>bash;
			for(i=0;sin.good();i++){
				
				sin>>field.data[i]->vcy;
			}
		}

		if(temp.substr(0,4)=="VANN"){
			istringstream sin(temp);
			sin>>bash;
			for(i=0;sin.good();i++){
				
				sin>>field.data[i]->van;
			}
		}

	//retrieve the fields in use
		if(temp.substr(0,4)=="XFLD"){
			istringstream sin(temp);
			sin>>bash;
			for(i=0;sin>>bash;i++);
			field.field_used=i;
		}

		if(temp.substr(0,4)=="WAVL"){
			istringstream sin(temp);
			sin>>bash;
			for(i=0;sin>>bash;i++);
			setting.wave_used=i;
		}

		if(temp.substr(0,4)=="PWAV"){
			istringstream sin(temp);
			sin>>bash;
			sin>>bash;
			setting.primary_wave=(int)strtod(bash.c_str(),NULL);
		}

	//retrieve the semi-diameter of pupil
		if(temp.substr(0,4)=="ENPD"){
			istringstream sin(temp);
			sin>>bash;
			sin>>bash;
			setting.hp=strtod(bash.c_str(),NULL)/2;
		}

	//retrieve the surface data
		if(temp.substr(0,4)=="SURF"){
			newsurf = new surfdata();
			newsurf->curv=0;
			newsurf->glassname="AIR";
			newsurf->sDiameter=0;
			newsurf->thickness=0;
			newsurf->type=0;
			
        	getline(in_zmx,temp);
        	while(temp.substr(0,2)=="  "){
				if(temp.substr(2,4)=="CURV"){
					istringstream sin(temp);
					sin>>bash;
					sin>>bash;
					if(bash=="INFINITY"){
						newsurf->curv=-1;
					}else{
						newsurf->curv=strtod(bash.c_str(),NULL);
					}
				}
            	else if(temp.substr(2,4)=="DISZ"){
            		istringstream sin(temp);
					sin>>bash;
					sin>>bash;
            		if(bash=="INFINITY"){
						newsurf->thickness=-1;
					}else{
						newsurf->thickness=strtod(bash.c_str(),NULL);
					}
            	}
				else if(temp.substr(2,4)=="GLAS"){
                	istringstream sin(temp);
					sin>>bash;
                	sin>>newsurf->glassname;
            	}
            	else if(temp.substr(2,4)=="DIAM"){
                	istringstream sin(temp);
					sin>>bash;
					sin>>bash;
					if(bash=="INFINITY"){
						newsurf->sDiameter=-1;
					}else{
						newsurf->sDiameter=strtod(bash.c_str(),NULL);
					}
            	}
            	else if(temp.substr(2,4)=="STOP"){
            		newsurf->type=2;
            	}
            	if(in_zmx.eof())break;
            	getline(in_zmx,temp);
        	}
        	surface.push_back(newsurf);
			flag=0;
        }
	
		if(temp.substr(0,4)=="WAVM"){
			istringstream sin(temp);
			wavelengthdata *newwave;
			newwave=new wavelengthdata;

			sin>>bash;
			sin>>bash;
			sin>>bash;
			newwave->wavelength=strtod(bash.c_str(),NULL);
			sin>>bash;
			newwave->weight=strtod(bash.c_str(),NULL);

			wave.push_back(newwave);
		}
	}



	//error info and close file
	if(in_zmx.bad()){
        cout<<"The file can not be opened!"<<endl;
	}
	else{
		surface[0]->type = 1;
		surface[surface.size() - 1]->type = 3;
	}
    in_zmx.close();
}

void getglass(string filename){
	ifstream in_agf(filename.c_str());
    string temp,bash;
    glassdata *newglass;
    int i,flag;
    
    istringstream sin;

    flag=1;
    
    while(in_agf.good()&&!in_agf.eof()){
		if(flag){
			getline(in_agf,temp);
		}
		flag=1;
		
		if(temp.substr(0,2)=="NM"){
			newglass = new glassdata();
			istringstream sin(temp);
			sin>>bash;
			sin>>newglass->name;
			sin>>bash;
			sin>>bash;
			sin>>bash;
			newglass->Nd=strtod(bash.c_str(),NULL);
			sin>>bash;
			newglass->Vd=strtod(bash.c_str(),NULL);

			getline(in_agf,temp);
			while(temp.substr(0,2)!="NM"){
				//get the coefficient of schott equation
				if(temp.substr(0,2)=="CD"){
					istringstream sin(temp);
					sin>>bash;
					sin>>bash;
					newglass->A0=strtod(bash.c_str(),NULL);
					sin>>bash;
					newglass->A1=strtod(bash.c_str(),NULL);
					sin>>bash;
					newglass->A2=strtod(bash.c_str(),NULL);
					sin>>bash;
					newglass->A3=strtod(bash.c_str(),NULL);
					sin>>bash;
					newglass->A4=strtod(bash.c_str(),NULL);
					sin>>bash;
					newglass->A5=strtod(bash.c_str(),NULL);
				}

				getline(in_agf,temp);
				if(in_agf.eof()){
					break;
				}
			}
			glass.push_back(newglass);
			flag=0;
		}
	}

	if(in_agf.bad()){
        cout<<"The file can not be opened!"<<endl;
    }
    in_agf.close();
}

double getnd(string glassname){
	if(glassname=="AIR")return 1.0; 
	
	for(int i=0;i<glass.size();i++){
		if(glass[i]->name==glassname){
			return glass[i]->Nd;
		}
	}
	
	return -1;
}

double getvd(string glassname){
	if(glassname=="AIR")return 0; 
	
	for(int i=0;i<glass.size();i++){
		if(glass[i]->name==glassname){
			return glass[i]->Vd;
		}
	}
	
	return -1;
}

double getid(string glassname){
	for(int i=0;i<glass.size();i++){
		if(glass[i]->name==glassname){
			return i;
		}
	}
	
	return -1;
}

double cal_glassn(string glassname,double wavelength){
	double A0,A1,A2,A3,A4,A5,n;
	int i;
	
	if(glassname=="AIR")return 1.0;
	
	i=getid(glassname);

	A0=glass[i]->A0;
	A1=glass[i]->A1;
	A2=glass[i]->A2;
	A3=glass[i]->A3;
	A4=glass[i]->A4;
	A5=glass[i]->A5;
	
	n=sqrt(A0+A1*pow(wavelength,2)+A2*pow(wavelength,-2)+A3*pow(wavelength,-4)+A4*pow(wavelength,-6)+A5*pow(wavelength,-8));

	if(i==-1){
		cout<<"Can't find this glass in current catalog!"<<endl;
		return -1;
	}

	return n;
}