#include <iostream>

using namespace std;

template<class CType>
class CircularBuffer {
private:
    int head = 0;
    int tail = 0;
    int capacity = 0;
    CType *buffer = nullptr;
public:
    class Iterator : public iterator<random_access_iterator_tag, CType> {
    private:
        CType *pointer;
    public:

    };

    CircularBuffer<CType>(size_t value) {
        this->capacity = value;
        buffer = new CType[capacity];
    }

    void push_back(CType value) {
        if (tail >= capacity) {
            buffer[tail] = value;
            tail = capacity;
        } else {
            buffer[tail] = value;
            tail++;
        }
    }

    void pop_back() {
        buffer[tail] = 0;
        tail--;
    }

    CircularBuffer operator[](int i) {
        return *(buffer + i);
    }

};

int main() {
    CircularBuffer<int> buf(10);

    buf.push_back(1);
    buf.pop_back();
    buf.pop_back();

    buf.push_back(2);
    buf.push_back(3);
    buf.push_back(4);
    buf.push_back(5);

    buf.operator[](1);
    buf[1];

    return 0;
}
