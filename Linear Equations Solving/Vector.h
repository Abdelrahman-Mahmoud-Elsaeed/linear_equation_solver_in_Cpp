#ifndef VECTOR_H_
#define VECTOR_H_

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <algorithm> 

using namespace std;

// Vector<string> v(50);
// Vector<float> 
// Vector<int> 
// Vector<char> 



// int x = 0

template <typename T>
class Vector {
private:
    T* data;
    int size;      
    int capacity;  

public:
    // --- 1. Constructor ---
    explicit Vector(int n = 0) {
        if (n < 0) n = 0;
        size = n;
        capacity = (n > 0) ? n : 2;

        data = new T[capacity];

        for (int i = 0; i < size; i++) {
            data[i] = T();
        }
    }

    // --- 2. Destructor ---
    ~Vector() {
        delete[] data;
    }

    // --- 3. Copy Constructor ---
    Vector(const Vector& other) {
        size = other.size;
        capacity = other.capacity;

        data = new T[capacity];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    // --- 4. Assignment Operator ---
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            if (capacity < other.size) {
                delete[] data;
                capacity = other.capacity;
                data = new T[capacity];
            }

            size = other.size;
            for (int i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
    // --- 5. Operator [] ---
    T& operator[](int index) {
        if (index < 0 || index >= size) {
            cerr << "Vector index out of bounds! Index: " << index << ", Size: " << size << endl;
            exit(1);
        }
        return data[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= size) {
            cerr << "Vector index out of bounds!" << endl;
            exit(1);
        }
        return data[index];
    }
     // arr(50)
     // 
    // --- 6. Push (Optimized) ---
    void push(const T& input) {
        if (size >= capacity) {
            int newCapacity = (capacity == 0) ? 2 : (capacity * 3) / 2;
            resize(newCapacity);
        }

        data[size] = input;
        size++;
    }

    // --- 7. Resize ---
    void resize(int newCapacity) {
        if (newCapacity < size) return; 

        T* newData = new T[newCapacity];

        for (int i = 0; i < size; i++) {
            newData[i] = data[i];
        }

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

    // --- Getters ---
    int getSize() const { return size; }
    int getCapacity() const { return capacity; }
    bool isEmpty() const { return size == 0; }
};

#endif