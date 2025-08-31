#include "kv_store.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <cassert>
using namespace std;

int main() {
    KVStore store;

    cout << "Testing basic PUT and GET...\n";
    store.put("Apple", "A sweet red fruit");
    store.put("Banana", "A sweet yellow fruit");
    
    assert(store.get("Apple").value_or("") == "A sweet red fruit");
    assert(store.get("Banana").value_or("") == "A sweet yellow fruit");
    assert(!store.get("Carrot").has_value());
    
    cout << "Basic tests passed.\n\n";

    cout << "Testing concurrent reads and writes...\n";

    vector<thread> threads;

    // Writer thread
    threads.emplace_back([&]() {
        store.put("Carrot", "An orange vegetable");
        cout << "Writer thread: Added Carrot.\n";
        store.put("Apple", "A crunchy fruit"); // Overwrite
        cout << "Writer thread: Updated Apple.\n";
    });

    // Reader threads
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([&, i]() {
            // Readers will see different states depending on timing
            auto apple_val = store.get("Apple");
            auto carrot_val = store.get("Carrot");
            
            cout << "Reader " << i << " sees Apple: " << apple_val.value_or("N/A") << "\n";
            cout << "Reader " << i << " sees Carrot: " << carrot_val.value_or("N/A") << "\n";
        });
    }

    for (auto& t : threads) {
        t.join();
    }
    
    cout << "\nConcurrency test finished.\n";
    
    cout << "Final value of Apple: " << store.get("Apple").value() << endl;
    assert(store.get("Apple").value() == "A crunchy fruit");
    assert(store.get("Carrot").has_value());
    
    cout << "All tests passed!\n";

    return 0;
}