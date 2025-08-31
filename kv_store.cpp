#include "kv_store.hpp"
#include <mutex>        // For unique_lock
#include <shared_mutex> // For shared_lock
using namespace std;

bool KVStore::put(string_view key, string_view value) {
    // Acquire an exclusive (unique) lock for writing
    unique_lock lock(mutex_);
    return trie_.put(key, value);
}

optional<string> KVStore::get(string_view key) {
    // Acquire a shared lock for reading, allowing other readers
    shared_lock lock(mutex_);
    return trie_.get(key);
}

bool KVStore::del(string_view key) {
    unique_lock lock(mutex_);
    return trie_.remove(key);
}

optional<pair<string, string>> KVStore::get(size_t n) {
    shared_lock lock(mutex_);
    return trie_.getNth(n);
}

bool KVStore::del(size_t n) {
    unique_lock lock(mutex_);
    return trie_.removeNth(n);
}