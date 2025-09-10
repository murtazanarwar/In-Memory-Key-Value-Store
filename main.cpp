#include "kv_store.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <cassert>
#include <fstream>

using namespace std;

// --- Concrete Observer Implementations ---

// 1. ConsoleObserver: Prints events to the standard output.
class ConsoleObserver : public StoreObserver {
public:
    void onEvent(EventType type, string_view key) override {
        switch (type) {
            case EventType::PUT:
                cout << "[Console] PUT: Key '" << key << "' was added or updated." << endl;
                break;
            case EventType::DEL:
                cout << "[Console] DEL: Key '" << key << "' was deleted." << endl;
                break;
        }
    }
};

// 2. FileObserver: Writes events to a log file.
class FileObserver : public StoreObserver {
public:
    FileObserver(const string& filename) {
        // RAII: Open the file in the constructor
        log_file_.open(filename, ios::out | ios::app);
        if (log_file_.is_open()) {
            log_file_ << "--- Log Started ---" << endl;
        }
    }

    ~FileObserver() {
        // RAII: Close the file in the destructor
        if (log_file_.is_open()) {
            log_file_ << "--- Log Ended ---" << endl;
            log_file_.close();
        }
    }

    void onEvent(EventType type, string_view key) override {
        if (!log_file_.is_open()) return;

        switch (type) {
            case EventType::PUT:
                log_file_ << "[File] PUT: Key '" << key << "' was added or updated." << endl;
                break;
            case EventType::DEL:
                log_file_ << "[File] DEL: Key '" << key << "' was deleted." << endl;
                break;
        }
    }

private:
    ofstream log_file_;
};

int main() {
    KVStore store;

    // --- Setup Observers ---
    // We create the observer objects on the heap because their lifetime
    // might need to extend beyond the scope they are created in.
    // In a real application, you might use smart pointers (std::unique_ptr).
    auto* console_logger = new ConsoleObserver();
    auto* file_logger = new FileObserver("kv_store.log");

    // Attach the observers to the store.
    // The KVStore holds them as StoreObserver* pointers. It doesn't know
    // their concrete types, only that they fulfill the StoreObserver "contract".
    store.attach(console_logger);
    store.attach(file_logger);

    cout << "--- Performing operations ---" << endl;

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
    cout << "--- Operations finished ---" << endl << endl;

    // --- Cleanup ---
    // It is crucial to delete the objects to prevent memory leaks.
    // Because StoreObserver has a virtual destructor, deleting the
    // base class pointers will correctly call the destructors for
    // ConsoleObserver and FileObserver, allowing FileObserver to close its file.
    delete console_logger;
    delete file_logger;
    
    cout << "Observers cleaned up. Check 'kv_store.log' for file output." << endl;

    return 0;
}