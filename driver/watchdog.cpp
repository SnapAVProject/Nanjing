#include "watchdog.h"
#include "sys/ioctl.h"
#include "fcntl.h"
#include <QDebug>





#define TYPE_E_WTDIOCTL 		0xFE
#define NR_FEED_WTD        		0x21
#define FEED_WTD         			_IO(TYPE_E_WTDIOCTL, NR_FEED_WTD)

Watchdog::Watchdog(QObject *parent) :
    QThread(parent)
{

    fd_watchdog = open( "/dev/ext_watchdog", O_RDWR | O_NOCTTY | O_NDELAY);

    if (fd_watchdog < 0)
    {
            qDebug("the ext watchdog driver open failed.\n");
            return;
    }

}

void Watchdog::run()
{
    if (fd_watchdog < 0)
        return;

    while(1)
    {
        ioctl(fd_watchdog, FEED_WTD, 0);
    }
}
