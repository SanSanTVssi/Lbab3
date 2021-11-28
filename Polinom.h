//
// Created by AAI2002 on 27.11.2021.
//

#ifndef LBAB3_POLINOM_H
#define LBAB3_POLINOM_H

#include "intrerfaces.h"



template<class decimal = double>
class Polynomial : public IDictionary<decimal, int>{
private:
    int number;
    struct DATA{
        int key;
        decimal value;
    } * data;

    void resizeOnce() {
        if (data == nullptr) {
            data = new DATA[++number];
            data[0].key = 1;
        } else {
            int k = MaxKey();
            data = (DATA *) realloc(data, ++number * sizeof(DATA));
            data[number - 1].key = k + 1;
        }
    }

public:
    Polynomial() : number(0), data(nullptr) {}

    explicit Polynomial(const decimal value) : Polynomial() {
        Polynomial::push_back(value);
    }

    template<typename otherType>
    explicit Polynomial(const Polynomial<otherType>& other) : Polynomial() {
        for (int i = 0; i < other.Length(); ++i) {
            Polynomial::push_back(static_cast<decimal>(other[i]));
        }
    }

    Polynomial(const Polynomial&) = default;

//    Polynomial(decimal value, ...) : Polynomial() {
//        decimal * p = &value;
//        int size = Size;
//        while (size--) {
//            Polynomial::push_back(*p);
//            p++;
//        }
//    }

    [[maybe_unused]] Polynomial(const int num, decimal value) : Polynomial() {
        for (int i = 0; i < num; ++i) {
            Polynomial::push_back(value);
        }
    }

    void push_back(const decimal other, const int KEY = -1) override{
        resizeOnce();
        data[number - 1] = other;
        if (KEY != -1) {
            if (!isExistKey(KEY))
            {
                key[number - 1] = KEY;
            }
        }
    }

    int MaxKey() {
        int res = key[0];
        for (int i = 0; i < number; ++i) {
            if (res < key[i]) {
                res = key[i];
            }
        }
        return res;
    }

    void resize(const int new_size) override{
        if (data == nullptr) {
            for (int i = 0; i < new_size; ++i) {
                push_back(0);
            }
        }
        if (new_size > number) {
            for (int i = number; i < new_size; ++i) {
                push_back(0.0);
            }
        } else if (new_size < number) {
            auto *newArr = new decimal[new_size];
            int *newkeys = new int[new_size];
            for (int i = 0; i < new_size; ++i) {
                newArr[i] = data[i];
                newkeys[i] = key[i];
            }
            delete[] data;
            delete[] key;
            data = newArr;
            key = newkeys;
            number = new_size;
        }
    }

    [[nodiscard]] int Length() const override{
        return number;
    }

    [[nodiscard]] int GetRealIndex(const int index) const{
        return (index >= 0) ? index : number + index;
    }

    decimal operator[](const int index) const override{
        return data[GetRealIndex(index)];
    }

    [[nodiscard]] int getKey(const int index) const override {
        return key[GetRealIndex(index)];
    }

    ~Polynomial() override{
        delete[] data;
        delete[] key;
    }

    friend std::ostream& operator<<(std::ostream& out, const Polynomial<decimal> & other) {
        for (int i = 0; i < other.Length() - 1; ++i) {
            out << other[i] << "X^" << other.getKey(i) << " + ";
        }
        out << other[-1] << "X^" << other.getKey(-1);
        return out;
    }

    template<typename Type = double>
    double operator()(Type const x) const{
        double result = 0;
        double y = x;
        for (int i = 0; i < number; ++i) {
            result += y * data[i];
            y *= x;
        }
        return result;
    }

    template<class OtherType>
    Polynomial<OtherType> convert() {
        return Polynomial<OtherType>(*this);
    }

    [[nodiscard]] std::string type() const {
        return typeid(decimal).name();
    }

    void pop(const int index) override{
        for (long i = index; i < number; ++i) {
            data[i] = data[i + 1];
            key[i] = key[i + 1];
        }
        resize(number - 1);
    }

    template<int index1, int index2>
    Polynomial slice() {
        int real_index1 = GetRealIndex(index1);
        int real_index2 = GetRealIndex(index2);
        Polynomial newPolinomial = Polynomial();
        if (real_index1 < real_index2) {
            for (int i = real_index1; i <= real_index2; ++i) {
                newPolinomial.push_back(data[i]);
                newPolinomial.key[i - real_index1] = key[i];
            }
        }
        else {
            for (int i = real_index1, j = 0; i >= real_index2; --i, j++) {
                newPolinomial.push_back(data[i]);
                newPolinomial.key[j] = key[i];
            }
        }
        return newPolinomial;
    }

    int getDegree() { return MaxKey(); }

    int getRealDegree() { return Length(); }

    template<typename Type>
    Polynomial operator+(Type value) {
        Polynomial newPol = Polynomial();
        for (int i = 0; i < number; ++i) {
            newPol.push_back(data[i] + value);
        }
        return newPol;
    }

    template<typename Type>
    Polynomial operator*(Type value) {
        Polynomial newPol = Polynomial();
        for (int i = 0; i < number; ++i) {
            newPol.push_back(data[i] * value);
        }
        return newPol;
    }

    template<typename Type>
    Polynomial operator-(Type value) {
        Polynomial newPol = Polynomial();
        for (int i = 0; i < number; ++i) {
            newPol.push_back(data[i] - value);
        }
        return newPol;
    }

    template<typename Type>
    Polynomial operator/(Type value) {
        Polynomial newPol = Polynomial();
        for (int i = 0; i < number; ++i) {
            newPol.push_back(data[i] / value);
        }
        return newPol;
    }

    bool isExistKey(int degree) {
        for (int i = 0; i < number; ++i) {
            if (key[i] == degree) {
                return true;
            }
        }
        return false;
    }


    int getIndexByDegree(int degree) {
        for (int i = 0; i < number; ++i) {
            if (key[i] == degree) {
                return i;
            }
        }
        return 0;
    }

    decimal getValueByDegree(int degree) {
        if (isExistKey(degree)) {
            return data[getIndexByDegree(degree)];
        }
        return 0;
    }
};


#endif //LBAB3_POLINOM_H
