#pragma once
struct suggestion {
	std::string id;
	std::string name;
	int cost;
	static bool compare(suggestion &sug1, suggestion & sug2){
		return(sug1.cost>sug2.cost);
	}
};
std::vector<suggestion> getFromJson(std::string path);
bool search(std::string item, std::string searchText);
