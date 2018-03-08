#ifndef SINGLETON_H
#define SINGLETON_H

#include <memory>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>

using namespace std;

template <class T>
class Singleton : public QObject
{

public:
    static inline T* instance();

private:

    Singleton(void){}

    ~Singleton(void){}

    Singleton(const Singleton&){}

    Singleton & operator= (const Singleton &){}

    static auto_ptr<T> _instance;

};

template <class T>
auto_ptr<T> Singleton<T>::_instance;



template <class T>


inline T* Singleton<T>::instance()
{
    static QMutex mutex;
    if( 0== _instance.get())
    {
        QMutexLocker locker(&mutex);
        _instance.reset ( new T);
    }
    return _instance.get();
}

#define DECLARE_SINGLETON_CLASS( type ) \
       friend class auto_ptr< type >;\
       friend class Singleton< type >;



#endif // SINGLETON_H
