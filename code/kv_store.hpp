#pragma once

#include "trie.hpp"
#include <string>
#include <string_view>
#include <shared_mutex>
#include <optional>

class KVStore {
public:
    KVStore() = default;

    // Sets a key-value pair. Returns true if an existing key was overwritten.
    bool put(std::string_view key, std::string_view value);

    // Gets a value by its key. Returns an empty optional if not found.
    std::optional<std::string> get(std::string_view key);
    
    // Deletes a key. Returns true if the key existed and was deleted.
    bool del(std::string_view key);

    // Gets the Nth key-value pair lexicographically.
    std::optional<std::pair<std::string, std::string>> get(size_t n);

    // Deletes the Nth key-value pair lexicographically.
    bool del(size_t n);

private:
    Trie trie_;
    // A reader-writer lock for high concurrency on reads
    mutable std::shared_mutex mutex_; 
};