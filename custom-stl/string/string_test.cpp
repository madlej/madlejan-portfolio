#include <cassert>
#include <sstream>

#include "string.hpp"

using namespace std;
int main() {
    {
        ostringstream oss;
        String a;
        oss << a;
        assert(oss.str() == "");
    }
    {
        ostringstream oss;
        String a("love");
        String b = "progtest";
        oss << a << " " << b;
        assert(oss.str() == "love progtest");
    }
    {
        ostringstream oss;
        String a("hello");
        String b(a);
        String c = a;
        String d = "hi";
        a = d;
        oss << a << " " << b << " " << c << " " << d;
        assert(oss.str() == "hi hello hello hi");
    }
    {
        ostringstream oss;
        String a, b;
        {
            String c = "stick", d = "finger";
            a = c, b = d;
        }
        String c("through"), d(String("throat"));
        oss << a << " " << b << " " << c << " " << d;
        assert(oss.str() == "stick finger through throat");
    }
    {
        ostringstream oss;
        String a = "a";
        String b = a + "b";
        oss << b;
        assert(oss.str() == "ab");
    }

    {
        ostringstream oss;
        String a = "From";
        String b = "the manhole";
        (a += " ") += b;
        a = a + "" + String("");
        String c = String(" ") + "to";
        a += c;
        String d = " " + String("the manhole");
        String e = d;
        e += " Cyclops rolled the ball.";
        oss << a + e;
        assert(oss.str() == "From the manhole to the manhole Cyclops rolled the ball.");
    }
    {
        ostringstream oss;
        String a(String("Never gonna give you up")), b("Segmentation fault (core dumped)");
        a = a;
        b = a;
        b = b;
        a = b;
        oss << a << " " << b;
        assert(oss.str() == "Never gonna give you up Never gonna give you up");
    }
    {
        assert(String("I could go for lasagna") < String("the cake is a lie"));
    }
    {
        String a = "a", b = "aa", c = "ab", d = c;
        assert(a < b);
        assert(a <= b);
        assert(c <= d);
        assert(d <= c);
        assert(!(b < a));
        assert(!(a >= b));
        assert(b >= a);
        assert(c > a);
        assert(a != c);
        assert(a != b);
        assert(d == c);
    }
}
