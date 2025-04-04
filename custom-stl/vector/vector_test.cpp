#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>

#include "vector.hpp"

using namespace std;

int main() {
    // Empty vector
    {
        Vector<int> v;
        assert(v.size() == 0);
    }

    // push_back and indexing
    {
        Vector<int> v;
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);
        assert(v.size() == 3);
        assert(v[0] == 1);
        assert(v[1] == 2);
        assert(v[2] == 3);
    }

    // vector of strings
    {
        Vector<string> v;
        v.push_back("love");
        v.push_back("progtest");

        ostringstream oss;
        for (size_t i = 0; i < v.size(); ++i) {
            oss << v[i];
            if (i + 1 < v.size())
                oss << " ";
        }
        assert(oss.str() == "love progtest");
    }

    // Copy constructor + test COW
    {
        Vector<int> a;
        for (int i = 0; i < 5; ++i)
            a.push_back(i);

        Vector<int> b = a; 

        a[0] = 42; 

        assert(b[0] == 0);
        assert(a[0] == 42);
    }

    // assignment operator
    {
        Vector<int> a;
        for (int i = 0; i < 10; ++i)
            a.push_back(i * i);

        Vector<int> b;
        b = a;

        assert(b.size() == 10);
        assert(b[3] == 9);
        assert(b[9] == 81);
    }

    // erase test
    {
        Vector<int> v;
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);
        v.erase(1);
        assert(v.size() == 2);
        assert(v[0] == 1);
        assert(v[1] == 3);
    }

    // test begin() + end()
    {
        Vector<int> v;
        for (int i = 1; i <= 5; ++i)
            v.push_back(i);

        int sum = 0;
        for (auto it = v.begin(); it != v.end(); ++it)
            sum += *it;

        assert(sum == 15); 
    }

    // insert test - inserting at the beginning, middle, and end
    {
        Vector<int> v;
        v.push_back(1);
        v.push_back(3);
        v.push_back(4);

        v.insert(1, 2);
        v.insert(0, 0);
        v.insert(v.size(), 5); 

        assert(v.size() == 6);
        for (int i = 0; i < 6; ++i)
            assert(v[i] == i);
    }

    {
        Vector<int> a;
        a.push_back(1);
        a.push_back(2);
        a.push_back(3);

        Vector<int> b = a;  // b shares the same buffer as a
        a.insert(1, 42);    // a manually rewrites vector->data (without changing the entire VectorBuffer)
    }

    {
        Vector<int> a;
        a.push_back(1);
        a.push_back(2);
        a.push_back(3);

        Vector<int> b = a;     

        a.insert(1, 99);      
    }

    // Optional: lower_bound test (activate once you have my_lower_bound)
    {
        Vector<int> v;
        int values[] = {1, 3, 5, 7, 9};
        for (int x : values)
            v.push_back(x);
    
        auto it = my_lower_bound(v.begin(), v.end(), 6);
        assert(it != v.end());
        assert(*it == 7);
    
        it = my_lower_bound(v.begin(), v.end(), 9);
        assert(it != v.end());
        assert(*it == 9);
    
        it = my_lower_bound(v.begin(), v.end(), 10);
        assert(it == v.end());
    }

    cout << "All tests passed!" << endl;
    return 0;
}
