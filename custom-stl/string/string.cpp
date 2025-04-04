#include "string.hpp"
size_t String::my_length(const char *str) const{
    size_t length = 0;
    while(str[length] != '\0')
        length++;        
    return length;
}

void String::ensure_unique(){
    if(data->refCounter == 1)
        return;

    data->refCounter--;
    char *tmp = new char[data->capacity];

    for(size_t i = 0; i <= data->length; i++)
        tmp[i] = data->data[i];
    data = new DataBuffer(tmp, data->length, data->capacity);
}

void my_swap(String &s1, String &s2){
    std::swap(s1.data, s2.data);
}

String::String(){
    char *tmp = new char[1];
    tmp[0] = '\0';
    data = new DataBuffer(tmp, 0, 1);
}

String::String(const char *input_str) {
    size_t capacity = my_length(input_str) + 1;
    size_t length = capacity - 1;
    char *tmp = new char[capacity];
    for(size_t i = 0; i < capacity; i++)
        tmp[i] = input_str[i]; 
    data = new DataBuffer(tmp, length, capacity);
}

String::String(const String &other) {
    data = other.data;
    data->refCounter++;
}

String::~String(){
    data->refCounter--;
    if(data->refCounter <= 0)
        delete data;
}

std::ostream &operator<<(std::ostream &os, const String &my_str) {
    for(size_t i = 0; i < my_str.data->length; i++)
        os << my_str.data->data[i];
    return os;
}
 
String &String::operator=(const String &other){
    if (this == &other)
        return *this;
    String tmp(other);
    my_swap(*this, tmp);
    return *this;
}

String &String::operator+=(const String &other) {
    this->ensure_unique();
    
    size_t new_length = this->data->length + other.data->length;
    size_t new_capacity = new_length + 1;    
    char *new_data = new char[new_capacity];

    for(size_t i = 0; i < this->data->length; i++) {
        new_data[i] = this->data->data[i];
    }
    for (size_t i = 0; i < other.data->length; i++) {
        new_data[this->data->length + i] = other.data->data[i];
    }
    new_data[new_length] = '\0'; 

    DataBuffer *oldData = this->data;
    this->data = new DataBuffer(new_data, new_length, new_capacity);
    
    oldData->refCounter--;
    if(oldData->refCounter == 0) 
        delete oldData;

    return *this;
}

String operator+(const String &a, const String &b){
    String tmp(a);
    return tmp += b;
}

char &String::operator[](size_t index) {
    if (index >= data->length)
        throw std::out_of_range("String::operator[] - index out of range");

    this->ensure_unique();
    return data->data[index];
}

const char &String::operator[](size_t index) const {
    if (index >= data->length)
        throw std::out_of_range("String::operator[] - index out of range");
    return data->data[index];
}

bool String::operator==(const String &other) const{
    if(this->data->length != other.data->length)
        return false;
    for(size_t i = 0; i < this->data->length; i++){
        if(this->data->data[i] != other.data->data[i])
            return false;
    }
    return true;
}


bool String::operator<(const String &other) const {
    size_t min_length = (data->length < other.data->length) ? data->length : other.data->length;
    for (size_t i = 0; i < min_length; i++) {
        if (data->data[i] < other.data->data[i])
            return true;
        if (data->data[i] > other.data->data[i])
            return false;
    }
    return data->length < other.data->length;
}

bool String::operator<=(const String &other) const {
    return *this < other || *this == other;
}

bool String::operator>(const String &other) const {
    return other < *this;
}

bool String::operator>=(const String &other) const {
    return !(*this < other);
}

bool String::operator!=(const String &other) const {
    return !(*this == other);
}