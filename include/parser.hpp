// Copyright 2020 Andreytorix
#pragma once
#include <filesystem>
#include <gumbo.h>
#include <iostream>
#include <vector>
#include "pageContainer.hpp"
#include <condition_variable>
#include <fstream>
#include <boost/algorithm/string.hpp>
std::condition_variable downloadFunction_cv, parseFunction_cv;
std::vector<std::string> hreflist_start;

//функция записи в файл
void saveHrefToFile(std::string href, int depth, std::string path) {
    std::ofstream out;
    out.open(path, std::ios_base::app);
    if (out.is_open()) {
        out << href << " " << depth << std::endl;
    }
    out.close();
}

//функция поиска ссылок и добавления их в вектор
void search_for_links(GumboNode* node, int depthbody, std::string path,
    std::string locHref) {
    if (node->type != GUMBO_NODE_ELEMENT) {
        return;
    }
    GumboAttribute* href;
    if (node->v.element.tag == GUMBO_TAG_A &&
        (href = gumbo_get_attribute(&node->v.element.attributes, "href"))) {
        if (std::filesystem::path(href->value).extension() == ".jpg" ||
            std::filesystem::path(href->value).extension() == ".png") {
            std::cout << href->value << " " << depthbody << std::endl;
            saveHrefToFile(href->value, depthbody + 1, path);
        }
        else {
            size_t found = locHref.find_first_of(":");
            std::string protocol = locHref.substr(0, found - 1);  //протокол
            if (protocol == "") {
                protocol = "http";
            }
            std::string url_new = locHref.substr(found + 3);
            size_t found1 = url_new.find_first_of(":");
            auto const host = url_new.substr(0, found1);  //адрес хоста
            size_t found2 = url_new.find_first_of("/");
            auto port = url_new.substr(found1 + 1, found2 - found1 - 1);  //порт
            if (port == "") {
                port = "80";
            }
            std::string urll = href->value;  //хранение найденой ссылки
            if (urll.size() < 5) {
                locHref.pop_back();
                urll = locHref + "" + urll;
            }
            else {
                size_t foundl = urll.find_first_of(":");
                std::string protocoll = urll.substr(0, foundl);  //протокол
                if (protocoll == "http" || protocoll == "https") {
                }
                else {
                    if (urll.find(host) != std::string::npos) {
                    }
                    else {
                        locHref.pop_back();
                        urll = locHref + "" + urll;
                    }
                }
            }
            hrefGlobalList.push_back(hrefItem(urll, depthbody));
            downloadFunction_cv
                .notify_all();  //если нашли ссылку оповещаем все потоки скачивания
        }
    }
    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
        search_for_links(static_cast<GumboNode*>(children->data[i]), depthbody,
            path, locHref);
        if (i == children->length) {
        }
    }
}

//функция инициализации списка ссылок (уровень страницы 1)
void initialize_search_for_links(GumboNode* node, std::string path,
    std::string url) {
    if (node->type != GUMBO_NODE_ELEMENT) {
        return;
    }
    GumboAttribute* href;
    if (node->v.element.tag == GUMBO_TAG_A &&
        (href = gumbo_get_attribute(&node->v.element.attributes, "href"))) {
        if (std::filesystem::path(href->value).extension() == ".jpg" ||
            std::filesystem::path(href->value).extension() == ".png") {
            std::cout << href->value << " 1" << std::endl;
            saveHrefToFile(href->value, 1, path);
        }
        else {
            //разбираем ссылку - аргумент
            size_t found = url.find_first_of(":");
            std::string protocol = url.substr(0, found - 1);  //протокол
            if (protocol == "") {
                protocol = "http";
            }
            std::string url_new = url.substr(found + 3);
            size_t found1 = url_new.find_first_of(":");
            auto const host = url_new.substr(0, found1);  //адрес хоста
            size_t found2 = url_new.find_first_of("/");
            auto port = url_new.substr(found1 + 1, found2 - found1 - 1);  //порт
            if (port == "") {
                port = "80";
            }
            std::string urll = href->value;  //хранение найденой ссылки
            if (urll.size() < 5) {
                url.pop_back();
                urll = url + "" + urll;
            }
            else {
                size_t foundl = urll.find_first_of(":");
                std::string protocoll = urll.substr(0, foundl);  //протокол
                if (protocoll == "http" || protocoll == "https") {
                }
                else {
                    if (urll.find(host) != std::string::npos) {
                    }
                    else {
                        url.pop_back();
                        urll = url + "" + urll;
                    }
                }
            }
            hrefGlobalList.push_back(hrefItem(urll, 1));
        }
    }
    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
        initialize_search_for_links(static_cast<GumboNode*>(children->data[i]),
            path, url);
    }
}
