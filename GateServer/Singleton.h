#pragma once

#include <memory>
#include <mutex>

template <typename T>
class Singleton {
public:
	static std::shared_ptr<T> GetInstance() {
		static std::once_flag flag;
		std::call_once(flag, []() {
			_instance = std::shared_ptr<T>(new T);
			});
		return _instance;
	}

protected:
	Singleton() = default;
	~Singleton() = default;
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	static std::shared_ptr<T> _instance;
};

template <typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;