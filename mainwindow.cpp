#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QXmlStreamReader>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <algorithm>


int MainWindow::W = 0;
int MainWindow::H = 0;
QString MainWindow::fileName = "";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    formWithPoints_ = new FormWithPoints((this->width() * 0.5), (this->height() * 0.5), this);
    ui->verticalLayout->addWidget(formWithPoints_);

    this->W = this->width();
    this->H = this->height();

    this->setWindowIconText("XML Loader");
    this->setWindowTitle("XML Loader");

    loadBinaryData_ = nullptr;

    pathToFile_ = qApp->applicationDirPath()+ "/iq_qam32.bin";
}

MainWindow::~MainWindow()
{
    delete ui;

    if(loadBinaryData_)
        delete loadBinaryData_;
}

void MainWindow::on_pushButtonXML_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, "Open Xml", ".", "Xml files (*.xml)");
    loadXml(fileName);
}

void MainWindow::loadXml(QString fileName)
{
    massX_.clear();
    massY_.clear();

    QXmlStreamReader xml;
    QFile file(fileName);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error loading XML file.";
        return;
    }

    xml.setDevice(&file);

    while(!xml.atEnd())
    {
        if(xml.isStartElement())
        {
            QString name = xml.name().toString();

            if(name == "Point")
            {
                for(int i = 0; i < xml.attributes().size(); ++i)
                {
                   if(xml.attributes().at(i).name() == "X")
                       massX_.push_back(xml.attributes().at(i).value().toDouble());

                   if(xml.attributes().at(i).name() == "Y")
                       massY_.push_back(xml.attributes().at(i).value().toDouble());
                }
            }
        }
        xml.readNext();
    }

    if(xml.hasError())
    {
        qDebug() << "Error loading XML: " << xml.errorString();
        file.close();
        return;
    }
    file.close();
    formWithPoints_->setPoints(massX_, massY_);
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    double delta = event->delta();
    int w{};
    int h{};

    w = this->width() + delta;
    h = this->height() + delta;

    this->formWithPoints_->~FormWithPoints();
    formWithPoints_ = new FormWithPoints((w * 0.5), (h * 0.5), this);

    ui->verticalLayout->addWidget(formWithPoints_);
    loadXml(fileName);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED( event );

    if(this->W == this->size().width() || this->H == this->size().height())
        return;
    else
    {
        this->formWithPoints_->~FormWithPoints();
        formWithPoints_ = new FormWithPoints((this->width() * 0.5), (this->height() * 0.5), this);

        ui->verticalLayout->addWidget(formWithPoints_);
        loadXml(fileName);

        formWithPoints_->resize((this->width() * 0.5), this->height() * 0.5);
        formWithPoints_->show();
    }
}

void MainWindow::on_pushButtonBinary_clicked()
{
    if(loadBinaryData_ == nullptr)
    {
        loadBinaryData_ = new LoadBinaryData();
        this->moveToThread(&formThread_);
        connect(&formThread_, &QThread::finished, formWithPoints_, &QObject::deleteLater);
        connect(this, &MainWindow::startWork, loadBinaryData_, &LoadBinaryData::startWork);
        connect(loadBinaryData_, &LoadBinaryData::threadStarted, this, &MainWindow::threadStarted, Qt::DirectConnection);
        connect(loadBinaryData_, &LoadBinaryData::threadStoped, this, &MainWindow::threadStoped, Qt::DirectConnection);
        connect(loadBinaryData_, &LoadBinaryData::loadBinaryFinished, this, &MainWindow::loadBinaryFinished, Qt::DirectConnection);
        formThread_.start();
        emit startWork();
    }
    else
    {
        connect(&formThread_, &QThread::finished, formWithPoints_, &QObject::deleteLater);
        connect(this, &MainWindow::startWork, loadBinaryData_, &LoadBinaryData::startWork);
        connect(loadBinaryData_, &LoadBinaryData::threadStarted, this, &MainWindow::threadStarted, Qt::DirectConnection);
        connect(loadBinaryData_, &LoadBinaryData::threadStoped, this, &MainWindow::threadStoped, Qt::DirectConnection);
        connect(loadBinaryData_, &LoadBinaryData::loadBinaryFinished, this, &MainWindow::loadBinaryFinished, Qt::DirectConnection);
        emit startWork();
    }
}

void MainWindow::loadBinaryFinished(QVector<int16_t> valuesX, QVector<int16_t> valuesY)
{
    formWithPoints_->setBinaryPoints(valuesX, valuesY);
}

void MainWindow::threadStarted()
{
    qDebug() << "Thread started!";
}

void MainWindow::threadStoped()
{
    formWithPoints_->setBinaryPoints(QVector<int16_t>(), QVector<int16_t>());
    qDebug() << "Thread stoped!";

    // если нужна постоянна работа, то расскоментировать строку ниже
    // правда при этом возможны "зависания проги"
    //emit startWork();
}

void MainWindow::on_closeButton_clicked()
{
    if(formThread_.isRunning())
        formThread_.destroyed();

    this->close();
}
