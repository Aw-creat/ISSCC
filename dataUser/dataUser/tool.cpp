#include <iostream>
#include <vector>
#include <string>
#include <fstream>

void inputAndSaveKeywords(std::vector<std::string>& keywords_list, int& k) {
	std::string keywords;
	std::cout << "Enter keywords separated by spaces: ";
	std::getline(std::cin, keywords);

	std::cout << "Enter the number of results to retrieve: ";
	std::cin >> k;
	std::cin.ignore(); // Clear the input buffer

	size_t pos = 0;
	while ((pos = keywords.find(' ')) != std::string::npos) {
		keywords_list.push_back(keywords.substr(0, pos));
		keywords.erase(0, pos + 1);
	}
	if (!keywords.empty()) {
		keywords_list.push_back(keywords);
	}

	// Saving the data to a file
	std::ofstream outFile("keywords_data.txt");
	for (const auto& word : keywords_list) {
		outFile << word << "\n";
	}
	outFile << k << "\n"; // Save k at the end of the file
	outFile.close();
}

void loadKeywords(std::vector<std::string>& keywords_list, int& k) {
	std::ifstream inFile("keywords_data.txt");
	std::string line;
	keywords_list.clear();

	while (std::getline(inFile, line)) {
		keywords_list.push_back(line);
	}

	// The last line is k, so extract it and remove from list
	if (!keywords_list.empty()) {
		k = std::stoi(keywords_list.back());
		keywords_list.pop_back();
	}
}