#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class CPolynomial {
private:
    vector<float> coe_table;

    static void set_coefficients(const bool was_x, unsigned &power, double &digit, CPolynomial &p, const int sign) {
        if (was_x && !power) {
            power = 1;
        }
        if (power >= p.coe_table.size()) {
            p.coe_table.resize(power + 1);
        }
        if (was_x && digit == 0.0) {
            digit = 1;
        }
        p.coe_table[power] += sign * digit;
    }

public:
    CPolynomial() {};

    CPolynomial(vector<float> vec) {};

    friend istream &operator>>(istream &in, CPolynomial &coe_table);

    friend ostream &operator<<(ostream &out, const CPolynomial &p);

    friend CPolynomial operator*(const CPolynomial &ths, const CPolynomial &rhs) {
        CPolynomial new_p;
        new_p.coe_table.resize(ths.coe_table.size() + rhs.coe_table.size() + 1);
        for (int i = 0; i < ths.coe_table.size(); i++)
            for (int j = 0; j < rhs.coe_table.size(); j++)
                new_p.coe_table[i + j] += ths.coe_table[i] * rhs.coe_table[j];

        return new_p;
    }

    CPolynomial &operator*=(const CPolynomial &other) {
        *this = *this * other;
        return *this;
    }

    friend CPolynomial operator*(const CPolynomial &p1, double digit) {
        CPolynomial new_p;
        new_p.coe_table.resize(p1.coe_table.size());
        for (int i = 0; i < p1.coe_table.size(); i++) {
            new_p.coe_table[i] = digit * p1.coe_table[i];
        }
        return new_p;
    }

    CPolynomial &operator*=(double digit) {
        *this = *this * digit;
        return *this;
    }

    CPolynomial &operator=(const CPolynomial &other) {
        if (&other == this)
            return *this;
        coe_table = other.coe_table;
        return *this;
    }

    CPolynomial &operator/=(float digit) {
        for (int i = 0; i < this->coe_table.size(); i++)
            this->coe_table[i] /= digit;
        return *this;
    }

    friend CPolynomial operator+(const CPolynomial &ths, const CPolynomial &rhs) {
        CPolynomial new_p;
        new_p = ths;
        new_p += rhs;

        return new_p;
    }

    CPolynomial &operator+=(const CPolynomial &other_p) {

        if (this->coe_table.size() >= other_p.coe_table.size())
            for (int i = 0; i < other_p.coe_table.size(); i++)
                this->coe_table[i] += other_p.coe_table[i];
        else {
            for (int i = 0; i < this->coe_table.size(); i++)
                this->coe_table[i] += other_p.coe_table[i];

            for (int i = this->coe_table.size(); i < other_p.coe_table.size(); i++)
                this->coe_table.push_back(other_p.coe_table[i]);
        }

        return *this;
    }

    friend CPolynomial operator-(const CPolynomial &ths, const CPolynomial &rhs) {
        CPolynomial new_p;
        new_p = ths + rhs * (-1);
        return new_p;
    }

    CPolynomial &operator-=(const CPolynomial &other) {
        *this = *this - other;
        return *this;
    }

    CPolynomial operator-() const {
        CPolynomial p;
        p = *this * (-1);
        return p;
    }

    friend bool operator==(const CPolynomial &ths, const CPolynomial &rhs) {
        if (ths.coe_table.size() != rhs.coe_table.size()) {
            return false;
        } else {
            for (int i = 0; i < ths.coe_table.size(); i++) {
                if (ths.coe_table[i] != rhs.coe_table[i]) {
                    return false;
                }
            }
        }
        return true;
    }

    friend bool operator!=(const CPolynomial &ths, const CPolynomial &rhs) {
        return !(ths == rhs);
    }

    float operator[](const int i) const {
        return this->coe_table[i - 1];
    }

    ~CPolynomial() {};
};

ostream &operator<<(ostream &out, const CPolynomial &p) {
    if (!p.coe_table.empty()) {
        for (int i = 0; i < p.coe_table.size() - 1; i++) {
            if (p.coe_table[i] != 0) {
                if (i == 0)
                    out << p.coe_table[i] << " + ";
                else if (i == 1)
                    out << p.coe_table[i] << "x + ";
                else if (p.coe_table[i] == 1)
                    out << "x^" << i << " + ";
                else if (p.coe_table[i + 1] >= 0)
                    out << p.coe_table[i] << "x^" << i << " + ";
                else
                    out << p.coe_table[i] << "x^" << i << " ";
            }
        }
        if (p.coe_table[p.coe_table.size() - 1] == 1)
            out << "x^" << p.coe_table.size() - 1 << "\n";
        else
            out << p.coe_table[p.coe_table.size() - 1] << "x^" << p.coe_table.size() - 1 << "\n";
    } else
        out << 0 << "\n";
    return out;
};

istream &operator>>(istream &in, CPolynomial &p) {
    vector<float> index;

    string str;
    getline(in, str);
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    unsigned power = 0;
    double digit = 0;
    int sign = 1;
    bool is_coef = false, was_x = false;
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == '+' || str[i] == '-') {
            is_coef = false;
            if (i != 0)
                CPolynomial::set_coefficients(was_x, power, digit, p, sign);

            digit = 0, power = 0;
            if (str[i] == '-') {
                sign = -1;
            } else {
                sign = 1;
            }
            was_x = false;
        } else if (str[i] == '^') {
            is_coef = true;
            power = 0;
        } else if (is_coef) {
            power = power * 10 + (str[i] - '0');
        } else {
            if (str[i] == 'x') {
                was_x = true;
            } else {
                digit = digit * 10 + (str[i] - '0');
            }
        }
    }
    CPolynomial::set_coefficients(was_x, power, digit, p, sign);

    return in;
};


int main() {
    CPolynomial l, k, sum, dif;
    cin >> l;
    cin >> k;
    //l *= 3;
    sum = k + l;
    cout << sum;
    dif = l - k;
    cout << dif;
    return 0;
}
