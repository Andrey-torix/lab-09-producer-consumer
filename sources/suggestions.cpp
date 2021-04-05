// Copyright 2020 Andreytorix
#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "single_include\nlohmann\json.hpp"
#include "suggestions.hpp"
using namespace nlohmann;

std::vector<suggestion> getFromJson(std::string path) {
  std::vector<suggestion> v;
  std::ifstream i(path);
  json j;
  i >> j;

  for (json::iterator it = j.begin(); it != j.end(); ++it) {
    json sug = it.value();
    suggestion temp;
    temp.id = sug["id"];
    temp.name = sug["name"];
    temp.cost = static_cast<int>(sug["cost"]);
    v.push_back(temp);
  }

  std::sort(v.begin(), v.end(), suggestion::compare);
  return v;
}

bool search(std::string item, std::string searchText) {
  return (boost::contains(item, searchText));
}
