#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <QThread>

class Watchdog : public QThread
{
    Q_OBJECT
public:
    explicit Watchdog(QObject *parent = 0);
    
private:
   void run();

private:
   int fd_watchdog;
    
};

#endif // WATCHDOG_H
