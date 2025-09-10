#pragma once

#include "trie.hpp"
#include "store_observer.hpp"
#include <string>
#include <string_view>
#include <shared_mutex>
#include <optional>
#include <vector> //to hold observers
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

    // Attaches an observer to listen for store events.
    void attach(StoreObserver* observer);

private:
    // Notifies all attached observers of an event.
    void notify(EventType type, string_view key);

    Trie trie_;

    // A reader-writer lock for high concurrency on reads
    mutable shared_mutex mutex_; 

    // A list of pointers to observers. We use base class pointers
    // to hold any object of a derived observer type (polymorphism).
    vector<StoreObserver*> observers_;

};

/*
==================================================
                OOPs Concepts
==================================================

OOP has 4 major pillars: Abstraction, Encapsulation, Inheritance, and Polymorphism.

--------------------------------------------------
1. Abstraction (WHAT)
--------------------------------------------------
- Definition:
    Hiding complex implementation details and exposing
    only the essential features to the user.
- Goal:
    Show "what" an object does, not "how" it does it.
- Example (KVStore):
    i.   Public interface: put(), get(), del().
    ii.  From main.cpp, KVStore looks simple.
    iii. User does not know/care if data is stored in a
         Trie, HashMap, or List.
- Analogy:
    TV remote → You press a button to change channels,
    but you don’t know the internal circuit.

--------------------------------------------------
2. Encapsulation (HOW)
--------------------------------------------------
- Definition:
    Mechanism of bundling data (variables) and methods
    (functions) together into a single unit (class).
    It also restricts direct access to internal data.
- Goal:
    Protect data, enforce controlled access.
- Example (KVStore):
    i.   Private section: Trie + shared_mutex.
    ii.  Only public functions (put/get/del) can touch
         these internals.
    iii. Ensures users cannot bypass locks or corrupt
         data → a strict boundary ("black box").
- Analogy:
    Capsule medicine → multiple ingredients sealed in 
    one unit, safe for use.

--------------------------------------------------
3. Inheritance (IS-A relationship)
--------------------------------------------------
- Definition:
    Process of creating a new class (derived) from an
    existing one (base), reusing and extending code.
- Goal:
    Code reusability, hierarchical relationships.
- Types in C++:
    * Single
    * Multiple
    * Multilevel
    * Hierarchical
    * Hybrid
- Example:
    class Vehicle { };
    class Car : public Vehicle { };
    --> Car "is-a" Vehicle.
- Analogy:
    Child inherits traits from parent.

--------------------------------------------------
4. Polymorphism (MANY FORMS)
--------------------------------------------------
- Definition:
    Ability of functions or objects to take multiple
    forms depending on context.
- Types:
    A) Compile-time (Static)
       * Function overloading
       * Operator overloading
    B) Run-time (Dynamic)
       * Virtual functions (via vtable)
- Example:
    class Animal { virtual void sound(); };
    class Dog : public Animal { void sound() override; };
    Animal* a = new Dog();
    a->sound();  // "Woof!" (runtime decision)
- Key Note:
    Always make base destructor `virtual` if using
    polymorphism.
- Analogy:
    The "+" symbol → adds ints, concatenates strings,
    works differently in each context.

--------------------------------------------------
QUICK SUMMARY (Interview-ready one-liners):
--------------------------------------------------
- Abstraction → Hides implementation, shows essentials.
- Encapsulation → Hides data, enforces boundaries.
- Inheritance → Reuse + "is-a" relationship.
- Polymorphism → Same interface, different behavior.

==================================================
*/
