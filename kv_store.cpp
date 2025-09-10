#include "kv_store.hpp"
#include <mutex>
#include <shared_mutex>
using namespace std;

bool KVStore::put(string_view key, string_view value) {
    // Acquire an exclusive (unique) lock for writing
    unique_lock lock(mutex_);
    bool result = trie_.put(key, value);
    
    // Notify observers about the PUT event before releasing the lock [RAII]
    notify(EventType::PUT, key);
    return result;
}

optional<string> KVStore::get(string_view key) {
    // Acquire a shared lock for reading, allowing other readers
    shared_lock lock(mutex_);
    return trie_.get(key);
}

bool KVStore::del(string_view key) {
    unique_lock lock(mutex_);
    bool result = trie_.remove(key);

    // Notify observers about the DEL event before releasing the lock
    notify(EventType::DEL, key);
    return result;
}

optional<pair<string, string>> KVStore::get(size_t n) {
    shared_lock lock(mutex_);
    return trie_.getNth(n);
}

bool KVStore::del(size_t n) {
    unique_lock lock(mutex_);
    return trie_.removeNth(n);
}

void KVStore::attach(StoreObserver* observer) {
    // No lock needed here if attach is only called during setup
    // before multiple threads are running. For a fully dynamic system,
    // this would also need a unique_lock.
    observers_.push_back(observer);
}

void KVStore::notify(EventType type, string_view key) {
    // This is the polymorphic call.
    // The code iterates through a vector of `StoreObserver*`, but the objects
    // can be `ConsoleObserver`, `FileObserver`, or any other future observer type.
    // `observer->onEvent(...)` calls the correct implementation for each object.
    for (StoreObserver* observer : observers_) {
        if (observer) {
            observer->onEvent(type, key);
        }
    }
}