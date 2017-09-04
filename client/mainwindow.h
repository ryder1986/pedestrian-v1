#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"
#include "protocol.h"
#include "config.h"
//#include "camera.h"
//#include "client.h"
#include "yuvrender.h"
#include "camera.h"
#include "videosrc.h"

class ClientCameraManager:public CameraManager{

public:
    ClientCameraManager()
    {
       //     p_cfg=new Config(":/config.json");
    }
    ~ClientCameraManager()
    {

    }

    void reconfig_camera(QGridLayout *g)
    {
        reload_camera();
//        foreach (Camera *c, cams) {
//            delete c;
//        }
//        int num;
//        cams.clear();
//        for(int i=0;i<cfg->data.camera_amount;i++){
//             Camera *c=new Camera(cfg->data.camera[i]);
//             cams.append(c);
//            //  if(i==0)
//          //    connect(c->p_src,SIGNAL(frame_update(Mat)),&c->render,SLOT(set_mat(Mat)));
//          //   if(i==0)
//          //   layout->addWidget(&c->render,i,i);
//        }
//        num=cams.size();
    }
    void save_config(QByteArray ba)
    {

    }
//    QByteArray get_config()
//    {
//      //  cfg.get_ba();
//      return get_config();
//    }
//    void add_camera(QString url)
//    {

//    }
//    void del_camera(int index)
//    {

//    }
//    int get_size()
//    {
//        return cams.size();
//    }

private:
//    Config *p_cfg;
//    QList <Camera *> cams;

};

namespace Ui {
class MainWindow;
}
/*
   client:manage network connection with server. provide cmd like add,del,modify camera through server
   camera manager: manage cfg file,manage camera widget layout and rendering
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit  MainWindow(QWidget *parent=0) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
    {
        ui->setupUi(this);
        ui->groupBox->setFixedWidth(300);
        ui->centralWidget->setLayout(ui->horizontalLayout);
        ui->groupBox->setLayout(ui->gridLayout);
        ui->widget->setLayout(ui->gridLayout_2);

        cam_manager=new ClientCameraManager();
        client=new Client();
        cam_manager->reconfig_camera(ui->gridLayout_2);
        //    clt=new client();
        //    //CameraManager *cam=new CameraManager();
        //    render=new YuvRender();
        //    clt->get_client_setting();
        //    Config *cfg=new Config;
        //    Camera *cam=new Camera(cfg->data.camera[0]);
        //    connect(&cam->src,SIGNAL(frame_update(Mat)),render,SLOT(set_mat(Mat) ));
#if 0
        QPushButton *b1=new QPushButton("1");
        QPushButton *b2=new QPushButton("2");
        QPushButton *b3=new QPushButton("3");
        ui->gridLayout_2->addWidget(b1,1,1);
        ui->gridLayout_2->addWidget(b2,2,1);
        ui->gridLayout_2->addWidget(b3,2,2);
#endif
    }

    ~MainWindow()
    {
        delete cam_manager;
        delete ui;
    }


private slots:
    //    void on_pushButton_2_clicked(){

    //    }

    //    void on_pushButton_4_clicked(){

    //    }

    //    void on_pushButton_clicked(){

    //    }

    void test(int t)
    {
        qDebug()<<"get pic "<<t;
    }


    void on_pushButton_search_device_clicked();

    void on_pushButton_connect_server_clicked();



    void on_pushButton_get_config_clicked();


    void on_pushButton_add_clicked();


    void on_pushButton_del_clicked();

private:
    //    client *  clt;
    Ui::MainWindow *ui;
    ClientCameraManager *cam_manager;
    Client *client;
    char buf[BUF_MAX_LEN];
    //   YuvRender *render;
    //  QList <YuvRender *> renders;
};

#endif // MAINWINDOW_H



