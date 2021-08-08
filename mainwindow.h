#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QWheelEvent>
#include <QThread>
#include "formwithpoints.h"
#include "loadbinarydata.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void startWork();


private slots:
    void on_pushButtonXML_clicked();
    void on_pushButtonBinary_clicked();

    void loadBinaryFinished(QVector<int16_t> valuesX, QVector<int16_t> valuesY);

    void threadStarted();
    void threadStoped();

    void on_closeButton_clicked();

private:
    Ui::MainWindow *ui;

    FormWithPoints *formWithPoints_ = nullptr;

    // коллекция координат X
    QVector<double> massX_;
    // коллекция координат Y
    QVector<double> massY_;

    LoadBinaryData *loadBinaryData_;

    QThread formThread_;

    // для ширины
    static int W;
    // для высоты
    static int H;

    // название файла
    static QString fileName;
    QString pathToFile_{};

    // загрузить данные из xml файла
    void loadXml(QString fileName);

    // QWidget interface
protected:
    // для масштабирования при помощи средней кл. мыши
    void wheelEvent(QWheelEvent *event);
    // для перерисовки
    void paintEvent(QPaintEvent *event);
};
#endif // MAINWINDOW_H

