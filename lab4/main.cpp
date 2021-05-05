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

int main() {

    vector<int> vec = {1, 2, 3, 4};

    cout << is_sorted(vec.begin(), vec.end(), my_greater<int>) << "\n";

    cout << is_sorted(vec.begin(), vec.end(), my_less<int>);

    return 0;
}
