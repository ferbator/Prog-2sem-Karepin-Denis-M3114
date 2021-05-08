#include <iostream>
#include <vector>
#include <set>

using namespace std;

class CPoint {
private:
    float y, x;
public:
    CPoint() : x(0), y(0) {}; // default constructor

    CPoint(float x_, float y_) : x(x_), y(y_) {};

    float get_x() const {
        return x;
    };

    float get_y() const {
        return y;
    };

    CPoint(const CPoint &t) : x(t.x), y(t.y) {}; // copy constructor

    bool operator==(const CPoint &other) const {
        if (this->x == other.x && this->y == other.y)
            return true;
        else
            return false;
    }

    bool operator>(const CPoint &other) const {
        if (this->x > other.x && this->y > other.y)
            return true;
        else
            return false;
    }

    bool operator<(const CPoint &other) const {
        if (this->x < other.x && this->y < other.y)
            return true;
        else
            return false;
    }

    CPoint &operator=(const CPoint &other) {
        if (&other == this)
            return *this;

        this->x = other.x;
        this->y = other.y;

        return *this;
    }

    ~CPoint() = default;
};

ostream &operator<<(ostream &out, const CPoint &p) {
    out << p.get_x() << " " << p.get_y();

    return out;
}

//место для предикатов
/*
    equal_to бинарный предикат равенства ==
    not_equal_to бинарный предикат неравенства !=
    greater бинарный предикат >
    less бинарный предикат <
    greater_equal бинарный предикат >=
    less_equal бинарный предикат <=
 */

template<typename type>
bool my_greater(type a, type b) {
    if (a > b)
        return true;
    return false;
}

template<typename type>
bool my_less(type a, type b) {
    if (a < b)
        return true;
    return false;
}

template<typename type>
bool my_less_equal(type a, type b) {
    if (a <= b)
        return true;
    return false;
}

template<typename type>
bool my_equal_to(type a, type b) {
    if (a == b)
        return true;
    return false;
}

template<typename type>
bool my_not_equal_to(type a, type b) {
    if (a != b)
        return true;
    return false;
}

template<typename type>
bool my_parity(type a, type b) {
    if ((a + b) % 2 == 0)
        return true;
    return false;
}

// место для алгоритмов

template<class Iterator, class T>
bool all_of(Iterator it_first, Iterator it_second, T elem, bool (&comp_func)(T, T)) {
    while (it_first != it_second) {
        if (!comp_func(*it_first, elem))
            return false;

        it_first++;
    }
    return true;
}

template<class Iterator, class T>
bool any_of(Iterator it_first, Iterator it_second, T elem, bool (&comp_func)(T, T)) {
    while (it_first != it_second) {
        if (comp_func(*it_first, elem))
            return true;

        it_first++;
    }
    return false;
}

template<class Iterator, class T>
bool none_of(Iterator it_first, Iterator it_second, T elem, bool (&comp_func)(T, T)) {
    while (it_first != it_second) {
        if (comp_func(*it_first, elem))
            return false;

        it_first++;
    }
    return true;
}

template<class Iterator, class T>
bool one_of(Iterator it_first, Iterator it_second, T elem, bool (&comp_func)(T, T)) {
    int count = 0;
    while (it_first != it_second) {
        if (!comp_func(*it_first, elem))
            count++;

        it_first++;
    }
    if (count == 1)
        return true;

    return false;
}

template<class Iterator, class T>
bool is_sorted(Iterator it_first, Iterator it_second, bool (&comp_func)(T, T)) {
    while (it_first != it_second) {
        it_first++;
        if (!comp_func(*prev(it_first), *it_first))
            return false;
    }
    return true;
}

template<class Iterator, class T>
bool is_partitioned(Iterator it_first, Iterator it_second, T elem, bool (&comp_func)(T, T)) {
    int count = 0;
    if (comp_func(*it_first, elem)) {
        while (it_first != it_second && comp_func(*it_first, elem))
            it_first++;

        if (it_first == it_second)
            return false;

        while (it_first != it_second) {
            if (comp_func(*it_first, elem))
                return false;
            it_first++;
            count++;
        }
    } else {
        while (it_first != it_second && !comp_func(*it_first, elem))
            it_first++;

        if (it_first == it_second)
            return false;

        while (it_first != it_second) {
            if (!comp_func(*it_first, elem))
                return false;
            it_first++;
        }
    }

    return true;
}

template<class Iterator, class T>
Iterator find_not(Iterator it_first, Iterator it_second, T elem) {
    while (it_first != it_second) {
        if (*it_first != elem) {
            return it_first;
        }
        it_first++;
    }

    return it_second;
}

template<class Iterator, class T>
Iterator find_backward(Iterator it_first, Iterator it_second, T elem) {
    it_second--;
    Iterator end = it_second;
    while (it_second != it_first) {
        if (*it_second == elem) {
            return it_second;
        }
        it_second--;
    }

    if (it_first == it_second && *it_first == elem) {
        return it_first;
    }

    return end;
}

template<class Iterator, class T>
bool is_palindrome(Iterator it_first, Iterator it_second, bool (&comp_func)(T, T)) {
    it_second--;
    while (it_first != it_second) {
        if (!comp_func(*it_first, *it_second))
            return false;

        if ((it_first + 1) == it_second)
            break;

        it_first++;
        it_second--;
    }

    return true;
}

int main() {

    vector<int> vec = {1, 2, 5};
    CPoint a(2, 3);
    CPoint b(3, 4);
    CPoint c(2, 3);
    vector<CPoint> vec_point = {a, b, c};
    set<int> set = {2, 1, 2, 0};

    cout << all_of(set.begin(), set.end(), 3, my_less<int>) << "\n";
    CPoint d(2, 3);
    cout << all_of(vec_point.begin(), vec_point.end(), d, my_less<CPoint>) << "\n";

    cout << any_of(vec.begin(), vec.end(), 3, my_less<int>) << "\n";

    cout << is_sorted(vec.begin(), vec.end(), my_greater<int>) << "\n";

    cout << is_sorted(vec.begin(), vec.end(), my_less<int>) << "\n";

    cout << *find_not(vec.begin(), vec.end(), 3) << "\n";

    cout << *find_backward(vec.begin(), vec.end(), 3) << "\n";

    cout << is_partitioned(vec.begin(), vec.end(), 4, my_less_equal<int>) << "\n";

    cout << is_palindrome(vec.begin(), vec.end(), my_parity<int>) << "\n";

    cout << is_palindrome(vec_point.begin(), vec_point.end(), my_equal_to<CPoint>) << "\n";

    return 0;
}
