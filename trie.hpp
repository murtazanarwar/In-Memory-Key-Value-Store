#pragma once

#include <string>
#include <string_view>
#include <memory>
#include <vector>
#include <optional>
using namespace std;

// The number of possible characters in a key (A-Z, a-z, etc.)
constexpr int ALPHABET_SIZE = 58;

class Trie {
public:
    Trie();
    
    // Public API for the data structure
    bool put(string_view key, string_view value);
    optional<string> get(string_view key) const;
    bool remove(string_view key);
    optional<pair<string, string>> getNth(size_t n) const;
    bool removeNth(size_t n);

private:
    struct TrieNode {
        unique_ptr<TrieNode> children[ALPHABET_SIZE];
        string edge_label; // The part of the key this node represents
        optional<string> value; // The actual value stored, if this is a terminal node
        size_t descendants = 0; // Count of terminal nodes in this subtree for Nth element search

        TrieNode(string_view label) : edge_label(label) {}
    };

    unique_ptr<TrieNode> root_;

    // Private helper methods for recursive operations
    bool removeHelper(TrieNode* node, string_view key);
    size_t findMismatch(string_view s1, string_view s2) const;
};