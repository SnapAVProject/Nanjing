#include "lxprotocol.h"

qint8 gIndex = 0;

QByteArray LxProtocolGenerater::generate(LX_CMD command, QByteArray data)
{
    qint8      check = 0;
    QByteArray protocol;

    //数据区超大
    if (data.size() > 0xFF)
        return NULL;

    protocol.append('L');
    protocol.append('X');
    protocol.append(++gIndex);
    check += gIndex;
    protocol.append(command);
    check += command;
    protocol.append(data.size());
    check += data.size();

    for (int i = 0; i < data.size(); i++)
    {
        check += data[i];
        protocol.append(data[i]);
    }
    protocol.append(check);
    protocol.append('E');

    return protocol;
}


