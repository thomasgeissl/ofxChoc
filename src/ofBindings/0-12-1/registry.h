#pragma once
#include <unordered_map>
#include <memory>

template<typename T>
struct ObjectStore {
    std::unordered_map<int, std::shared_ptr<T>> items;
    int nextId = 0;

    int add(std::shared_ptr<T> obj) {
        int id = nextId++;
        items[id] = std::move(obj);
        return id;
    }

    T* get(int id) const {
        auto it = items.find(id);
        return it != items.end() ? it->second.get() : nullptr;
    }

    void remove(int id) { items.erase(id); }
    void clear()        { items.clear(); }
};
