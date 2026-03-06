#ifndef VECTOR_H_
#define VECTOR_H_

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <algorithm> 

using namespace std;




template <typename T>
class Vector {
private:
    T* data;
    int size;      
    int capacity;  

public:

    explicit Vector(int n = 0) {
        if (n < 0) n = 0;
        size = n;
        capacity = (n > 0) ? n : 2;

        data = new T[capacity];

        for (int i = 0; i < size; i++) {
            data[i] = T();
        }
    }


    ~Vector() {
        delete[] data;
    }


    Vector(const Vector& other) {
        size = other.size;
        capacity = other.capacity;

        data = new T[capacity];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }


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

    void push(const T& input) {
        if (size >= capacity) {
            int newCapacity = (capacity == 0) ? 2 : (capacity * 3) / 2;
            resize(newCapacity);
        }

        data[size] = input;
        size++;
    }

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

    int getSize() const { return size; }
    int getCapacity() const { return capacity; }
    bool isEmpty() const { return size == 0; }
};

#endif