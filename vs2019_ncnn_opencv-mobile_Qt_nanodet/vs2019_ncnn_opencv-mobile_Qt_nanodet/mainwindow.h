#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

#include<QImage>
#include<QCamera>
#include<QCameraImageCapture>
#include<QFileDialog>

#include<opencv2/opencv.hpp>

#include<benchmark.h>

#include"matting.h"
#include"mixxing.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

public slots:
    void on_loadBackground_clicked();
    void on_openMix_clicked();

private:
    inline cv::Mat QImageToMat(const QImage& image);
    inline QImage MatToQImage(const cv::Mat& mat);

private:
    Ui::MainWindowClass ui;
    
    Matting* matting;
    Mixxing* mixxing;

    cv::Mat rgb;
    cv::Mat background;

    bool isMatting;
    bool isMixxing;

    double timeCost;
};
