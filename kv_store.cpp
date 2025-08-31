#include "kv_store.hpp"
#include <mutex>        // For std::unique_lock
#include <shared_mutex> // For std::shared_lock

bool KVStore::put(std::string_view key, std::string_view value) {
    // Acquire an exclusive (unique) lock for writing
    std::unique_lock lock(mutex_);
    return trie_.put(key, value);
}

std::optional<std::string> KVStore::get(std::string_view key) {
    // Acquire a shared lock for reading, allowing other readers
    std::shared_lock lock(mutex_);
    return trie_.get(key);
}

bool KVStore::del(std::string_view key) {
    std::unique_lock lock(mutex_);
    return trie_.remove(key);
}

std::optional<std::pair<std::string, std::string>> KVStore::get(size_t n) {
    std::shared_lock lock(mutex_);
    return trie_.getNth(n);
}

bool KVStore::del(size_t n) {
    std::unique_lock lock(mutex_);
    return trie_.removeNth(n);
}