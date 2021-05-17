#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

#define M_PI 3.14159265358979323846

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
        if (&other == this)
            return *this;

        this->x = other.x;
        this->y = other.y;

        return *this;
    }

    void PrintPoint() {
        cout << "Point: (" << x << ", " << y << ")\n";
    };

    static float PointToPoint(CPoint &a, CPoint &b) {
        return sqrt(pow(a.get_x() - b.get_x(), 2) + pow(a.get_y() - b.get_y(), 2));
    };

    ~CPoint() = default;
};

class CPolyline {
protected:
    vector<CPoint> vec;
    float length = 0.0;
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
            length += CPoint::PointToPoint(vec[i], vec[i + 1]);

        return length;
    };

    CPolyline &operator=(const CPolyline &other) {
        if (&other == this)
            return *this;
        vec.pop_back();
        for (int i = 0; i < vec.size(); i++)
            vec.push_back(other.vec[i]);
        return *this;
    };

    ~CPolyline() = default;
};

class CPolyline_closed : public CPolyline {
private:
    float perimeter_ = 0.0;
public:
    CPolyline_closed() {};

    CPolyline_closed(CPoint m[], int n) : CPolyline(m, n) {};

    CPolyline_closed(vector<CPoint> &m) : CPolyline(m) {};

    CPolyline_closed(CPolyline &p) : CPolyline(p) {};

    float perimeter() {
        for (int i = 0; i < vec.size() - 1; i++)
            perimeter_ += CPoint::PointToPoint(vec[i], vec[i + 1]);
        perimeter_ += CPoint::PointToPoint(vec[0], vec[vec.size() - 1]);

        return perimeter_;
    };

    CPolyline_closed &operator=(const CPolyline_closed &other) {
        if (&other == this)
            return *this;
        vec.pop_back();
        for (int i = 0; i < vec.size(); i++)
            vec.push_back(other.vec[i]);
        return *this;
    };

    ~CPolyline_closed() = default;
};

class CPolygon : public CPoint {
protected:
    float perimeter_ = 0.0;
    float square_ = 0.0;
    vector<CPoint> vec;

    bool isConvex(CPoint points[], int n) {
        bool neg = false;
        bool pos = false;
        for (int i = 0; i < n; i++) {
            int a = i;
            int b = (i + 1) % n;
            int c = (i + 2) % n;
            int crossProduct = calc(points[a].get_x(), points[a].get_y(), points[b].get_x(), points[b].get_y(),
                                    points[c].get_x(), points[c].get_y());
            if (crossProduct < 0) neg = true;
            else if (crossProduct > 0) pos = true;
            if (neg && pos) return false;
        }
        return true;
    }

    int calc(int ax, int ay, int bx, int by, int cx, int cy) {
        int BAx = ax - bx;
        int BAy = ay - by;
        int BCx = cx - bx;
        int BCy = cy - by;
        return (BAx * BCy - BAy * BCx);
    }

public:
    CPolygon() {};

    CPolygon(CPoint m[], int n) {
        bool k = isConvex(m, n);
        if (k)
            for (int i = 0; i < n; i++)
                vec.push_back(m[i]);
        else throw "Wrong polygon";
    };

    CPolygon(vector<CPoint> &m) {
        vec = m;
    };

    CPolygon(CPolygon &other) : vec(other.vec) {};


    float perimeter() {
        if (!vec.empty()) {
            for (int i = 0; i < vec.size() - 1; i++)
                perimeter_ += PointToPoint(vec[i], vec[i + 1]);
            perimeter_ += PointToPoint(vec[0], vec[vec.size() - 1]);
        }
        return perimeter_;
    };

    float virtual square() {
        if (!vec.empty()) {
            for (int i = 0; i < vec.size(); i++) {
                if (i + 1 == vec.size()) {
                    square_ += vec[i].get_x() * vec[0].get_y() - vec[0].get_x() * vec[i].get_y();
                } else {
                    square_ += (vec[i].get_x() * vec[i + 1].get_y() - vec[i + 1].get_x() * vec[i].get_y());
                }
            }
            square_ = (abs(square_)) * 0.5;
        }
        return square_;
    };

    CPolygon &operator=(const CPolygon &other) {
        if (&other == this)
            return *this;
        vec.pop_back();
        for (int i = 0; i < vec.size(); i++)
            vec.push_back(other.vec[i]);
        return *this;
    };

    ~CPolygon() = default;
};

class CTriangle : public CPolygon {
private:
    bool On_one_straight_line() {
        float tr = (vec[1].get_x() - vec[0].get_x()) * (vec[2].get_y() - vec[0].get_y()) -
                   (vec[2].get_x() - vec[0].get_x()) * (vec[1].get_y() - vec[0].get_y());
        if (tr == 0)
            return true;
        else
            return false;
    }

public:
    CTriangle() {};

    CTriangle(CPoint m[], int n) : CPolygon(m, n) {
        if (n != 3 || On_one_straight_line())
            throw "Wrong triangle";
    };

    CTriangle(CTriangle &other) : CPolygon(other.vec) {};

    CTriangle(CPolygon &p) : CPolygon(p) {};

    float square() override {
        float AB = PointToPoint(vec[0], vec[1]);
        float BC = PointToPoint(vec[1], vec[2]);
        float CB = PointToPoint(vec[2], vec[0]);
        float p = 0;
        p = this->perimeter();
        p /= 2;

        return sqrt(p / 2 * (p / 2 - AB) * (p / 2 - BC) * (p / 2 - CB));
    };

    CTriangle &operator=(const CTriangle &other) {
        if (&other == this)
            return *this;
        vec.pop_back();
        for (int i = 0; i < vec.size(); i++)
            vec.push_back(other.vec[i]);
        return *this;
    };

    ~CTriangle() = default;
};

class CTrapezoid : public CPolygon {
private:
    bool IsTrapezoid() {
        if ((((vec[1].get_y() - vec[0].get_y()) / (vec[1].get_x() - vec[0].get_x())) !=
             ((vec[3].get_y() - vec[2].get_y()) / (vec[3].get_x() - vec[2].get_x()))) &&
            (((vec[3].get_y() - vec[0].get_y()) / (vec[3].get_x() - vec[0].get_x())) !=
             ((vec[2].get_y() - vec[1].get_y()) / (vec[2].get_x() - vec[1].get_x()))))
            return false;
        else
            return true;
    }

public:
    CTrapezoid() {};

    CTrapezoid(CPoint m[], int n) : CPolygon(m, n) {
        if (!IsTrapezoid())
            throw "Wrong trapezoid";
    };

    CTrapezoid(vector<CPoint> &m) : CPolygon(m) {};

    CTrapezoid(CPolygon &p) : CPolygon(p) {};

    float square() override {
        float AC = PointToPoint(vec[0], vec[2]);
        float BD = PointToPoint(vec[1], vec[3]);
        CPoint vec_AC((vec[0].get_x() - vec[2].get_x()), (vec[0].get_y() - vec[2].get_y()));
        CPoint vec_BD((vec[1].get_x() - vec[3].get_x()), (vec[1].get_y() - vec[3].get_y()));

        float angel = sqrt(1 - pow((vec_AC.get_x() * vec_BD.get_x() + vec_AC.get_y() * vec_BD.get_y()) /
                                   (sqrt(pow(vec_AC.get_x(), 2) + pow(vec_AC.get_y(), 2)) *
                                    sqrt(pow(vec_BD.get_x(), 2) + pow(vec_BD.get_y(), 2))), 2));

        return 0.5 * AC * BD * angel;
    };

    CTrapezoid &operator=(const CTrapezoid &other) {
        if (&other == this)
            return *this;
        vec.pop_back();
        for (int i = 0; i < vec.size(); i++)
            vec.push_back(other.vec[i]);
        return *this;
    };

    ~CTrapezoid() = default;
};

class CPolygon_regular : public CPolygon {
public:
    CPolygon_regular() {};

    CPolygon_regular(CPoint m[], int n) : CPolygon(m, n) {};

    explicit CPolygon_regular(vector<CPoint> &m) : CPolygon(m) {};

    explicit CPolygon_regular(CPolygon &p) : CPolygon(p) {};

    float square() override {
        float a = PointToPoint(vec[0], vec[1]);
        int n = vec.size();
        return (n / 4) * pow(a, 2) * (1 / tan(M_PI / n));
    };

    CPolygon_regular &operator=(const CPolygon_regular &other) {
        if (&other == this)
            return *this;
        vec.pop_back();
        for (int i = 0; i < vec.size(); i++)
            vec.push_back(other.vec[i]);
        return *this;
    };

    ~CPolygon_regular() = default;
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
    cout << "perimetr  line closed: " << lineclosed.perimeter() << "\n";
    cout << "\n";

    // polygon
    CPoint x1(float(2), float(1));
    CPoint x2(float(5), float(1));
    CPoint x3(float(5), float(6));
    CPoint x4(float(2), float(6));
    CPoint array3[4] = {x1, x2, x3, x4};
    CPolygon polygon(array3, 5);
    cout << "perimeter polygon: " << polygon.perimeter() << "\n";
    cout << "square polygon: " << polygon.square() << "\n";
    cout << "\n";

    // triangle
    CPoint x5(float(1), float(1));
    CPoint x6(float(1), float(4));
    CPoint x7(float(5), float(1));
    CPoint array4[3] = {x5, x6, x7};
    CTriangle triangle(array4, 3);
    cout << "perimeter triangle: " << triangle.perimeter() << "\n";
    cout << "square triangle: " << triangle.square() << "\n";
    cout << "\n";

    //trapezoid
    CPoint a1(float(2), float(5));
    CPoint a2(float(1), float(1));
    CPoint a3(float(10), float(1));
    CPoint a4(float(7), float(5));
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
