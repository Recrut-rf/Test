#include "formwithpoints.h"
#include "ui_formwithpoints.h"
#include <QPointF>
#include <QImage>
#include <QPainter>
#include <algorithm>


FormWithPoints::FormWithPoints(int width, int height, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormWithPoints), width_(width), height_(height)
{
    ui->setupUi(this);

    this->setFixedSize(width_, height_);

    width_ = this->width() * 0.9;
    height_ = this->height() * 0.9;
}

FormWithPoints::~FormWithPoints()
{
    delete ui;
}

void FormWithPoints::setPoints(QVector<double> massX, QVector<double> massY)
{
    massX_ = massX;
    massY_ = massY;
    this->repaint();
}

void FormWithPoints::setBinaryPoints(QVector<int16_t> binaryMassX, QVector<int16_t> binaryMassY)
{
    this->binaryMassX_ = binaryMassX;
    this->binaryMassY_ = binaryMassY;
    this->repaint();
}

void FormWithPoints::paintEvent(QPaintEvent *event)
{
    Q_UNUSED( event );

    QImage image(":/cross.png");

    double hx, hy, max_x, min_x, max_y, min_y;

    int hxInt, hyInt, max_xInt, min_xInt, max_yInt, min_yInt;

    // поиск максимума по X
    max_x = *(std::max_element(massX_.begin(), massX_.end()));
    // поиск минимума по X
    min_x = *(std::min_element(massX_.begin(), massX_.end()));

    // тоже самое с Y
    max_y = *(std::max_element(massY_.begin(), massY_.end()));
    min_y = *(std::min_element(massY_.begin(), massY_.end()));

    hx = max_x - min_x;
    hy = max_y - min_y;

    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 3) );

    for (int i = 0; i < massX_.size(); ++i)
    {
        // перевод абсолютных значений в относительные
       qreal X = (massX_[i] - min_x) * width_ / hx + 0.5;
       qreal Y = (massY_[i] - min_y) * height_ / hy + 0.5;
       // рисуем крестик в нужную позицию
       painter.drawImage(QPointF(X, Y),image);
    }
    if(massX_.size() == 0)
    {
        painter.setPen(QPen(Qt::white, 1) );
        painter.drawPoint(QPointF(0, 0));
    }

    painter.setPen(QPen(Qt::red, 2) );

    // поиск максимума по X
    max_xInt = *(std::max_element(binaryMassX_.begin(), binaryMassX_.end()));
    // поиск минимума по X
    min_xInt = *(std::min_element(binaryMassX_.begin(), binaryMassX_.end()));

    // тоже самое с Y
    max_yInt = *(std::max_element(binaryMassY_.begin(), binaryMassY_.end()));
    min_yInt = *(std::min_element(binaryMassY_.begin(), binaryMassY_.end()));

    hxInt = max_xInt - min_xInt;
    hyInt = max_yInt - min_yInt;

    for (int i = 0; i < binaryMassX_.size() /*/ 16000*/; ++i)
    {
        // перевод абсолютных значений в относительные
       qreal X = (binaryMassX_[i] - min_xInt) * width_ / hxInt + 0.5;
       qreal Y = (binaryMassY_[i] - min_yInt) * height_ / hyInt + 0.5;
       // рисуем крестик в нужную позицию
       painter.drawPoint(QPoint(X, Y));
    }
    if(binaryMassX_.size() == 0)
    {
        painter.setPen(QPen(Qt::white, 1) );
        painter.drawPoint(QPoint(0, 0));
    }

}

