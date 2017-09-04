#ifndef CAMERA_H
#define CAMERA_H
/*
    well,there are two policies,one is emit buffer when VideoSrc avilable, one is timer emit fetching from VideoSrc per xx msecond.

*/
#include <QTimer>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <QObject>
#include <QMutex>
#include <QThread>
#include "config.h"
#include "videosrc.h"
#include "videohandler.h"
using namespace cv;
using namespace std;
//class CameraManager;
/*
    get mat from src  every 10 msec , give mat to video handler

*/

class Camera : public QObject
{
    Q_OBJECT
public:
    explicit Camera(camera_data_t dat,QObject *parent=0) : data(dat),QObject(parent)
    {
        tick=0;
        tick_work=0;
        p_video_src=new VideoSrc(data.ip);
        timer=new QTimer();
        connect(timer,SIGNAL(timeout()),this,SLOT(work()));
        fetch_thread.start();
   //     timer->start(30);
    }
    ~Camera(){
        delete timer;
        delete p_video_src;
    }
    void restart(camera_data_t dat)
    {
        data=dat;
    }
    void fetch()
    {
         IplImage *f=p_video_src->fetch_frame();
    }

signals:

public slots:
    void work()
    {
        //  p_video_src->work(video_handler);
        IplImage *f=p_video_src->fetch_frame();
        if(tick_work++%1==0){
            video_handler.set_frame(f);
            video_handler.work("test url");
        }
        tick++;
    }
private:
    camera_data_t data;//data that camera need
    QTimer *timer;//do work per xx micro seconds
    VideoSrc*p_video_src;//camera frame source
    VideoHandler video_handler;//camera frame handler
    int tick;
    int tick_work;
    QList <IplImage> frame_list;
    QMutex lock;
    QThread fetch_thread;
};


class CameraManager:public QObject{
    Q_OBJECT
public:
    CameraManager(){
         p_cfg=new Config(":/config.json");
       //     p_cfg=new Config();
        for(int i=0;i<p_cfg->data.camera_amount;i++){
            Camera *c=new Camera(p_cfg->data.camera[i]);
            cams.append(c);
        }
    }
    ~CameraManager(){
        for(int i=0;i<p_cfg->data.camera_amount;i++){
            delete cams[i];
        }
    }

    void reload_camera()
    {
        foreach (Camera *c, cams) {
            delete c;
        }
        int num;
        cams.clear();
        for(int i=0;i<p_cfg->data.camera_amount;i++){
             Camera *c=new Camera(p_cfg->data.camera[i]);
             cams.append(c);
            //  if(i==0)
          //    connect(c->p_src,SIGNAL(frame_update(Mat)),&c->render,SLOT(set_mat(Mat)));
          //   if(i==0)
          //   layout->addWidget(&c->render,i,i);
        }
        num=cams.size();
    }
public slots:
    void add_camera(QByteArray buf)
    {
        p_cfg->set_ba((buf));
        Camera *c=new Camera(p_cfg->data.camera[p_cfg->data.camera_amount-1]);
        cams.append(c);
    }
    void add_camera(QString ip)
    {
        //         Camera *c=new Camera(cfg.data.camera[i]);

        camera_data_t ca;
        ca.ip=ip;
        ca.port=554;
        p_cfg->data.camera.append(ca);
        p_cfg->data.camera_amount++;
        Camera *c=new Camera(p_cfg->data.camera[p_cfg->data.camera_amount-1]);
        cams.append(c);
        p_cfg->save();
       //  if(i==0)
     //    connect(c->p_src,SIGNAL(frame_update(Mat)),&c->render,SLOT(set_mat(Mat)));
     //   if(i==0)
    //    layout->addWidget(&c->render,1,cams.length()-1);
    }
    void del_camera(int index)
    {
        if(index<=p_cfg->data.camera_amount&&index>0){
            p_cfg->data.camera.removeAt(index-1);
            p_cfg->data.camera_amount--;
            p_cfg->save_config_to_file();
            delete cams[index-1];
            cams.removeAt(index-1);
        }
    }


    void modify_camera(int index)
    {
        cams[index]->restart(p_cfg->data.camera[index-1]);
    }
    int get_config(char *c)
    {
        QByteArray b(p_cfg->get_ba());
        int len=b.length();
        memcpy(c,b.data(),len);
        return len;
    }
    QString get_config()
    {
        QByteArray b(p_cfg->get_ba());
        int len=b.length();
     //   memcpy(c,b.data(),len);
       // return len;
         QString str(b);
         return str;
    }
    QByteArray get_config(int i)
    {
        QByteArray b(p_cfg->get_ba());

         return b;
    }
    int get_size()
    {
        return cams.size();
    }

private:
    QList <Camera *> cams;//cameras that opened, all cameras is working,or trying to work
    Config *p_cfg;//all the setting on this server
};


#endif // CAMERA_H
