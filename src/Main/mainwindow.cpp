#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->imageLabel->setScaledContents(true);
    ui->imageLabel->setPixmap(QPixmap::fromImage(QImage(":/empty/resource/empty-image.png")));
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
    analyzer.analyze(imagesInfo);
    auto result = analyzer.getResults();
    qDebug()<< result.size();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    currentImgInfo = imagesInfo.begin() + value;
    displayImage(*currentImgInfo);
}

void MainWindow::on_prevImagePushButton_clicked()
{

    displayImage(*--currentImgInfo);
    ui->horizontalSlider->setValue(ui->horizontalSlider->value()-1);
}

void MainWindow::on_nextImagePushButton_clicked()
{
    displayImage(*++currentImgInfo);
    ui->horizontalSlider->setValue(ui->horizontalSlider->value()+1);
}

void MainWindow::on_imageNumSpinBox_valueChanged(int arg1)
{
    currentImgInfo = imagesInfo.begin() + arg1;
    displayImage(*currentImgInfo);
}

void MainWindow::displayImage(const QFileInfo imgInfo)
{
    const QImage image(imgInfo.absoluteFilePath());
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
    ui->binLabel->setPixmap(QPixmap::fromImage(binImage(image)));
    ui->colorBinImage->setPixmap(QPixmap::fromImage(colorBinImage(image)));
}

QImage MainWindow::binImage(const QImage &image)
{
    Binarizator bin;
    auto result = bin.getImage(ASM::QImageToCvMat(image));
    return ASM::cvMatToQImage(result);
}

QImage MainWindow::colorBinImage(const QImage &image)
{
    Binarizator bin;
    BinarizatorSettings settings;
    settings.mode = BinarizatorSettings::FilterType::Colorfull;
    settings.color = QColor::fromHsl(30, 90, 100);
    BinarizatorSettings::CoefficientHsv coefHSV;
    coefHSV.hue = {50, 50};
    coefHSV.value = {50, 50};
    coefHSV.saturation = {50, 50};
    settings.coefficientHsv = coefHSV;
    bin.setSettings(settings);
    auto result = bin.getImage(ASM::QImageToCvMat(image));
    return ASM::cvMatToQImage(result);
}

void MainWindow::on_openFolderAction_triggered()
{
    QDir dir(QFileDialog::getExistingDirectory(this, tr("Выбор папки с файлами"), "D:/"));
    imagesInfo = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
    currentImgInfo = imagesInfo.begin();
    displayImage(*currentImgInfo);
    ui->horizontalSlider->setRange(1, imagesInfo.size());
    ui->imageNumSpinBox->setRange(1, imagesInfo.size());
}
