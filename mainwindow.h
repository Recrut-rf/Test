#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QWheelEvent>
#include "formwithpoints.h"


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
    void on_pushButtonXML_clicked();

    void on_pushButtonBinary_clicked();

private:
    Ui::MainWindow *ui;

    FormWithPoints *formWithPoints;

    // коллекция координат X
    QVector<double> massX_;
    // коллекция координат Y
    QVector<double> massY_;

    QVector<qint8> valuesX;
    QVector<qint8> valuesY;

    // для ширины
    static int W;
    // для высоты
    static int H;

    // название файла
    static QString fileName;

    // загрузить данные из xml файла
    void loadXml(QString fileName);
    // загрузить данные из бинарного файла
    bool loadBinaryData(QString fileName);


    // QWidget interface
protected:
    // для масштабирования при помощи средней кл. мыши
    void wheelEvent(QWheelEvent *event);
    // для перерисовки
    void paintEvent(QPaintEvent *event);
};
#endif // MAINWINDOW_H
