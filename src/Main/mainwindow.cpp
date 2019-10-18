#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->imageLabel->setScaledContents(true);
    ui->imageLabel->setPixmap(QPixmap::fromImage(QImage(":/empty/resource/empty-image.png")));
    ui->settingsWidget->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_SelectFolderPushButton_clicked()
{
    if(imagesInfo.empty())
        return;
    analyzer.setSettings(ui->generalSettings->getSettings());
    auto result = analyzer.getRedDotsCoordinate(imagesInfo);
    TrackDetector detector;
    auto tracks = detector.separatePoints(result);

    auto [xData, yData, numbers] = separateGraphData(result);
    ui->xPlot->xAxis->setRange(0, result.size());
    ui->yPlot->xAxis->setRange(0, result.size());

    ui->xPlot->graph(0)->addData(numbers, xData);

    ui->yPlot->graph(0)->addData(numbers, yData);

    ui->xPlot->replot();
    ui->yPlot->replot();

    ui->horizontalSlider->setRange(1, result.size());
    ui->imageNumSpinBox->setRange(1, result.size());
}

std::tuple<QVector<double>, QVector<double>, QVector<double>> MainWindow::separateGraphData(const std::vector<std::vector<cv::Point> > &data)
{
    QVector<double> xData, yData, commonData;
    xData.reserve(static_cast<int>(data.size()));
    yData.reserve(static_cast<int>(data.size()));
    commonData.reserve(static_cast<int>(data.size()));
    int number = 0;
    for (const auto &points : data) {
        for (const auto &point : points) {
            xData.push_back(point.x);
            yData.push_back(point.y);
            commonData.push_back(number);
        }
        number++;
    }

    return {xData, yData, commonData};

}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    currentImgInfo = imagesInfo.begin() + value;
    displayImage(currentImgInfo);
}

void MainWindow::on_prevImagePushButton_clicked()
{

    displayImage(--currentImgInfo);
    ui->horizontalSlider->setValue(ui->horizontalSlider->value()-1);
}

void MainWindow::on_nextImagePushButton_clicked()
{
    displayImage(++currentImgInfo);
    ui->horizontalSlider->setValue(ui->horizontalSlider->value()+1);
}

void MainWindow::on_imageNumSpinBox_valueChanged(int arg1)
{
    currentImgInfo = imagesInfo.begin() + arg1;
    displayImage(currentImgInfo);
}

void MainWindow::displayImage(const QList<QFileInfo>::iterator &it)
{
    auto imgInfo = *it;
    const QImage image(imgInfo.absoluteFilePath());
    auto matImage = ASM::QImageToCvMat(image);
    auto points = analyzer.getPoints(matImage);
    for (const auto &point : points) {
        cv::drawMarker(matImage, point, cv::Scalar(0, 255, 0), cv::MARKER_TILTED_CROSS);
    }
    ui->imageLabel->setPixmap(ASM::cvMatToQPixmap(matImage));


//    double curretnPosition = -(imagesInfo.begin() - currentImgInfo);
//    for(const auto &point : points){
//        ui->xPlot->graph(0)->addData(curretnPosition, point.x);
//        ui->xPlot->graph(0)->addData(curretnPosition, point.y);
////        qDebug()<<-(imagesInfo.begin() - currentImgInfo)<< point.x << point.y;
//    }
//    ui->xPlot->replot();
//    ui->yPlot->replot();


//    ui->binLabel->setPixmap(QPixmap::fromImage(binImage(image)));
//    ui->colorBinImage->setPixmap(QPixmap::fromImage(colorBinImage(image)));
}


void MainWindow::setupPlot(QCustomPlot * const plot)
{
    QPen pen;

    plot->addGraph();
    pen.setColor(QColor(255, 0, 0));
    plot->graph(0)->setPen(pen);
    plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));

    plot->graph(0)->rescaleAxes(true);

    // zoom out a bit:
    plot->yAxis->scaleRange(1.1, plot->yAxis->range().center());
    plot->xAxis->scaleRange(1.1, plot->xAxis->range().center());
    plot->yAxis->setRange(600, 650);
    plot->xAxis->setRange(0, 100);
    // set blank axis lines:
//    plot->xAxis->setTicks(false);
    plot->yAxis->setTicks(true);
//    plot->xAxis->setTickLabels(false);
    plot->yAxis->setTickLabels(true);
    // make top right axes clones of bottom left axes:
    plot->axisRect()->setupFullAxesBox();

}



void MainWindow::on_openFolderAction_triggered()
{
    QDir dir(QFileDialog::getExistingDirectory(this, tr("Выбор папки с файлами"), "D:/"));
    imagesInfo = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
    if(imagesInfo.empty()){
        return;
    }
    currentImgInfo = imagesInfo.begin();
//    displayImage(currentImgInfo);
//    ui->horizontalSlider->setRange(1, imagesInfo.size());
//    ui->imageNumSpinBox->setRange(1, imagesInfo.size());

    setupPlot(ui->xPlot);
    setupPlot(ui->yPlot);
    ui->xPlot->yAxis->setRange(0, 1200);
    ui->yPlot->yAxis->setRange(0, 400);

    ui->xPlot->replot();
    ui->yPlot->replot();

}
