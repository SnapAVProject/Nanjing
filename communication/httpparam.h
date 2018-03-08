#ifndef HTTPPARAM_H
#define HTTPPARAM_H

#include <QString>
#include <QVariant>
#include <QStack>
using namespace std;

typedef struct{
    int     type;
    QString prefix;
}PREFIX;

class HttpParam
{
public:
    HttpParam();
    /**********************************************************************
    * 函数名称： serialize
    * 功能描述： 链接服务器数据转换为特定格式
    * 输入参数：
    * 输出参数：
    * 修改日期        版本号     修改人	      修改内容
    * -----------------------------------------------
    * 2013/08/07	 V1.0
    ***********************************************************************/
    QString serialize(const QVariant& v);
private:
    QVector<PREFIX> prefix_;
    QString         serialize_;
};

#endif // HTTPPARAM_H
