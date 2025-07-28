#ifndef SINGLETON_H
#define SINGLETON_H

#include "global.h"

template <typename T>
class Singleton {
protected:
    Singleton() = default;
    Singleton(const Singleton<T>&) = delete;
    Singleton& operator=(const Singleton<T>& st) = delete;

    static std::shared_ptr<T> _instance;
public:
    static std::shared_ptr<T> GetInstance() {
        static std::once_flag s_flag;
        std::call_once(s_flag, [&]() {
            // 注意：HttpMgr 是 QObject 子类，不能用 make_shared 创建。
            // 必须使用 shared_ptr<T>(new T)，否则 shared_from_this() 会非法。
            _instance = std::shared_ptr<T>(new T);
        });

        return _instance;
    }
    void PrintAddress() {
        std::cout << _instance.get() << std::endl;
    }
    ~Singleton() {
        std::cout << "this is singleton destruct" << std::endl;
    }
};

template <typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;

#endif // SINGLETON_H
