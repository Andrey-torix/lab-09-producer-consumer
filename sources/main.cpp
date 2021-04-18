// Copyright 2020 Andreytorix
#include <iostream>
#include "main.hpp"
#include <boost/program_options.hpp>
int main(int argc, const char* argv[])
{
    setlocale(LC_ALL, "Russian");
    EApplication app(argc, argv);
    return app.exec();
}
