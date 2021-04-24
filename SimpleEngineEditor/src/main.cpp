#include <iostream>

#include <SimpleEngineCore/Utils/test.hpp>

int main()
{
    std::cout << "Hello from Simple Engine Editor" << std::endl;

    SimpleEngine::checkGLFW();

    std::cin.get();
}