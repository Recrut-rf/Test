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

public slots:
    void setBinaryPoints(QVector<int16_t> binaryMassX, QVector<int16_t> binaryMassY);

private:
    Ui::FormWithPoints *ui;

    int width_;  // ширина
    int height_; // высота

    // данные по X из XML
    QVector<double> massX_;
    // данные по Y из XML
    QVector<double> massY_;
    // данные по X из бинарного файла
    QVector<int16_t> binaryMassX_;
    // данные по Y из бинарного файла
    QVector<int16_t> binaryMassY_;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // FORMWITHPOINTS_H

