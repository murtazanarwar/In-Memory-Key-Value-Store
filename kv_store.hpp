#pragma once

#include "trie.hpp"
#include <string>
#include <string_view>
#include <shared_mutex>
#include <optional>
using namespace std;

//string_view:
//Why string_view was introduced? → To improve efficiency by avoiding copies in read-only string operations.
//When should you use it? → For function parameters when you don’t need ownership or mutation.

//size_t:
//using size_t keeps your API consistent with the STL.
//Index values are never negative. size_t is unsigned
//size_t won’t overflow as quickly as int.

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

//OOPs Concept:

//1.//Abstraction(WHAT): its about hiding complex reality while exposing only the essential parts.
    //i.   Public Section of KVStore CLass -> This is the abstraction. 
    //ii.  From the outside world (like main.cpp), the KVStore object is a simple thing
    //     that can only do a few things: put, get, and del. 
    //iii. A user of your class doesn't need to know how you store the data. 
    //     Is it in a tree? A hash map? A simple list? They don't care, and they don't need to.
    //     The complexity is hidden.
//2.//Encapsulation(HOW): its the mechanism for achieving abstraction. 
    //i.  It bundles the data (attributes) and the methods that operate on that data together,
    //    and restricts access to the internals. This is what creates the "black box."
    //ii. Private Section of KVStore CLass: This contains the complex Trie data structure and the shared_mutex for concurrency. 
    //    The public methods are the only things that can interact with these components. 
    //    This strict boundary is encapsulation
    //iii.Encapsulation prevents any programmer from accidentally forgetting to use the lock or trying to access the trie through a "back door." 
    //    The only door is the public interface, and that door has a guard (the mutex).