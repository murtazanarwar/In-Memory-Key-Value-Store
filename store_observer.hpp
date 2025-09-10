#pragma once

#include <string_view>

// An enum to represent the type of event that occurred in the store.
enum class EventType { PUT, DEL };

// Abstract base class for all store observers.
// This defines the interface that all concrete observers MUST implement.
class StoreObserver {
public:
    // The pure virtual function that concrete classes must override.
    // It's called when an event occurs in the KVStore.
    virtual void onEvent(EventType type, std::string_view key) = 0;

    // A virtual destructor is CRITICAL for base classes in a polymorphic hierarchy.
    // When a derived class object is deleted through a base class pointer,
    // this ensures the correct destructor (the derived one) is called.
    // Without it, you would have a memory leak or undefined behavior.
    virtual ~StoreObserver() = default;
};

/*
    // OOP Concepts in KVStore Example

    1. Abstract Class (StoreObserver):
       - StoreObserver acts like an "interface".
       - It has no implementation of its own (onEvent is a pure virtual function).
       - It defines a strict contract: any class that inherits from StoreObserver
         must provide an implementation for onEvent().

    2. Inheritance (ConsoleObserver, FileObserver):
       - ConsoleObserver and FileObserver inherit from StoreObserver.
       - They fulfill the contract by providing their own implementation of onEvent():
           * ConsoleObserver -> prints to the console.
           * FileObserver    -> writes to a file.

    3. Polymorphism (in KVStore::notify):
       - KVStore stores a list of StoreObserver* (vector<StoreObserver*>).
       - When notify() is called, it loops through the observers and calls:
            observer->onEvent(...)
       - The C++ runtime decides which actual onEvent() to invoke
         based on the true type of the object (ConsoleObserver or FileObserver).
       - This avoids using if/else type-checking, keeping KVStore simple.

    4. Virtual Destructor (in StoreObserver):
       - In main(), objects like ConsoleObserver* or FileObserver*
         may be deleted through a StoreObserver* pointer.
       - If StoreObserver’s destructor is NOT virtual:
           * Only the base class destructor would run.
           * Example: FileObserver’s destructor (that closes file handle) 
             would never execute → resource leak.
       - By declaring a virtual destructor, C++ ensures the
         entire destructor chain is executed correctly.
*/
