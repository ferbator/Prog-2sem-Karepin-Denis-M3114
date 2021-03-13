#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

class CPoint {
private:
    float x, y;
public:
    CPoint() : x(0), y(0) {}; // default constructor


    CPoint(float x_, float y_) : x(x_), y(y_) {};

    float get_x() const {
        return x;
    };

    float get_y() const {
        return y;
    };

    CPoint(const CPoint &t) : x(t.get_x()), y(t.get_y()) {}; // copy constructor

    CPoint &operator=(const CPoint &other) {
        if (&other == this) {
            return *this;
        }
        this->x = other.x;
        this->y = other.y;

        return *this;
    }

    void PrintPoint() {
        cout << "Point: (" << x << ", " << y << ")\n";
    };

    float to_zero() {
        return sqrt(pow(x, 2) + pow(y, 2));
    };

    ~CPoint() {}
};

class CPolyline {
protected:
    vector<CPoint> vec;
    float length = 0.0;

    float PointToPoint(CPoint &a, CPoint &b) {
        return sqrt(pow((a.get_x() - b.get_x()), 2) + pow((a.get_y() - b.get_y()), 2));
    }

public:

    CPolyline() {};

    CPolyline(CPoint v[], int k) {
        for (int i = 0; i < k; i++)
            vec.push_back(v[i]);
    };

    CPolyline(vector<CPoint> &v) {
        vec = v;
    };

    void PrintPoints() {
        cout << "PolyLine: ";
        for (int i = 0; i < vec.size(); i++) {
            cout << "(" << vec[i].get_x() << ", " << vec[i].get_y() << ") ";
        }
        cout << "\n";
    };

    float getLength() {
        for (int i = 0; i < vec.size() - 1; i++)
            length += PointToPoint(vec[i], vec[i + 1]);

        return length;
    };

    void operator=(const CPolyline &t) {
        vec = t.vec;
    };

    ~CPolyline() {}
};

class CPolyline_closed : public CPolyline {
public:
    CPolyline_closed() {};

    CPolyline_closed(CPoint m[], int n) : CPolyline(m, n) {};

    CPolyline_closed(vector<CPoint> &m) : CPolyline(m) {};

    CPolyline_closed(CPolyline &p) : CPolyline(p) {};

    float getLength() {
        float length = CPolyline::getLength();

        length += PointToPoint(vec[0], vec[vec.size() - 1]);

        return length;
    }

    void operator=(const CPolyline_closed &t) {
        vec = t.vec;
    }

    ~CPolyline_closed() {}
};

class CPolygon : public CPolyline {
protected:
    float perimeter_ = 0.0;
    float square_ = 0.0;
public:
    CPolygon() {};

    CPolygon(CPoint m[], int n) {
        for (int i = 0; i < n; i++)
            vec.push_back(m[i]);
    };

    CPolygon(vector<CPoint> &m) {
        vec = m;
    };

    CPolygon(CPolygon &p) {
        vec = p.vec;
    };

    float perimeter() {
        for (int i = 0; i < vec.size() - 1; i++)
            perimeter_ += PointToPoint(vec[i], vec[i + 1]);
        perimeter_ += PointToPoint(vec[0], vec[vec.size() - 1]);

        return perimeter_;
    };

    float square() {
        for (int i = 0; i < vec.size(); i++) {
            if (i + 1 == vec.size()) {
                square_ += vec[i].get_x() * vec[0].get_y() - vec[0].get_x() * vec[i].get_y();
            } else {
                square_ += (vec[i].get_x() * vec[i + 1].get_y() - vec[i + 1].get_x() * vec[i].get_y());
            }
        }
        square_ = (abs(square_)) * 0.5;

        return square_;
    };

    ~CPolygon() {}
};

class CTriangle : public CPolygon {
private:
public:
    CTriangle() {};

    CTriangle(CPoint m[], int n) : CPolygon(m, n) {};

    CTriangle(vector<CPoint> &m) : CPolygon(m) {};

    CTriangle(CPolygon &p) : CPolygon(p) {};

    void operator=(const CTriangle &t) {
        vec = t.vec;
    };

    ~CTriangle() {}
};

class CTrapezoid : public CPolygon {
public:
    CTrapezoid() {};

    CTrapezoid(CPoint m[], int n) : CPolygon(m, n) {};

    CTrapezoid(vector<CPoint> &m) : CPolygon(m) {};

    CTrapezoid(CPolygon &p) : CPolygon(p) {};

    void operator=(const CTrapezoid &t) {
        vec = t.vec;
    };

    ~CTrapezoid() {}
};

class CPolygon_regular : public CPolygon {
private:
public:
    CPolygon_regular() {};

    CPolygon_regular(CPoint m[], int n) : CPolygon(m, n) {};

    CPolygon_regular(vector<CPoint> &m) : CPolygon(m) {};

    CPolygon_regular(CPolygon &p) : CPolygon(p) {};

    void operator=(const CPolygon_regular &b) {
        vec = b.vec;
    };

    ~CPolygon_regular() {}
};

int main() {
    // point
    CPoint a(float(2.2), float(3.2));
    a.PrintPoint();
    CPoint b(float(2.0), float(1.2));
    b.PrintPoint();
    cout << "\n";

    // polyline
    CPoint array1[2] = {a, b};
    CPolyline line(array1, 2);
    line.PrintPoints();
    cout << "length line: " << line.getLength() << "\n";
    cout << "\n";

    // closed polyline
    CPoint c(float(5.2), float(3.0));
    CPoint d(float(3.0), float(1.2));
    CPoint e(float(1.2), float(5.2));
    CPoint array2[3] = {c, d, e};
    CPolyline_closed lineclosed(array2, 3);
    lineclosed.PrintPoints();
    cout << "length  line closed: " << lineclosed.getLength() << "\n";
    cout << "\n";

    // polygon
    CPoint x1(float(1.2), float(3.0));
    CPoint x2(float(3.0), float(1.2));
    CPoint x3(float(1.2), float(5.2));
    CPoint x4(float(5.2), float(3.0));
    CPoint array3[4] = {x1, x2, x3, x4};
    CPolygon polygon(array3, 4);
    cout << "perimeter polygon: " << polygon.perimeter() << "\n";
    cout << "square polygon: " << polygon.square() << "\n";
    cout << "\n";

    // triangle
    CPoint x5(float(1.2), float(3.0));
    CPoint x6(float(3.0), float(1.2));
    CPoint x7(float(1.2), float(5.2));
    CPoint array4[3] = {x5, x6, x7};
    CTriangle triangle(array4, 3);
    cout << "perimeter triangle: " << triangle.perimeter() << "\n";
    cout << "square triangle: " << triangle.square() << "\n";
    cout << "\n";

    //trapezoid
    CPoint a1(float(3), float(2));
    CPoint a2(float(5), float(2));
    CPoint a3(float(6), float(6));
    CPoint a4(float(9), float(6));
    CPoint array5[4] = {a1, a2, a3, a4};
    CTrapezoid trapezoid(array5, 4);
    cout << "perimeter trapezoid: " << trapezoid.perimeter() << "\n";
    cout << "square trapezoid: " << trapezoid.square() << "\n";
    cout << "\n";

    // polygon regular
    CPoint b1(float(0), float(0));
    CPoint b2(float(1), float(2));
    CPoint b3(float(3), float(3));
    CPoint b4(float(5), float(2));
    CPoint b5(float(6), float(0));
    CPoint b6(float(5), float(-2));
    CPoint b7(float(3), float(-3));
    CPoint b8(float(1), float(-2));
    CPoint array6[8] = {b1, b2, b3, b4, b5, b6, b7, b8};
    CPolygon_regular polygon_regular(array6, 8);
    cout << "perimeter polygon regular: " << polygon_regular.perimeter() << "\n";
    cout << "square polygon regular: " << polygon_regular.square() << "\n";
    return 0;
}