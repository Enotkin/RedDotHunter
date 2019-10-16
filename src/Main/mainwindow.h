#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTime>

#include <memory>
#include "asmOpenCV.h"
#include "qcustomplot.h"
#include "Analyzer/Binarizator/colorbinarizator.h"
#include "Analyzer/Binarizator/normalbinarizator.h"
#include "Analyzer/redanalyzer.h"
#include "Analyzer/Binarizator/binarizatorsettingsform.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_SelectFolderPushButton_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_prevImagePushButton_clicked();

    void on_nextImagePushButton_clicked();

    void on_imageNumSpinBox_valueChanged(int arg1);

    void on_openFolderAction_triggered();

private:
    Ui::MainWindow *ui;
    QFileInfoList imagesInfo;
    QList<QFileInfo>::iterator currentImgInfo = nullptr;
    RedAnalyzer analyzer;

    bool iteratorCheck(const QList<QFileInfo>::iterator &it);
    void displayImage(const QList<QFileInfo>::iterator &it);

    void setupPlot(QCustomPlot * const plot);

    std::tuple<QVector<double>, QVector<double>, QVector<double>> separateGraphData(const std::vector<std::vector<cv::Point>> &data);

};
#endif // MAINWINDOW_H
