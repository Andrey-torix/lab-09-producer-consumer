// Copyright 2020 Andreytorix
#pragma once
#include <iostream>
#include <string>
#include <condition_variable>
#include <vector>
#include <thread>
//хранения списка ссылок
struct hrefItem {
    std::string hrefString;
    int depthItem;
    hrefItem(std::string h, int d) {
        hrefString = h;
        depthItem = d;
    }
};
std::vector<hrefItem> hrefGlobalList;
//хранение списка скаченных страниц
struct pageT {
    std::string bodySite;
    int depthPage;
    std::string pageHref;
    pageT(std::string b, int d, std::string h) {
        bodySite = b;
        depthPage = d;
        pageHref = h;
    }
};
std::vector<pageT> pages;
