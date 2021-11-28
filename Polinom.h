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
            data[0].key = 0;
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

    Polynomial* clone() {
        auto * temp = new Polynomial();
        for (int i = 0; i < Length(); ++i) {
            temp->push_back(data[i].value, data[i].key);
        }
        return temp;
    }

    [[maybe_unused]] Polynomial(const int num, decimal value) : Polynomial() {
        for (int i = 0; i < num; ++i) {
            Polynomial::push_back(value);
        }
    }

    inline bool isEmpty() const { return data == nullptr; }

    void push_back(const decimal other, const int KEY = -1) override{
        resizeOnce();
        data[number - 1].value = other;
        if (KEY != -1) {
            if (!isExistKey(KEY))
            {
                data[number - 1].key = KEY;
            }
        }
    }

    int MaxKey() {
        if (data == nullptr) return 0;
        int res = data[0].key;
        for (int i = 0; i < number; ++i) {
            if (res < data[i].key) {
                res = data[i].key;
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
            auto *newArr = new DATA[new_size];
            for (int i = 0; i < new_size; ++i) {
                newArr[i] = data[i];
            }
            delete[] data;
            data = newArr;
            number = new_size;
        }
    }

    [[nodiscard]] int Length() const override{
        return number;
    }

    [[nodiscard]] inline int GetRealIndex(const int index) const{
        return (index >= 0) ? index : number + index;
    }

    decimal operator[](const int index) const override{
        return data[GetRealIndex(index)].value;;
    }

    [[nodiscard]] int getKey(const int index) const override {
        return data[GetRealIndex(index)].key;
    }

    ~Polynomial() override{
        delete[] data;
    }

    friend std::ostream& operator<<(std::ostream& out, const Polynomial<decimal> & other) {
        if (other.data == nullptr) {
            out << "nullptr";
        }
        else {
            for (int i = 0; i < other.Length() - 1; ++i) {
                out << other[i] << "X^" << other.getKey(i) << " + ";
            }
            out << other[-1] << "X^" << other.getKey(-1);
        }
        return out;
    }

    template<typename Type = double>
    double operator()(Type const x) const{
        double result = 0;
        double y = x;
        for (int i = 0; i < number; ++i) {
            result += y * data[i].value;
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
        }
        resize(number - 1);
        if (number == 0) {
            data = nullptr;
        }
    }

    void popLast() {
        resize(number - 1);
        if (number == 0) {
            data = nullptr;
        }
    }

    template<int index1, int index2>
    Polynomial slice() {
        int real_index1 = GetRealIndex(index1);
        int real_index2 = GetRealIndex(index2);
        Polynomial newPolynomial = Polynomial();
        if (real_index1 < real_index2) {
            for (int i = real_index1; i <= real_index2; ++i) {
                newPolynomial.push_back(data[i].value);
                newPolynomial.data[i - real_index1].key = data[i].key;
            }
        }
        else {
            for (int i = real_index1, j = 0; i >= real_index2; --i, j++) {
                newPolynomial.push_back(data[i].value);
                newPolynomial.data[j].key = data[i].key;
            }
        }
        return newPolynomial;
    }

    int getDegree() { return MaxKey(); }

    int getRealDegree() { return Length(); }

    template<typename Type>
    Polynomial operator+(Type value) {
        Polynomial newPol = Polynomial();
        for (int i = 0; i < number; ++i) {
            newPol.push_back(data[i].value + value);
        }
        return newPol;
    }

    template<typename Type>
    Polynomial operator*(Type value) {
        Polynomial newPol = Polynomial();
        for (int i = 0; i < number; ++i) {
            newPol.push_back(data[i].value * value);
        }
        return newPol;
    }

    template<typename Type>
    Polynomial operator-(Type value) {
        Polynomial newPol = Polynomial();
        for (int i = 0; i < number; ++i) {
            newPol.push_back(data[i].value - value);
        }
        return newPol;
    }

    template<typename Type>
    Polynomial operator/(Type value) {
        Polynomial newPol = Polynomial();
        for (int i = 0; i < number; ++i) {
            newPol.push_back(data[i].value / value);
        }
        return newPol;
    }

    bool isExistKey(int degree) {
        for (int i = 0; i < number; ++i) {
            if (data[i].key == degree) {
                return true;
            }
        }
        return false;
    }


    int getIndexByDegree(int degree) {
        for (int i = 0; i < number; ++i) {
            if (data[i].key == degree) {
                return i;
            }
        }
        return 0;
    }

    decimal getValueByKey(int degree) {
        if (isExistKey(degree)) {
            return data[getIndexByDegree(degree)].value;
        }
        return 0;
    }

    friend Polynomial operator+(Polynomial& polynomial1, Polynomial& polynomial2) {
        Polynomial result = Polynomial();
        int maxKey = (polynomial1.MaxKey() > polynomial2.MaxKey()) ? polynomial1.MaxKey() : polynomial2.MaxKey();
        for (int i = 0; i <= maxKey; ++i) {
            decimal tmp1 = polynomial1.getValueByKey(i);
            decimal tmp2 = polynomial2.getValueByKey(i);
            result.push_back(tmp1 + tmp2);
        }
        return result;
    }

    Polynomial& operator=(const Polynomial& other) {
        delete[] data;
        data = nullptr;
        number = 0;
        for (int i = 0; i < other.Length(); ++i) {
            this->push_back(other[i], other.getKey(i));
        }
        return *this;
    }

    Polynomial& operator+=(Polynomial &other) {
        Polynomial<decimal> temp = *this + other;
        *this = temp;
        return *this;
    }


    friend Polynomial operator*( Polynomial& polynomial1, Polynomial& polynomial2) {
        Polynomial result = Polynomial();
        for (int i = 0; i < polynomial1.Length(); ++i) {
            for (int j = 0; j < polynomial2.Length(); ++j) {
                Polynomial temp = Polynomial();
                temp.push_back(
                        polynomial1[i] * polynomial2[j],
                        polynomial1.getKey(i) + polynomial2.getKey(j)
                        );
                result += temp;
            }
        }
        return result;
    }
};

//template <class T1, class T2>
//Polynomial<T1> operator/(Polynomial<T1> pol1, Polynomial<T2> pol2) {
//    Polynomial<double> temp =
//    while(pol1.Length() > pol2.Length()) {
//        pol1 = diff(pol1, pol2);
//    }
//    return pol1;
//}
//
//template <class T1, class T2>
//Polynomial<T1> diff(Polynomial<T1> pol1, Polynomial<T2> pol2) {
//    while(!pol2.isEmpty()) {
//
//    }
//}


#endif //LBAB3_POLINOM_H
