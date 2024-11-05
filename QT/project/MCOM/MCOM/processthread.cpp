#include "processthread.h"
#include "QDebug"
ProcessThread::ProcessThread(QObject *parent)  
    : QThread(parent), m_running(true)  
{  
}  

ProcessThread::~ProcessThread()  
{  
    m_mutex.lock();  
    m_running = false;  
    m_mutex.unlock();  
    wait();
}  

void ProcessThread::run()  
{
    m_mutex.lock();
    ProcessData();
    m_mutex.unlock();
}

void ProcessThread::stop()  
{  
    m_mutex.lock();  
    m_running = false;  
    m_mutex.unlock();  
}

void ProcessThread::ProcessData()  
{
    // qDebug()<<"ProcessData";
}
