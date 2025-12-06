#ifndef VECTOR_H
#define VECTOR_H

#include <stdexcept>
#include <initializer_list>

template <typename T>
class Vector {
private:
    T* data;
    int capacity;
    int count;

    void resize(int newCapacity) {
        T* newData = new T[newCapacity];
        for (int i = 0; i < count; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    Vector() : data(nullptr), capacity(0), count(0) {}
    
    // Initializer list constructor
    Vector(std::initializer_list<T> init) : data(nullptr), capacity(0), count(0) {
        if (init.size() > 0) {
            capacity = init.size();
            count = init.size();
            data = new T[capacity];
            int i = 0;
            for (const T& item : init) {
                data[i++] = item;
            }
        }
    }
    
    ~Vector() {
        delete[] data;
    }
    
    // Copy constructor
    Vector(const Vector& other) : data(nullptr), capacity(0), count(0) {
        if (other.count > 0) {
            capacity = other.count;
            count = other.count;
            data = new T[capacity];
            for (int i = 0; i < count; i++) {
                data[i] = other.data[i];
            }
        }
    }
    
    // Assignment operator
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data;
            data = nullptr;
            capacity = 0;
            count = 0;
            
            if (other.count > 0) {
                capacity = other.count;
                count = other.count;
                data = new T[capacity];
                for (int i = 0; i < count; i++) {
                    data[i] = other.data[i];
                }
            }
        }
        return *this;
    }
    
    void push_back(const T& value) {
        if (count >= capacity) {
            int newCapacity = (capacity == 0) ? 4 : capacity * 2;
            resize(newCapacity);
        }
        data[count++] = value;
    }
    
    void erase(int index) {
        if (index < 0 || index >= count) {
            throw std::out_of_range("Index out of range");
        }
        
        for (int i = index; i < count - 1; i++) {
            data[i] = data[i + 1];
        }
        count--;
    }
    
    void clear() {
        delete[] data;
        data = nullptr;
        capacity = 0;
        count = 0;
    }
    
    int size() const {
        return count;
    }
    
    bool empty() const {
        return count == 0;
    }
    
    T& operator[](int index) {
        if (index < 0 || index >= count) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
    
    const T& operator[](int index) const {
        if (index < 0 || index >= count) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
    
    T& at(int index) {
        if (index < 0 || index >= count) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
    
    const T& at(int index) const {
        if (index < 0 || index >= count) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
    
    T* begin() {
        return data;
    }
    
    const T* begin() const {
        return data;
    }
    
    T* end() {
        return data + count;
    }
    
    const T* end() const {
        return data + count;
    }
    
    void assign(const Vector& other) {
        *this = other;
    }
    
    // Resize to new size (public method, different from private resize)
    void resize(int newSize) {
        if (newSize < 0) {
            throw std::out_of_range("Size cannot be negative");
        }
        if (newSize > capacity) {
            int newCapacity = (capacity == 0) ? 4 : capacity * 2;
            while (newCapacity < newSize) {
                newCapacity *= 2;
            }
            resize(newCapacity);
        }
        // If newSize > count, new elements are default-constructed
        // If newSize < count, elements are removed
        count = newSize;
    }
    
    T& front() {
        if (count == 0) {
            throw std::out_of_range("Vector is empty");
        }
        return data[0];
    }
    
    const T& front() const {
        if (count == 0) {
            throw std::out_of_range("Vector is empty");
        }
        return data[0];
    }
};

#endif


