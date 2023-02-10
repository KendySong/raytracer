#include "Core/Application.hpp"

int main(int argc, char* args[])
{
    srand(time(nullptr));
    return Application::instance()->run();
}