#include "mainwindow.h"

#include<QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setWindowTitle("Meeting-Matting");

    isMatting = false;
    isMixxing = false;
    
    timeCost = ncnn::get_current_time();

    QCamera* cam = new QCamera;
    cam->setCaptureMode(QCamera::CaptureStillImage);
    QCameraImageCapture* cap = new QCameraImageCapture(cam);
    cap->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);

    matting = new Matting();
    mixxing = new Mixxing();

    QObject::connect(cap, &QCameraImageCapture::imageCaptured, [=](int id, QImage img) {
        rgb = QImageToMat(img);
        cv::flip(rgb, rgb, 1);
        ui.showInput->setPixmap(QPixmap::fromImage(MatToQImage(rgb)).scaled(ui.showInput->width(), ui.showInput->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

        if (isMixxing) {
            rgb = mixxing->once(rgb);
        }
        if (isMatting) {
            cv::Mat mask = matting->once(rgb);
            for (int y = 0; y < rgb.rows; y++) {
                for (int x = 0; x < rgb.cols; x++) {
                    float scale = mask.at<float>(y, x);
                    rgb.at<cv::Vec3b>(y, x)[0] = scale * rgb.at<cv::Vec3b>(y, x)[0] + (1.f - scale) * background.at<cv::Vec3b>(y, x)[0];
                    rgb.at<cv::Vec3b>(y, x)[1] = scale * rgb.at<cv::Vec3b>(y, x)[1] + (1.f - scale) * background.at<cv::Vec3b>(y, x)[1];
                    rgb.at<cv::Vec3b>(y, x)[2] = scale * rgb.at<cv::Vec3b>(y, x)[2] + (1.f - scale) * background.at<cv::Vec3b>(y, x)[2];
                }
            }
        }

        QImage image = MatToQImage(rgb);
        QPixmap pixmap = QPixmap::fromImage(image);
        ui.showOutput->setPixmap(pixmap);

        double time = ncnn::get_current_time();
        setWindowTitle(QString("Meeting-Matting %1fps").arg(int(1000 / (time - timeCost))));
        timeCost = time;
        });

    QObject::connect(cap, &QCameraImageCapture::readyForCaptureChanged, [=](bool state) {
        if (state == true) {
            cam->searchAndLock();
            cap->capture();
            cam->unlock();
        }
        });

    cam->start();

    QCameraViewfinderSettings set;
    set.setResolution(1280, 720);
    cam->setViewfinderSettings(set);
}

void MainWindow::on_loadBackground_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open image file"), "./", tr("Image files(*.bmp *.jpg *.png *.jpeg);;All files (*.*)"));
    if (!fileName.isEmpty()) {
        std::string path = fileName.toLocal8Bit().toStdString();
        background = cv::imread(path);
        cv::resize(background, background, cv::Size(1280, 720));
        cv::cvtColor(background, background, cv::COLOR_BGR2RGB);
        QPixmap pixmap;
        pixmap = pixmap.fromImage(MatToQImage(background));
        pixmap = pixmap.scaled(ui.showBackground->width(), ui.showBackground->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui.showBackground->setPixmap(pixmap);
        isMatting = true;
    }
}

void MainWindow::on_openMix_clicked()
{
    if (isMixxing == true) {
        isMixxing = false;
    }
    else if (!background.empty() && isMixxing==false) {
        mixxing->calT(rgb, background);
        isMixxing = true;
    }
}

inline cv::Mat MainWindow::QImageToMat(const QImage& image)
{
    QImage   swapped = image;
    if (image.format() == QImage::Format_RGB32) {
        swapped = swapped.convertToFormat(QImage::Format_RGB888);
    }

    return cv::Mat(swapped.height(), swapped.width(),
        CV_8UC3,
        const_cast<uchar*>(swapped.bits()),
        static_cast<size_t>(swapped.bytesPerLine())
    ).clone();
}

inline QImage MainWindow::MatToQImage(const cv::Mat& mat)
{
    QImage image(mat.data,
        mat.cols, mat.rows,
        static_cast<int>(mat.step),
        QImage::Format_RGB888);

    return image;
}