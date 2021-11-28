#include <iostream>
#include "Polinom.h"

int main() {
    using namespace std;
    Polynomial<double> instance = Polynomial<double>(1);
    instance.push_back(2);
    instance.push_back(3.5);
    instance.push_back(4);
    instance.push_back(5);
    instance.push_back(1);
    instance.push_back(1);
    instance.push_back(2);
    instance.push_back(3);
    instance.push_back(4.5);
    instance.push_back(5);
    instance.pop(1);
    cout << instance << endl;
    cout << instance[-1] << endl;
    cout << instance[-2] << endl;
    auto inc = instance + instance.convert<float>();
    cout << inc << endl;

//    Polynomial<int> instance2 = Polynomial<int>(instance);
//    instance2.resize(3);
//    cout << instance2 << endl;
//    cout << instance2(4) << endl;
//    Polynomial<float> instance3 = instance2.convert<float>();
//    std::cout << instance3.type() << std::endl;
//    Polynomial<double> instance4 = instance.slice<-1, -3>();
//    std::cout << instance4/2 << std::endl;
}
