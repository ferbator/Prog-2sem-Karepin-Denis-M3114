#include <iostream>
#include <vector>
#include<algorithm>

using namespace std;

template<typename type>
bool my_greater(type a, type b) {
    if (a > b)
        return true;
    return false;
}

template<class Iterator, class T>
bool all_of(Iterator it_first, Iterator it_second, bool (&comp_func)(T, T)) {
    while (it_first != it_second) {
        it_first++;
        if (!comp_func(*prev(it_first), *it_first))
            return false;
        return true;
    }
    return false;
}

template<class Iterator, class T>
bool any_of(Iterator it_first, Iterator it_second, bool (&comp_func)(T, T)) {
    while (it_first != it_second) {
        it_first++;
        if (comp_func(*prev(it_first), *it_first))
            return true;
        return false;
    }
    return false;
}

template<class Iterator, class T>
bool none_of(Iterator it_first, Iterator it_second, bool (&comp_func)(T, T)) {
    while (it_first != it_second) {
        it_first++;
        if (comp_func(*prev(it_first), *it_first))
            return false;
        return true;
    }
    return false;
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
        return false;
    }
    return false;
}

int main() {
    vector<int> vec = {1, 2, 3, 4};

    cout << all_of(vec.begin(), vec.end(), my_greater<int>);

    return 0;
}