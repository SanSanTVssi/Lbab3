//
// Created by AAI2002 on 19.11.2021.
//

#ifndef LAB22_INTRERFACES_H
#define LAB22_INTRERFACES_H

#include <iostream>

template <typename Type, typename Type2>
class IDictionary {
public:
    virtual void push_back(Type, Type2) = 0;
    virtual void resize(int) = 0;
    [[nodiscard]] virtual int Length() const = 0;
    virtual Type operator[](Type2) const = 0;
    virtual void pop(Type2) = 0;
    virtual int getKey(const int index) const = 0;
    [[nodiscard]] virtual bool isEmpty() const = 0;
    virtual ~IDictionary() = 0;
};

template <typename Type, typename Type2>
IDictionary<Type, Type2>::~IDictionary<Type, Type2>() = default;

#endif //LAB22_INTRERFACES_H
