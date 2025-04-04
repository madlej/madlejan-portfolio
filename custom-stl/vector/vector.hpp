#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>

template <typename T>
struct VectorBuffer {
    T *data;
    size_t size;
    size_t capacity;
    size_t refCounter;

    VectorBuffer(T *d, size_t s, size_t c): data(d), size(s), capacity(c), refCounter(1) {};
    ~VectorBuffer(){ delete[] data; };
};

template <typename T>
class Vector {
public:
    Vector();
    Vector(const Vector &other);
    ~Vector();
    
    size_t size() const;
    void ensure_unique();
    void push_back(const T &item);
    void insert(size_t index, const T &item);
    void erase(size_t index);
    T* begin() const;
    T* end() const;
        
    T &operator[](size_t index);
    const T &operator[](size_t index) const;
    Vector &operator=(const Vector &other);
    bool operator==(const Vector &other) const;

private:
    VectorBuffer<T> *vector = nullptr;
};

template <typename T>
Vector<T>::Vector(){
    T *data = new T[2];
    vector = new VectorBuffer(data, 0, 2);
}

template <typename T>
Vector<T>::Vector(const Vector &other){
    vector = other.vector;
    vector->refCounter++;
}

template <typename T>
Vector<T>::~Vector(){
    vector->refCounter--;
    if(vector->refCounter <= 0)
        delete vector;
}

template <typename T>
size_t Vector<T>::size() const{
    return vector->size;
}

template <typename T>
void Vector<T>::ensure_unique(){
    if(vector->refCounter == 1)
        return;

    vector->refCounter--;

    T *tmp = new T[vector->capacity];
    for(size_t i = 0; i < vector->size; i++)
        tmp[i] = vector->data[i];
    vector = new VectorBuffer<T>(tmp, vector->size, vector->capacity);
}

template <typename T>
void Vector<T>::push_back(const T& item){
    ensure_unique();

    if (vector->size >= vector->capacity) {
        size_t newCapacity = vector->capacity * 2;
    
        T* tmp = new T[newCapacity];
        for (size_t i = 0; i < vector->size; ++i)
            tmp[i] = vector->data[i];
    
        delete[] vector->data;
        vector->data = tmp;
        vector->capacity = newCapacity;
    }
    
    vector->data[vector->size++] = item;
}


template <typename T>
void Vector<T>::insert(size_t index, const T &item){
    if (index > vector->size)
        throw std::out_of_range("Vector::insert - index out of range");

    ensure_unique();

    if(vector->size >= vector->capacity){
        size_t newCapacity = vector->capacity * 2;

        T* tmp = new T[newCapacity];
        for (size_t i = 0; i < vector->size; ++i)
            tmp[i] = vector->data[i];
    
        delete[] vector->data;
        vector->data = tmp;
        vector->capacity = newCapacity;
    }

    for(size_t i = vector->size; i > index; i--)
        vector->data[i] = vector->data[i - 1];
    
    vector->data[index] = item;
    vector->size++;
}


template <typename T>
void Vector<T>::erase(size_t index){
    ensure_unique();

    if (index >= vector->size)
        throw std::out_of_range("Vector::erase - index out of range");

    for(size_t i = index; i < vector->size - 1; i++)
        vector->data[i] = vector->data[i + 1];
    
    vector->size--;  
}

template <typename T>
T* Vector<T>::begin() const{ return vector->data; }

template <typename T>
T* Vector<T>::end() const{ return vector->data + vector->size; }

template <typename T>
T &Vector<T>::operator[](size_t index) {
    if (index >= vector->size)
        throw std::out_of_range("Vector::operator[] - index out of range");
    
    this->ensure_unique();
    return vector->data[index];
}

template <typename T>
const T &Vector<T>::operator[](size_t index) const{
    if (index >= vector->size)
        throw std::out_of_range("Vector::operator[] - index out of range");
    return vector->data[index];
}

template <typename T>
bool Vector<T>::operator==(const Vector<T> &other) const{
    if(this->vector->size != other.vector->size)
        return false;
    for(size_t i = 0; i < this->vector->size; i++){
        if(this->vector->data[i] != other.vector->data[i])
            return false;
    }
    return true;
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) {
    if (this == &other)
        return *this;
    Vector tmp(other);
    std::swap(this->vector, tmp.vector);
    return *this;
}



template <typename Iterator, typename T>
Iterator my_lower_bound(Iterator begin, Iterator end, const T &value) {
    while(true){
        if(begin >= end)
            return begin;
        
        Iterator mid = begin + (end - begin) / 2;
        if (*mid < value)
            begin = mid + 1;
        else
            end = mid;
    }
}