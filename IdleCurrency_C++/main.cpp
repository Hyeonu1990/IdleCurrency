//
//  main.cpp
//  testproject
//
//  Created by hyeonu choi on 2020/12/05.
//

#include <iostream>
#include "IdleCurrency.hpp"

int main(int argc, const char * argv[]) {
    Idle::IdleCurrency gold_curreny(Idle::BWFType::Idle_Gold);
    std::cout << "gold_currecy 생성" << std::endl;
    std::cout << "gold_currency : " << gold_curreny.ToString() << std::endl;
    
    gold_curreny += 100;
    std::cout << "gold_currecy + 100" << std::endl;
    std::cout << "gold_currency : " << gold_curreny.ToString() << std::endl;
    
    gold_curreny *= 100;
    std::cout << "gold_currecy * 100" << std::endl;
    std::cout << "gold_currency : " << gold_curreny.ToString() << std::endl;
    
    gold_curreny += Idle::IdleCurrency(123, "d", Idle::BWFType::Idle_Gold);
    std::cout << "gold_currecy + 123d" << std::endl;
    std::cout << "gold_currency : " << gold_curreny.ToString() << std::endl;
    
    for(int i = 0; i <= 10; i++)
    {
        gold_curreny += Idle::IdleCurrency(123, "z", Idle::BWFType::Idle_Gold);
        std::cout << "gold_currecy + 123z" << std::endl;
        std::cout << "gold_currency : " << gold_curreny.ToString() << std::endl;
    }
    return 0;
}
