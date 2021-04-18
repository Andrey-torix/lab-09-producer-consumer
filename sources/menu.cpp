// Copyright 2020 Andreytorix
#include "lab9.hpp"
#include <iostream>
#include <mutex>
#include "Downloader.hpp"
#include "Parser.hpp"
#include "pageContainer.hpp"
std::mutex m;
bool work = true;
void downloadFunction(int id) {
    while (work) {
        std::unique_lock<std::mutex> lk(m);
        if (hrefGlobalList.size() > 0) {  //если в векторе ссылок есть элементы
                                          //скачиваем страницы
          // std::cout << "" << std::endl;
          // std::cout << "начинаю скачку" << std::endl;
            std::string localHref = hrefGlobalList.back().hrefString;
            // std::cout << "ссылка: "<< localHref << std::endl;
            std::int16_t localDepth = hrefGlobalList.back().depthItem;
            hrefGlobalList.pop_back();  //удаляем эту строку из списка
            std::string contents =
                Network(localHref);  //берём ссылку из списка и качаем страницу
            if (contents != "ERROR") {
                pages.push_back(pageT(contents, localDepth,
                    localHref));  //полученную страницу вставляем в
                                  //вектор (очередь на парсинг) вторым
                                  //аргументом передаём глубину
parseFunction_cv.notify_all();  //оповещаем все потоки парсеров, что мы
                                                //скачали новую страницу
            }
        } else {
            downloadFunction_cv.wait(
lk, [] { return hrefGlobalList.size() != 0; });  //иначе простаиваем
        }
        lk.unlock();
        std::this_thread::sleep_for(
std::chrono::milliseconds(100));  // задём время простоя 
    }
}
void parseFunction(int id, int depthParse,
    std::string path) {  //парсим контент страницы
    while (work) {
        if (hrefGlobalList.size() == 0 && pages.size() == 0) {
            work = false;
            exit(0);
        }
        std::unique_lock<std::mutex> lk(m);
        if (!pages.empty()) {
            std::string sityBody = pages.back().bodySite;  //контент страницы
            int depthBody = pages.back().depthPage;        //глубина
            std::string locHref = pages.back().pageHref;  //адрес страницы
            if (depthBody <= depthParse) {
                pages.pop_back();
                GumboOutput* output = gumbo_parse(sityBody.c_str());
                search_for_links(output->root, (depthBody + 1), path,
                    locHref);  // список ссылок
                gumbo_destroy_output(&kGumboDefaultOptions, output);
            } else {
                if (hrefGlobalList.size() == 0) {
                    work = false;
                    std::cout << "обход завершён" << std::endl;
                } else {
                }
            }
        } else {
            parseFunction_cv.wait(lk, [] { return pages.size() > 0; });
        }
        lk.unlock();
        std::this_thread::sleep_for(
std::chrono::milliseconds(120));  //задём время простоя (миллисекунд)
    }
}

EApplication::EApplication(int argc, const char** argv) {
    m_desk.add_options()("help", "вызов справки")(
        "url", boost::program_options::value<std::string>(&url)->composing(),
        "url адрес страницы для парсинга")(
"output", boost::program_options::value<std::string>(&path)->composing(),
            "путь к файлу с результатом")(
"depth", boost::program_options::value<int>(&depth)->composing(),
                "глубина поиска")(
                    "network_threads",
boost::program_options::value<int>(&network_threads)->composing(),
                    "количество поток скачивания")(
                        "parser_threads",
boost::program_options::value<int>(&parser_threads)->composing(),
                        "количество потоков парсера");
    boost::program_options::store(
        boost::program_options::parse_command_line(argc, argv, m_desk), m_vm);
    boost::program_options::notify(m_vm);
}

int EApplication::exec() {
    if (m_vm.count("help")) {
        std::cout << m_desk << std::endl;
        return 1;
    }
    if (m_vm.count("url") && m_vm.count("output") && m_vm.count("depth") &&
        m_vm.count("network_threads") && m_vm.count("parser_threads")) {
        //инициализируем список ссылок
        std::string contents = Network(url);
        GumboOutput* output = gumbo_parse(contents.c_str());
        initialize_search_for_links(output->root, path,
            url);  //стартовый список ссылок
        gumbo_destroy_output(&kGumboDefaultOptions, output);
        //конец инициализации
        if (depth != 1) {
            //запуск потоков
            if (hrefGlobalList.size() != 0) {
                std::vector<std::thread> downloaders;
                std::vector<std::thread> parsers;
                for (int i = 0; i < network_threads; i++) {
                    downloaders.push_back(std::thread(downloadFunction, i));
                }
                for (int i = 0; i < parser_threads; i++) {
                    parsers.push_back(std::thread(parseFunction,
                                                  i, depth, path));
                }
                for (int i = 0; i < network_threads; i++) {
                    downloaders[i].join();
                }
                for (int i = 0; i < parser_threads; i++) {
                    parsers[i].join();
                }
            }
        }
    } else {
        std::cout << "Пожалуйста, используйте --help опцию для вывода справки"
            << std::endl;
        return 1;
    }
    return 0;
}
