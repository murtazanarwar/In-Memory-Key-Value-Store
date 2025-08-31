#include "kv_store.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <cassert>

int main() {
    KVStore store;

    std::cout << "Testing basic PUT and GET...\n";
    store.put("Apple", "A sweet red fruit");
    store.put("Banana", "A sweet yellow fruit");
    
    assert(store.get("Apple").value_or("") == "A sweet red fruit");
    assert(store.get("Banana").value_or("") == "A sweet yellow fruit");
    assert(!store.get("Carrot").has_value());
    
    std::cout << "Basic tests passed.\n\n";

    std::cout << "Testing concurrent reads and writes...\n";

    std::vector<std::thread> threads;

    // Writer thread
    threads.emplace_back([&]() {
        store.put("Carrot", "An orange vegetable");
        std::cout << "Writer thread: Added Carrot.\n";
        store.put("Apple", "A crunchy fruit"); // Overwrite
        std::cout << "Writer thread: Updated Apple.\n";
    });

    // Reader threads
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([&, i]() {
            // Readers will see different states depending on timing
            auto apple_val = store.get("Apple");
            auto carrot_val = store.get("Carrot");
            
            std::cout << "Reader " << i << " sees Apple: " << apple_val.value_or("N/A") << "\n";
            std::cout << "Reader " << i << " sees Carrot: " << carrot_val.value_or("N/A") << "\n";
        });
    }

    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "\nConcurrency test finished.\n";
    
    std::cout << "Final value of Apple: " << store.get("Apple").value() << std::endl;
    assert(store.get("Apple").value() == "A crunchy fruit");
    assert(store.get("Carrot").has_value());
    
    std::cout << "All tests passed!\n";

    return 0;
}