#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QXmlStreamReader>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <algorithm>
#include <future>


int MainWindow::W = 0;
int MainWindow::H = 0;
QString MainWindow::fileName = "";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    formWithPoints = new FormWithPoints((this->width() * 0.5), (this->height() * 0.5), this);
    ui->verticalLayout->addWidget(formWithPoints);

    this->W = this->width();
    this->H = this->height();

    this->setWindowIconText("XML Loader");
    this->setWindowTitle("XML Loader");
}

MainWindow::~MainWindow()
{
    delete ui;
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
        qDebug() << "Error loading XML: " << xml.errorString();

    file.close();

    formWithPoints->setPoints(massX_, massY_);
}

bool MainWindow::loadBinaryData(QString fileName)
{    
    QFile file(fileName);

    QDataStream stream(&file);
    stream.setVersion (QDataStream::Qt_DefaultCompiledVersion);
    qint8 tempVal = 0;

    bool flag = false;
    qint16 count = 0;

    if(file.open(QIODevice::ReadOnly ))
    {
        while(!file.atEnd())
        {
            ++count;
            stream >> tempVal;            

            if(stream.status() != QDataStream::Ok)
            {
                qDebug() << "Error read data from binary file!";
                return false;
            }
            else
            {
                if(!flag)
                {
                    valuesX.push_back(tempVal);
                    flag = true;
                }
                else
                {
                    valuesY.push_back(tempVal);
                    flag = false;
                }
            }
        }
    }
    else
    {
        qDebug() << "Error open binary file!";
        return false;
    }    
    file.close();
    return true;
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    double delta = event->delta();
    int w{};
    int h{};

    w = this->width() + delta;
    h = this->height() + delta;

    this->formWithPoints->~FormWithPoints();
    formWithPoints = new FormWithPoints((w * 0.5), (h * 0.5), this);

    ui->verticalLayout->addWidget(formWithPoints);
    loadXml(fileName);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED( event );

    if(this->W == this->size().width() || this->H == this->size().height())
        return;
    else
    {
        this->formWithPoints->~FormWithPoints();
        formWithPoints = new FormWithPoints((this->width() * 0.5), (this->height() * 0.5), this);

        ui->verticalLayout->addWidget(formWithPoints);
        loadXml(fileName);

        formWithPoints->resize((this->width() * 0.5), this->height() * 0.5);
        formWithPoints->show();
    }
}

void MainWindow::on_pushButtonBinary_clicked()
{
    QString pathToFile = qApp->applicationDirPath()+ "/iq_qam32.bin";    

    auto f1 = std::async(std::launch::async, &MainWindow::loadBinaryData, this, pathToFile);

    if(f1.get())
        formWithPoints->setBinaryPoints(valuesX, valuesY);
}
