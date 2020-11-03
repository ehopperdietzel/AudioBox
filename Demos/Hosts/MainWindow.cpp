#include "MainWindow.h"
#include "../AudioBox/AudioBox.h"
#include <QDebug>
#include <QWindow>

void callBack(void *buffer,unsigned int bufferSize,AudioBox::Format format,void *userData)
{
    Q_UNUSED(format);
    Q_UNUSED(userData);

    float *theta = (float*)userData;
    float *buff = (float*)buffer;
    for(unsigned int i = 0; i < bufferSize; i++)
    {
        buff[i] = sinf(*theta);
        (*theta) += (2*M_PI*440.f)/44100.f;
    }
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{

    unsigned int size;
    DeviceBasicInfo *list;
    getOutputDevices(&list,&size);
    qDebug() << " Output Devices:";
    for(unsigned int i = 0; i < size; i++)
      qDebug() << "Device:" << list[i].name;

    AudioBox *box = loadBox("/Users/eduardo/Desktop/Cuarzo/Projects/AudioBoxRelease/libPluginDemo.1.0.0.dylib");

    if(box == 0)
    {
        qDebug() << "Could not open AudioBox\n";
        exit(1);
    }

    unsigned int *rates;
    box->getSampleRates(&rates,&size);
    qDebug() << " Avaliable Box Sample Rates:";
    for(unsigned int i = 0; i < size; i++)
      qDebug() << rates[i];

   box->setCallbackFunction(callBack,&theta);
   box->Initialize();
   box->start();

}

MainWindow::~MainWindow()
{
}
