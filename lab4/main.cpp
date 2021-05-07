#include <iostream>
#include <vector>
#include<algorithm>

using namespace std;

//место для предикатов
/*
    equal_to бинарный предикат равенства
    not_equal_to бинарный предикат неравенства
    greater бинарный предикат >
    less бинарный предикат < (используется по умолчанию)
    greater_equal бинарный предикат >=
    less_equal бинарный предикат <=
    logical_and бинарный предикат И
    logical_or бинарный предикат ИЛИ
    logical_not унарный предикат НЕ
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

// место для алгоритмов

template<class Iterator, class T>
bool all_of(Iterator it_first, Iterator it_second, bool (&comp_func)(T, T)) {
    while (it_first != it_second) {
        it_first++;
        if (!comp_func(*prev(it_first), *it_first))
            return false;
    }
    return true;
}

template<class Iterator, class T>
bool any_of(Iterator it_first, Iterator it_second, bool (&comp_func)(T, T)) {
    while (it_first != it_second) {
        it_first++;
        if (comp_func(*prev(it_first), *it_first))
            return true;
    }
    return false;
}

template<class Iterator, class T>
bool none_of(Iterator it_first, Iterator it_second, bool (&comp_func)(T, T)) {
    while (it_first != it_second) {
        it_first++;
        if (comp_func(*prev(it_first), *it_first))
            return false;
    }
    return true;
}

template<class Iterator, class T>
bool one_of(Iterator it_first, Iterator it_second, bool (&comp_func)(T, T)) {
    int count = 0;
    while (it_first != it_second) {
        it_first++;
        if (!comp_func(*prev(it_first), *it_first))
            count++;
        if (count == 1)
            return true;
    }
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
bool is_partitioned() {

}

template<class Iterator, class T>
T find_not(Iterator it_first, Iterator it_second, T elem) {
    while (it_first != it_second) {
        it_first++;
        if (*prev(it_first) != elem)
            return *prev(it_first);
    }
}

template<class Iterator, class T>
T find_backward(Iterator it_first, Iterator it_second, T elem) {
    while (it_second != it_first) {
        it_second--;
        if (*next(it_first) == elem)
            return *next(it_first);
    }
}

template<class Iterator, class T>
bool is_palindrome() {

}

int main() {

    vector<int> vec = {3, 3, 3, 2};

    cout << is_sorted(vec.begin(), vec.end(), my_greater<int>) << "\n";

    cout << is_sorted(vec.begin(), vec.end(), my_less<int>) << "\n";

    cout << find_not(vec.begin(), vec.end(), 3) << "\n";

    cout << find_backward(vec.begin(), vec.end(), 3) << "\n";

    return 0;
}
