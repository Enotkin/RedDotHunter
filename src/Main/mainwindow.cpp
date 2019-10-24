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
    plots.push_back(ui->xPlot);
    plots.push_back(ui->yPlot);
}

MainWindow::~MainWindow()
{
    delete ui;
}

std::tuple<QVector<double>, QVector<double>, QVector<double>> MainWindow::separateGraphData(const std::vector<std::vector<cv::Point> > &data)
{
    QVector<double> xData, yData, commonData;
    xData.reserve(static_cast<int>(data.size()));
    yData.reserve(static_cast<int>(data.size()));
    commonData.reserve(static_cast<int>(data.size()));
    int number = 0;
    for (const auto &points : data) {
        if (!points.empty()){
            for (const auto &point : points) {
                xData.push_back(point.x);
                yData.push_back(point.y);
                commonData.push_back(number);
            }
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
    plot->addGraph();
    setupGraph(plot->graph(0), QColor(255, 0, 0));

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

void MainWindow::setupGraph(QCPGraph * const graph, const QColor color)
{
    QPen pen;
    pen.setColor(color);
    graph->setPen(pen);
    graph->setLineStyle(QCPGraph::lsNone);
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    graph->rescaleAxes(true);
}

std::vector<QPoint> MainWindow::cvPoints2QPoints(const std::vector<cv::Point> &cvPoints)
{
    std::vector<QPoint> qPoints;
    std::transform(cvPoints.begin(), cvPoints.end(), std::back_inserter(qPoints), [](const cv::Point &point){
        return QPoint(point.x, point.y);
    });
    return qPoints;
}

PointsPacks MainWindow::findDifferences(const std::vector<Points> &bigPack, const std::vector<Points> &lowPack)
{
    std::vector<Points> packsDifference;

    unsigned size = bigPack.size();
    auto sortPred = [](const cv::Point &a, const cv::Point &b){
        return a.y < b.y;
    };

//    auto sortPredN = [](const cv::Point &a, const cv::Point &b){
//        return a.y != b.y;
//    };

    for (unsigned index = 0; index < size; index++){
        Points bigMiniPack = bigPack.at(index);

        Points lowMiniPack = lowPack.at(index);

//        QVector<int> yFirst, yLast;
//        std::transform(bigMiniPack.begin(), bigMiniPack.end(), std::back_inserter(yFirst), [](const cv::Point &a){
//            return a.y;
//        });
//        std::transform(lowMiniPack.begin(), lowMiniPack.end(), std::back_inserter(yFirst), [](const cv::Point &a){
//            return a.y;
//        });
        std::sort(bigMiniPack.begin(), bigMiniPack.end(), sortPred);
        std::sort(lowMiniPack.begin(), lowMiniPack.end(), sortPred);

        bool isPermutation = std::is_permutation(bigMiniPack.begin(), bigMiniPack.end(),
                                                 lowMiniPack.begin(), lowMiniPack.end(),
                                                 [](const cv::Point &a, const cv::Point &b){
            bool xEqual = a.x == b.x;
            bool yEqual = a.y == b.y;
            return xEqual && yEqual;
        });

        Points difference;
        if (!isPermutation) {
            std::set_difference(bigMiniPack.begin(), bigMiniPack.end(),
                                lowMiniPack.begin(), lowMiniPack.end(),
                                std::back_inserter(difference), sortPred);
//            qDebug()<< "Первый вектор" << QVector<QPoint>::fromStdVector(cvPoints2QPoints(bigMiniPack)) <<
//                       "Второй вектор" << QVector<QPoint>::fromStdVector(cvPoints2QPoints(lowMiniPack)) <<
//                       "Результирующий" << QVector<QPoint>::fromStdVector(cvPoints2QPoints(difference));
//            if(!difference.empty())
//                qDebug() << difference.front().x<< difference.front().y;

//            std::vector<int> v1 {1, 2, 5, 5, 5, 9};
//            std::vector<int> v2 {2, 5, 7};
//            std::vector<int> diff;

//            auto sortPredN = [](const int &a, const int &b){
//                return a < b;
//            };

//            std::set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(),
//                                std::inserter(diff, diff.begin()), sortPredN);
//            qDebug()<< "Первый вектор" << QVector<int>::fromStdVector(v1) <<
//                       "Второй вектор" << QVector<int>::fromStdVector(v2) <<
//                       "Результирующий" << QVector<int>::fromStdVector(diff);

        }
        packsDifference.push_back(difference);

    }
    return packsDifference;
}

int MainWindow::countNoisesFrame(const std::vector<Points> &noiseFramePack)
{
    int count = 0;
    for (const auto &pack : noiseFramePack){
        if (!pack.empty())
            count++;
    }
    return count;
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

void MainWindow::on_SelectFolderPushButton_clicked()
{
    if(imagesInfo.empty())
        return;
    analyzer.setSettings(ui->generalSettings->getSettings());
    auto resultDots = analyzer.getRedDotsCoordinate(imagesInfo);
    auto result = analyzer.timeFiltrate(resultDots);
    auto noiseDots = findDifferences(resultDots, result);

    qDebug()<<result.size();
    auto countNoiseFrame = countNoisesFrame(noiseDots);
    qDebug()<<"Количество шумных кадров:"<< countNoiseFrame <<"что составляет"<<static_cast<double>(static_cast<double>(countNoiseFrame) / static_cast<double>(result.size()) * 100) << "%";


    int countBreakFrame = 0;
    for (const auto &pack : result) {
        if(pack.size() > 2)
            countBreakFrame++;
    }
    qDebug()<<"Количество бракованых кадров:"<< countBreakFrame <<"что составляет"<<static_cast<double>(static_cast<double>(countBreakFrame) / static_cast<double>(result.size()) * 100) << "%";


    auto [xData, yData, numbers] = separateGraphData(result);
    auto [xNoise, yNoise, noiseNumbers] = separateGraphData(noiseDots);
//    qDebug() << "Шумовых паков" << static_cast<int>(noiseDots.size()) << "без пустых" << noiseNumbers.size();

    ui->xPlot->xAxis->setRange(0, result.size());
    ui->yPlot->xAxis->setRange(0, result.size());

    ui->xPlot->graph(0)->addData(numbers, xData);
    ui->yPlot->graph(0)->addData(numbers, yData);

    ui->xPlot->addGraph();
    ui->yPlot->addGraph();

    setupGraph(ui->xPlot->graph(1), QColor(0, 0, 255));
    setupGraph(ui->yPlot->graph(1), QColor(0, 0, 255));

    ui->xPlot->graph(1)->addData(noiseNumbers, xNoise);
    ui->yPlot->graph(1)->addData(noiseNumbers, yNoise);


    ui->xPlot->replot();
    ui->yPlot->replot();

    ui->horizontalSlider->setRange(1, result.size());
    ui->imageNumSpinBox->setRange(1, result.size());
}
