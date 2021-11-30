#include <iostream>
#include "Polinom.h"
#include <windows.h>
#include <signal.h>


int memento()
{
    int a=0;
    MessageBoxA(NULL,"Memento mori","POSIX Signal",NULL);
    return 0;
}

void posix_death_signal(int signum)
{
    memento();
    signal(signum, SIG_DFL);
    exit(3);
}

int main() {
    signal(SIGSEGV, posix_death_signal);
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
    instance.pop(2);
    cout << instance << endl;
    cout << instance[-1] << endl;
    cout << instance[-2] << endl;

    Polynomial<int> instance2 = Polynomial<int>(instance);
    instance2.resize(3);
    cout << instance2 << endl;
    cout << instance2(4) << endl;
    Polynomial<float> instance3 = instance2.convert<float>();
    std::cout << instance3.type() << std::endl;
    Polynomial<double> instance4 = instance.slice<-1, -3>();
    std::cout << instance4/2 << std::endl;
    Polynomial pol = instance + instance4;
    std::cout << "(" << instance << ")"  << "+" << "(" << instance4 << ")\n= " << pol << std::endl;
    instance.resize(2);
    Polynomial pol2 = instance * instance4;
    std::cout << "(" << instance << ")"  << "*" << "(" << instance4 << ")\n= " << pol2 << std::endl;

    cout << "========================" << endl;
    std::cout << "(" << instance4 << ")"  << "/" << "(" << instance << ") = " << endl;
    Polynomial pol3 = instance4 / instance;
    cout <<  pol3 << std::endl;

    int x;
    cin >> x;
}
