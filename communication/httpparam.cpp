#include "httpparam.h"

HttpParam::HttpParam()
{
}

QString HttpParam::serialize(const QVariant &v)
{
    if (!v.isValid())
        return NULL;

    switch (v.type()) {
        case QVariant::Map:
        {
            QMapIterator<QString, QVariant> iter(v.toMap());
            while (iter.hasNext())
            {
                iter.next();
                PREFIX prefix;
                prefix.type = 0;
                prefix.prefix = iter.key();
                prefix_.append(prefix);
                if (serialize(iter.value()) == NULL)
                    return NULL;
            }
            if (!prefix_.isEmpty())
                prefix_.pop_back();
            break;
        }

        case QVariant::List:
        {
            const QVariantList vList = v.toList();
            for (int i = 0; i < vList.size(); i++)
            {
                PREFIX prefix;
                prefix.type = 1;
                prefix.prefix = QString::number(i, 10);
                prefix_.append(prefix);
                if (NULL == serialize(vList[i]))
                    return NULL;
            }

            if (!prefix_.isEmpty())
                prefix_.pop_back();

            break;
        }
        case QVariant::String:
        {
            if (prefix_.isEmpty()){
                 break;
            }

            QString key = prefix_[0].prefix;
            for (int i = 1; i < prefix_.size(); ++i)
            {
                if (prefix_[i].type)
                {
                     key += "[" + prefix_[i].prefix + "]";
                }
                else{
                    key += "." + prefix_[i].prefix;
                }
            }
            if (serialize_.size())
                serialize_ += "&";
            serialize_ += key + "=" + v.toString();

            prefix_.pop_back();
            break;
        }
        default:
            return NULL;
            break;
    }

    return serialize_;
}

