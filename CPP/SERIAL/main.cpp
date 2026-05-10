#include <iostream>
#include "motorSerial.hpp"

int main()
{
    auto portas = MotorSerial::ListarPortas();
    std::cout << "Portas: " << portas.size() << std::endl;
    for(const auto &p : portas) std::cout << p << std::endl;
    return 0;
}
