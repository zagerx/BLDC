
#ifndef PROCESSTHREAD__H
#define PROCESSTHREAD__H
#include <QThread>  
#include <QMutex>  
  
class ProcessThread : public QThread  
{  
    Q_OBJECT  
  
public:  
    explicit ProcessThread(QObject *parent = nullptr);
    ~ProcessThread();
    void run() override;
    void stop();
    void ProcessData();
signals:  
    void newDataReceived();
private:  
    volatile bool m_running;  
    QMutex m_mutex;  
};

#endif
