#ifndef FORMWITHPOINTS_H
#define FORMWITHPOINTS_H

#include <QWidget>
#include <QVector>
#include <QWheelEvent>
#include <QVBoxLayout>

namespace Ui {
class FormWithPoints;
}

class FormWithPoints : public QWidget
{
    Q_OBJECT

public:
    explicit FormWithPoints(int width, int height, QWidget *parent = nullptr);
    ~FormWithPoints();

    void setPoints(QVector<double> massX, QVector<double> massY);
    void setBinaryPoints(QVector<qint8> massX, QVector<qint8> massY);

private:
    Ui::FormWithPoints *ui;

    int width_;  // ширина
    int height_; // высота

    // данные по X из XML
    QVector<double> massX_;
    // данные по Y из XML
    QVector<double> massY_;
    // данные по X из бинарного файла
    QVector<qint8> binaryMassX_;
    // данные по Y из бинарного файла
    QVector<qint8> binaryMassY_;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // FORMWITHPOINTS_H
