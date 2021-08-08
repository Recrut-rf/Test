#include "loadbinarydata.h"
#include <QFile>
#include <QFileDialog>
#include <QApplication>
#include <QDebug>

LoadBinaryData::LoadBinaryData(QObject *parent) : QObject(parent)
{
    pathToFile_ = qApp->applicationDirPath()+ "/iq_qam32.bin";
}

void LoadBinaryData::startWork()
{
    emit threadStarted();

    binaryMassX_.clear();
    binaryMassY_.clear();

    QFile file(pathToFile_);

    QDataStream stream(&file);
    stream.setVersion (QDataStream::Qt_DefaultCompiledVersion);
    int16_t tempVal{};

    bool flag = false;
    int16_t count{};  // счётчик прочитанных координат точек

    stream.setByteOrder(QDataStream::LittleEndian);

    if(file.open(QIODevice::ReadOnly))
    {
        while(true)
        {
            if(file.isOpen())
            {
                if(count > 0 && (count % 2000 == 0))
                {
                    emit loadBinaryFinished(binaryMassX_, binaryMassY_);
                    QThread::msleep(1000);
                }

                if(count == 16000)
                {
                    count = 0;
                    file.close();
                    emit loadBinaryFinished(binaryMassX_, binaryMassY_);
                    emit threadStoped();
                    break;
                }

                stream >> tempVal;

                if(stream.status() != QDataStream::Ok)
                {
                    file.close();
                    void threadStoped();
                    qDebug() << "Error read data from binary file!";
                    break;
                }
                else
                {
                    if(!flag)
                    {
                        binaryMassX_.push_back(tempVal);
                        flag = true;
                    }
                    else
                    {
                        binaryMassY_.push_back(tempVal);
                        flag = false;
                    }
                }
                 ++count;
            }

            else
            {
                qDebug() << "Error open binary file!";
                void threadStoped();
                break;
            }
        }
    }
}
