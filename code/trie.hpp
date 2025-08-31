#pragma once

#include <string>
#include <string_view>
#include <memory>
#include <vector>
#include <optional>

// The number of possible characters in a key (A-Z, a-z, etc.)
constexpr int ALPHABET_SIZE = 58;

class Trie {
public:
    Trie();
    
    // Public API for the data structure
    bool put(std::string_view key, std::string_view value);
    std::optional<std::string> get(std::string_view key) const;
    bool remove(std::string_view key);
    std::optional<std::pair<std::string, std::string>> getNth(size_t n) const;
    bool removeNth(size_t n);

private:
    struct TrieNode {
        std::unique_ptr<TrieNode> children[ALPHABET_SIZE];
        std::string edge_label; // The part of the key this node represents
        std::optional<std::string> value; // The actual value stored, if this is a terminal node
        size_t descendants = 0; // Count of terminal nodes in this subtree for Nth element search

        TrieNode(std::string_view label) : edge_label(label) {}
    };

    std::unique_ptr<TrieNode> root_;

    // Private helper methods for recursive operations
    bool removeHelper(TrieNode* node, std::string_view key);
    size_t findMismatch(std::string_view s1, std::string_view s2) const;
};