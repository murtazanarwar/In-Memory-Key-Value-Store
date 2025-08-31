#pragma once

#include "trie.hpp"
#include <string>
#include <string_view>
#include <shared_mutex>
#include <optional>
using namespace std;

class KVStore {
public:
    KVStore() = default;

    // Sets a key-value pair. Returns true if an existing key was overwritten.
    bool put(string_view key, string_view value);

    // Gets a value by its key. Returns an empty optional if not found.
    optional<string> get(string_view key);
    
    // Deletes a key. Returns true if the key existed and was deleted.
    bool del(string_view key);

    // Gets the Nth key-value pair lexicographically.
    optional<pair<string, string>> get(size_t n);

    // Deletes the Nth key-value pair lexicographically.
    bool del(size_t n);

private:
    Trie trie_;
    // A reader-writer lock for high concurrency on reads
    mutable shared_mutex mutex_; 
};