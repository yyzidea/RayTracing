#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ui_mainwindow.h"

#include <QMainWindow>

class MainWindow : public QMainWindow,public Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void newFile();
    void openFile();
    bool save();
    bool saveAs();

    void surfaceDataChanged();
    void wavelengthDataChanged();
    void fieldDataChanged();
    void pupilDataChanged(double value);

    void addSurfaceData();
    void addFieldData();
    void addWavelengthData();
    void delSurfaceData();
    void delFieldData();
    void delWavelengthData();
    
    void calculate();

private:
    Ui::MainWindow *ui;
    bool okToContinue();
    void createActions();
    void createConnections();
    void setupMenus();
    void syncSurfaceData(int row,int col,QVariant data);
    void syncFieldData(int row,int col,QVariant data);
    void syncWavelengthData(int row,int col,QVariant data);
    void loadData();
    void loadGlass();

    bool loadOrNot=false;

    QTableWidgetSelectionRange selectRange(QTableWidget* temp);

    QString curFile;
    QStringList recentFiles;
    

    enum{MaxRecentFiles = 5};
    QAction *action_recentFile[MaxRecentFiles];
    QAction *action_Separator;

    QAction *action_Quit;

};

#endif // MAINWINDOW_H
