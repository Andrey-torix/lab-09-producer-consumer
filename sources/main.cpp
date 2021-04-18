// Copyright 2020 Andreytorix
#include <iostream>
#include "../include/menu.hpp"
int main(int argc, const char* argv[])
{
    EApplication obj;
    obj.EApplication(argc, argv);
    //setlocale(LC_ALL, "Russian");
    //EApplication::EApplication app(argc, argv);
    return app.exec();
}
