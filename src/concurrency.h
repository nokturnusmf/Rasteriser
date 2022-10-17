#pragma once

#include <atomic>

class SimpleMutex {
public:
    bool Lock();
    void Unlock();

private:
    std::atomic_flag flag;
};

class SimpleLock {
public:
    SimpleLock(SimpleMutex& mutex);
    ~SimpleLock();

    SimpleLock(SimpleLock&& other);

    operator bool() const;

private:
    SimpleMutex* mutex;
};

inline bool SimpleMutex::Lock() {
    return !this->flag.test_and_set(std::memory_order_acquire);
}

inline void SimpleMutex::Unlock() {
    this->flag.clear(std::memory_order_release);
}

inline SimpleLock::SimpleLock(SimpleMutex& mutex) {
    this->mutex = mutex.Lock() ? &mutex : nullptr;
}

inline SimpleLock::~SimpleLock() {
    if (this->mutex) {
        this->mutex->Unlock();
    }
}

inline SimpleLock::SimpleLock(SimpleLock&& other) {
    this->mutex = other.mutex;
    other.mutex = nullptr;
}

inline SimpleLock::operator bool() const {
    return this->mutex;
}
