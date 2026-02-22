#include "Vector.h"

	template <typename T>


	Vector<T>::Vector(int n) {
		size = n;

		capacity = (n > 0) ? (int)round(n * 1.5) : 2;

		data = new T[capacity];
		lastIndex = 0;

		for (int i = 0; i < size; i++) data[i] = 0;
	}
	template <typename T>

	Vector<T>::~Vector() {
		delete[] data;
	}

	Vector<T>::T& operator[](int index) {
		if (index < 0 || index >= size) {
			cerr << "Vector index out of bounds!" << std::endl;
			exit(1);
		}
		return data[index];
	}

	void Vector<T>::push(T input) {

		if (lastIndex == capacity) {
			int newCapacity = (capacity == 0) ? 2 : (int)round(capacity * 1.5);
			resize(newCapacity);
		}

		data[lastIndex] = input;

		lastIndex++;

		if (lastIndex > size) {
			size = lastIndex;
		}
	}

	int Vector::getSize() const {
		return size;
	}

	int Vector::getCapacity() const {
		return capacity;
	}

	int getLastIndex() const {
		return lastIndex;
	}

	void resize(int newCapacity) {

		T* newData = new T[newCapacity];

		for (int i = 0; i < size; i++) {
			newData[i] = data[i];
		}

		delete[] data;
		data = newData;
		capacity = newCapacity;
	}

