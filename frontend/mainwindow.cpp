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

#include "mainwindow.h"
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    createActions();
    createConnections();
    editor_surface->setCurrentCell(0,0);
    editor_field->setCurrentCell(0,0);
    editor_wavelength->setCurrentCell(0,0);
}


//MainWindow setup
void MainWindow::createActions(){
    action_New->setShortcut(QKeySequence::New);
    action_Open->setShortcut(QKeySequence::Open);
    action_Save->setShortcut(QKeySequence::Save);
    action_Save_as->setShortcut(QKeySequence::SaveAs);

    action_Quit = new QAction(tr("&Quit"),this);
    action_Quit->setShortcut(QKeySequence::Quit);

    connect(action_Open,SIGNAL(triggered()),this,SLOT(openFile()));
    connect(action_Exit,SIGNAL(triggered()),this,SLOT(close()));
    
}

void MainWindow::createConnections(){

    connect(editor_surface,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(surfaceDataChanged()));
    connect(editor_wavelength,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(wavelengthDataChanged()));
    connect(editor_field,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(fieldDataChanged()));
    connect(add_surf,SIGNAL(clicked()),this,SLOT(addSurfaceData()));
    connect(add_field,SIGNAL(clicked()),this,SLOT(addFieldData()));
    connect(add_wavelength,SIGNAL(clicked()),this,SLOT(addWavelengthData()));
    connect(del_surf,SIGNAL(clicked()),this,SLOT(delSurfaceData()));
    connect(del_field,SIGNAL(clicked()),this,SLOT(delFieldData()));
    connect(del_wavelength,SIGNAL(clicked()),this,SLOT(delWavelengthData()));
    connect(cal,SIGNAL(clicked()),this,SLOT(calculate()));
    connect(spin_pupil,SIGNAL(valueChanged(double)),this,SLOT(pupilDataChanged(double)));

}

void MainWindow::setupMenus(){

}

//sync data
void MainWindow::syncSurfaceData(int row,int col,QVariant data){
    switch(col){
        case 0:
            if(data=="INFINITY"){
                surface[row]->curv=0;
            }else{
                surface[row]->curv=1/data.toDouble();
            }
            break;
        case 1:
            if(data=="INFINITY"){
                surface[row]->thickness=-1;
            }else{
                surface[row]->thickness=data.toDouble();
            }
            break;
        case 2:surface[row]->glassname=data.toString().toStdString();break;
        case 3:surface[row]->sDiameter=data.toDouble();break;
    }
}

void MainWindow::syncFieldData(int row,int col,QVariant data){
    switch(col){
        case 0:field.data[row]->x_field=data.toDouble();break;
        case 1:field.data[row]->y_field=data.toDouble();break;
        case 2:field.data[row]->weight=data.toDouble();break;
        case 3:field.data[row]->vdx=data.toDouble();break;
        case 4:field.data[row]->vdy=data.toDouble();break;
        case 5:field.data[row]->vcx=data.toDouble();break;
        case 6:field.data[row]->vcy=data.toDouble();break;
        case 7:field.data[row]->van=data.toDouble();break;
    }
}

void MainWindow::syncWavelengthData(int col,int row,QVariant data){
    switch(col){
        case 0:wave[row]->wavelength=data.toDouble();break;
        case 1:wave[row]->weight=data.toDouble();break;
    }
}

void MainWindow::loadData(){
    int i,count;

    QTableWidgetItem *itemSurface;
    QTableWidgetItem *itemField;
    QTableWidgetItem *itemWavelength;
    QTableWidgetItem *itemGlass;

    loadOrNot=true;

    editor_surface->setRowCount(surface.size());
    for(i = 0;i < surface.size();i++){
        itemSurface=new QTableWidgetItem;
        if(surface[i]->curv==0){
            itemSurface->setData(Qt::DisplayRole,"INFINITY");
        }else{
            itemSurface->setData(Qt::DisplayRole,1/surface[i]->curv);
        }
        editor_surface->setItem(i,0,itemSurface);

        itemSurface=new QTableWidgetItem;
        if(surface[i]->thickness==-1){
            itemSurface->setData(Qt::DisplayRole,"INFINITY");
        }else{
            itemSurface->setData(Qt::DisplayRole,surface[i]->thickness);
        }
        editor_surface->setItem(i,1,itemSurface);

        itemSurface=new QTableWidgetItem;
        QString temp(QString(surface[i]->glassname.c_str()));
        itemSurface->setData(Qt::DisplayRole,temp);
        editor_surface->setItem(i,2,itemSurface);

        itemSurface=new QTableWidgetItem;
        itemSurface->setData(Qt::DisplayRole,surface[i]->sDiameter);
        editor_surface->setItem(i,3,itemSurface);

        switch(surface[i]->type){
            case 1:
                itemSurface=new QTableWidgetItem;
                itemSurface->setData(Qt::DisplayRole,"Object");
                editor_surface->setVerticalHeaderItem(i,itemSurface);
                break;
            case 2:
                itemSurface=new QTableWidgetItem;
                itemSurface->setData(Qt::DisplayRole,"Stop");
                editor_surface->setVerticalHeaderItem(i,itemSurface);
                break;
            case 3:
                itemSurface=new QTableWidgetItem;
                itemSurface->setData(Qt::DisplayRole,"Image");
                editor_surface->setVerticalHeaderItem(i,itemSurface);
                break;
            default:
                itemSurface=new QTableWidgetItem;
                itemSurface->setData(Qt::DisplayRole,i);
                editor_surface->setVerticalHeaderItem(i,itemSurface);
                break;
        }
    }

    editor_field->setRowCount(field.field_used);
    for(i = 0;i < field.data.size();i++){
        itemField=new QTableWidgetItem;
        itemField->setData(Qt::DisplayRole,field.data[i]->x_field);
        editor_field->setItem(i,0,itemField);
        itemField=new QTableWidgetItem;
        itemField->setData(Qt::DisplayRole,field.data[i]->y_field);
        editor_field->setItem(i,1,itemField);
        itemField=new QTableWidgetItem;
        itemField->setData(Qt::DisplayRole,field.data[i]->weight);
        editor_field->setItem(i,2,itemField);
        itemField=new QTableWidgetItem;
        itemField->setData(Qt::DisplayRole,field.data[i]->vdx);
        editor_field->setItem(i,3,itemField);
        itemField=new QTableWidgetItem;
        itemField->setData(Qt::DisplayRole,field.data[i]->vdy);
        editor_field->setItem(i,4,itemField);
        itemField=new QTableWidgetItem;
        itemField->setData(Qt::DisplayRole,field.data[i]->vcx);
        editor_field->setItem(i,5,itemField);
        itemField=new QTableWidgetItem;
        itemField->setData(Qt::DisplayRole,field.data[i]->vcy);
        editor_field->setItem(i,6,itemField);
        itemField=new QTableWidgetItem;
        itemField->setData(Qt::DisplayRole,field.data[i]->van);
        editor_field->setItem(i,7,itemField);
    }

    editor_wavelength->setRowCount(setting.wave_used);
    for(i = 0;i< wave.size();i++){
        itemWavelength=new QTableWidgetItem;
        itemWavelength->setData(Qt::DisplayRole,wave[i]->wavelength);
        editor_wavelength->setItem(i,0,itemWavelength);
        itemWavelength=new QTableWidgetItem;
        itemWavelength->setData(Qt::DisplayRole,wave[i]->weight);
        editor_wavelength->setItem(i,1,itemWavelength);
    }

    spin_pupil->setValue(setting.hp*2);

    loadOrNot=false;
}

//file input and output
void MainWindow::loadGlass(){
    string filename;
    filename="glasscat/";
    filename.append(setting.glass_catalog);
    filename.append(".AGF");
    getglass(filename);
    while(glass.size()==0){
        QString filenameNew=QFileDialog::getOpenFileName(this,tr("Open another AGF file"),".",tr("Glass Catalog files (*.AGF)"));
        getglass(filenameNew.toStdString());
    }
}

void MainWindow::openFile(){
    if(okToContinue()){
        QString filename=QFileDialog::getOpenFileName(this,tr("Open ZMX file"),".",tr("ZEMAX files (*.ZMX)"));
        if(!filename.isEmpty()){
            clearSurface();
            clearField();
            clearWave();
            readfile(filename.toStdString());
            loadData();
            loadGlass();
            editor_surface->setCurrentCell(0,0);
            editor_field->setCurrentCell(0,0);
            editor_wavelength->setCurrentCell(0,0);
        }
    }
}

void MainWindow::newFile(){

}

bool MainWindow::save(){
    if(curFile.isEmpty()){
        return saveAs();
    }else{
        return true; //savefile();
    }
}

bool MainWindow::saveAs(){
    return true;
}

//response to actions
//modify the item
QTableWidgetSelectionRange MainWindow::selectRange(QTableWidget* temp){
    QList<QTableWidgetSelectionRange> ranges=temp->selectedRanges();
    if(ranges.isEmpty())
        return QTableWidgetSelectionRange();
    return ranges.first();    
}

void MainWindow::surfaceDataChanged(){
    // QTableWidgetSelectionRange* temp=selectRange(editor_surface);
    int row,col;

    QTableWidgetItem* tempItem;
    if(!loadOrNot){
        col=editor_surface->currentColumn();
        row=editor_surface->currentRow();
        tempItem=editor_surface->currentItem();

        syncSurfaceData(row,col,tempItem->data(Qt::DisplayRole));
    }
}

void MainWindow::wavelengthDataChanged(){
    int row,col;

    QTableWidgetItem* tempItem;
    if(!loadOrNot){
        col=editor_wavelength->currentColumn();
        row=editor_wavelength->currentRow();
        tempItem=editor_wavelength->currentItem();

        syncWavelengthData(row,col,tempItem->data(Qt::DisplayRole));
    }
}

void MainWindow::fieldDataChanged(){
    int row,col;

    QTableWidgetItem* tempItem;
    if(!loadOrNot){
        col=editor_field->currentColumn();
        row=editor_field->currentRow();
        tempItem=editor_field->currentItem();

        syncFieldData(row,col,tempItem->data(Qt::DisplayRole));
    }
}

void MainWindow::pupilDataChanged(double value){
    if(!loadOrNot){
        setting.hp=value/2;
    }
}

//add and delete data

void MainWindow::addSurfaceData(){
    int row,col;
    surfdata* temp;
    temp=new surfdata;

    row=editor_surface->currentRow();
    col=editor_surface->currentColumn();

    addSurface(row,temp);
    loadData();
}

void MainWindow::addFieldData(){
    int row,col;
    fielddata* temp;
    temp=new fielddata;

    row=editor_field->currentRow();
    col=editor_field->currentColumn();

    addField(row,temp);
    field.field_used++;
    loadData();
}

void MainWindow::addWavelengthData(){
    int row,col;
    wavelengthdata* temp;
    temp=new wavelengthdata;

    row=editor_wavelength->currentRow();
    col=editor_wavelength->currentColumn();

    addWavelength(row,temp);
    setting.wave_used++;
    loadData();
}

void MainWindow::delSurfaceData(){
    int row;

    row=editor_surface->currentRow();
    
    delSurface(row);
    loadData(); 
}

void MainWindow::delFieldData(){
    int row;

    row=editor_field->currentRow();
    
    delField(row);
    loadData(); 
}

void MainWindow::delWavelengthData(){
    int row;

    row=editor_wavelength->currentRow();
    
    delWavelength(row);
    loadData(); 
}

void MainWindow::calculate(){
    QString result;
    stringstream ss;
    string temp;

    result.append(tr("<html><body><h1>Standard output:</h1>"));

    result.append(tr("<h2>Guassian Optics result:</h2>"));
    
    switch(field.type){
        case 0:
            result.append(tr("<p>Paraxial Image Height:"));
            ss<<guass_imagh(-surface[0]->getdata('t'),surface[0]->getdata('t')*tan(field.data[0]->y_field/180*M_PI),DWAVE);
            ss>>temp;
            ss.clear();
            result.append(tr(temp.c_str()));
            result.append(tr("</p>"));
            break;
        case 1:
            result.append(tr("<p>Paraxial Image Height:"));
            ss<<guass_imagh(-surface[0]->getdata('t'),field.data[0]->y_field,DWAVE);
            ss>>temp;
            ss.clear();
            result.append(tr(temp.c_str()));
            result.append(tr("</p>"));
            break;
    }

    result.append(tr("<p>Effective Focal Length:"));
    ss<<guass_f();
    ss>>temp;
    ss.clear();
    result.append(tr(temp.c_str()));
    result.append(tr("</p>"));


    result.append(tr("<h2>Abberation result:</h2>"));
    
    result.append(tr("<h4>Sphere Abberation and<br>Longitudinal Chromatic Abberation:</h4>"));

    result.append(tr("<table><tr><td></td><th>Full Aperture</th><th>0.707 Aperture</th></tr><tr><th>Sphere Abberation:</th><td>"));

    ss<<sphere(1,DWAVE);
    ss>>temp;
    ss.clear();
    result.append(tr(temp.c_str()));

    result.append(tr("</td><td>"));

    ss<<sphere(0.707,DWAVE);
    ss>>temp;
    ss.clear();
    result.append(tr(temp.c_str()));

    result.append(tr("</td></tr><tr><th>Longitudinal Chromatic Abberation</th><td>"));

    ss<<lch(1);
    ss>>temp;
    ss.clear();
    result.append(tr(temp.c_str()));

    result.append(tr("</td><td>"));

    ss<<lch(0.707);
    ss>>temp;
    ss.clear();
    result.append(tr(temp.c_str()));

    result.append(tr("</td></tr></table>"));


    result.append(tr("<h4>Tag. Coma:</h4>"));
    
    result.append(tr("<table><tr><td></td><th>Full Aperture</th><th>0.707 Aperture</th></tr><tr><th>Full Field</th><td>"));    

    ss<<coma(0,1,0,1);
    ss>>temp;
    ss.clear();
    result.append(tr(temp.c_str()));

    result.append(tr("</td><td>"));

    ss<<coma(0,0.707,0,1);
    ss>>temp;
    ss.clear();
    result.append(tr(temp.c_str()));

    result.append(tr("</td></tr><tr><th>0.707 Field</th><td>"));

    ss<<coma(0,1,0,0.707);
    ss>>temp;
    ss.clear();
    result.append(tr(temp.c_str()));

    result.append(tr("</td><td>"));

    ss<<coma(0,0.707,0,0.707);
    ss>>temp;
    ss.clear();
    result.append(tr(temp.c_str()));

    result.append(tr("</td></tr></table>"));


    result.append(tr("<h4>Sag. Coma:</h4>"));
    
    result.append(tr("<table><tr><td></td><th>Full Aperture</th><th>0.707 Aperture</th></tr><tr><th>Full Field</th><td>"));    

    ss<<coma(1,0,0,1);
    ss>>temp;
    ss.clear();
    result.append(tr(temp.c_str()));

    result.append(tr("</td><td>"));

    ss<<coma(0.707,0,0,1);
    ss>>temp;
    ss.clear();
    result.append(tr(temp.c_str()));

    result.append(tr("</td></tr><tr><th>0.707 Field</th><td>"));

    ss<<coma(1,0,0,0.707);
    ss>>temp;
    ss.clear();
    result.append(tr(temp.c_str()));

    result.append(tr("</td><td>"));

    ss<<coma(0.707,0,0,0.707);
    ss>>temp;
    ss.clear();
    result.append(tr(temp.c_str()));

    result.append(tr("</td></tr></table>"));


    result.append(tr("<h4>Lateral Chromatic Abberation:</h4>"));
    
    result.append(tr("<table><tr><th>Full Field</th><th>0.707 Field</th></tr><tr><td>"));  

    ss<<ych(1);
    ss>>temp;
    ss.clear();
    result.append(tr(temp.c_str()));

    result.append(tr("</td><td>"));

    ss<<ych(0.707);
    ss>>temp;
    ss.clear();
    result.append(tr(temp.c_str()));

    result.append(tr("</td></tr></table>"));


    result.append(tr("<h4>Curvature and Astigmatism:</h4>"));

    result.append(tr("<table><tr><td></td><th>Full Aperture</th><th>0.707 Aperture</th></tr><tr><th>Tag. Curvature</th><td>"));


    ss<<curvature_t(1,DWAVE);
    ss>>temp;
    ss.clear();
    result.append(tr(temp.c_str()));

    result.append(tr("</td><td>"));

    ss<<curvature_t(0.707,DWAVE);
    ss>>temp;
    ss.clear();
    result.append(tr(temp.c_str()));

    result.append(tr("</td></tr><tr><th>Sag. Curvature</th><td>"));

    ss<<curvature_s(1,DWAVE);
    ss>>temp;
    ss.clear();
    result.append(tr(temp.c_str()));

    result.append(tr("</td><td>"));

    ss<<curvature_s(0.707,DWAVE);
    ss>>temp;
    ss.clear();
    result.append(tr(temp.c_str()));

    result.append(tr("</td></tr><tr><th>Astigmatism</th><td>"));

    ss<<astigmatism(1,DWAVE);
    ss>>temp;
    ss.clear();
    result.append(tr(temp.c_str()));

    result.append(tr("</td><td>"));

    ss<<astigmatism(0.707,DWAVE);
    ss>>temp;
    ss.clear();
    result.append(tr(temp.c_str()));

    result.append(tr("</td></tr></table>"));

    result.append(tr("<h4>Distortion:</h4>"));

    result.append(tr("<table><tr><td></td><th>Full Aperture</th><th>0.707 Aperture</th></tr><tr><th>Line Distortion</th><td>"));

    ss<<distortion(1,'a',DWAVE);
    ss>>temp;
    ss.clear();
    result.append(tr(temp.c_str()));

    result.append(tr("</td><td>"));

    ss<<distortion(0.707,'a',DWAVE);
    ss>>temp;
    ss.clear();
    result.append(tr(temp.c_str()));

    result.append(tr("</td></tr><tr><th>Relative Distortion</th><td>"));

    ss<<distortion(1,'r',DWAVE);
    ss>>temp;
    ss.clear();
    result.append(tr(temp.c_str()));

    result.append(tr("</td><td>"));

    ss<<distortion(0.707,'r',DWAVE);
    ss>>temp;
    ss.clear();
    result.append(tr(temp.c_str()));

    result.append(tr("</td></tr></table>"));

    result.append(tr("</body></html>"));


    resultText->setHtml(result);
}

bool MainWindow::okToContinue(){
    if(isWindowModified()){
        int r=QMessageBox::warning(this,tr("RayTracing"),
                                   tr("The file has been modified.\n"
                                      "Do you want to save your changes?."),
                                   QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if(r==QMessageBox::Yes){
            return save();
        }else if(r==QMessageBox::Cancel){
            return false;
        }
    }

    return true;
}


MainWindow::~MainWindow()
{
    delete ui;
}
