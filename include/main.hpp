// Copyright 2020 Andreytorix
#pragma once
#include <boost/program_options.hpp>
#include <string>
class EApplication {
public:
  explicit EApplication(int argc, const char **argv);
  int exec();

private:
  boost::program_options::options_description m_desk{"доступные опции"};
  boost::program_options::variables_map m_vm;
  /*
  --url
  ---depth
--network_threads
--parser_threads
--output
*/
  std::string url;     //адрес сайта для парсинга
  int depth;           //глубина поиска
  int network_threads; //количество поток скачивания
  int parser_threads;  //количество потоков парсера
  std::string path;    //путь к файлу с результатом
};
EApplication::EApplication(int argc, const char **argv);
int EApplication::exec();
