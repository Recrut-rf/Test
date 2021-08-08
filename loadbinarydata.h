#ifndef LOADBINARYDATA_H
#define LOADBINARYDATA_H

#include <QObject>
#include <QThread>
#include <QVector>

class LoadBinaryData : public QObject
{
    Q_OBJECT
public:
    explicit LoadBinaryData(QObject *parent = nullptr);

public slots:
    void startWork();

signals:
    void loadBinaryFinished(QVector<int16_t>, QVector<int16_t>);

    void threadStarted();
    void threadStoped();

private:
     QString pathToFile_{};

     QVector<int16_t> binaryMassX_{};
     QVector<int16_t> binaryMassY_{};

};

#endif // LOADBINARYDATA_H
