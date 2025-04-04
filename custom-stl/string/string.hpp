#include <cstddef>
#include <ostream>

struct DataBuffer{
    char *data;
    size_t length;
    size_t capacity;
    size_t refCounter;

    DataBuffer(char *d, size_t len, size_t cap): data(d), length(len), capacity(cap), refCounter(1){};
    ~DataBuffer(){ delete[] data; }
};

class String{
public:
    String();
    String(const char *input_str);
    String(const String &other);
    ~String();

    String &operator=(const String &other);
    char &operator[](size_t index);
    const char &operator[](size_t index) const;
    friend String operator+(const String &a, const String &b);
    String &operator+=(const String &other);
    
    bool operator==(const String &other) const;
    bool operator<(const String &other) const;
    bool operator<=(const String &other) const;
    bool operator>(const String &other) const;
    bool operator>=(const String &other) const;
    bool operator!=(const String &other) const;
    
    friend std::ostream &operator<<(std::ostream &os, const String &my_str);

    friend void my_swap(String &s1, String &s2);

    void ensure_unique();
    size_t my_length(const char *str) const;
    
private:
    DataBuffer *data = nullptr;
};