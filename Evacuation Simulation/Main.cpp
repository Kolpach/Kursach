#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include "mapObject.h"
#include "levelDisigner.h"

namespace cm {
	enum commands
	{
		start,
		levelDisigner,
	};
}
int main() {
	cm::commands command = cm::start;
	std::map<std::string, cm::commands> getEnum = { {"start", cm::start}, {"editor", cm::levelDisigner}, {"1", cm::start}, {"2", cm::levelDisigner} };
	std::string input;
	while (input != "finish" && input != "3") {

		std::cout << "Choose what to do: 1-start, 2-editor, 3-finish" << std::endl;
		std::cin >> input;
		if (input == "3" || input == "finish")
			continue;
		if (getEnum.find(input) == getEnum.end()) {

			system("cls");
			std::cout << "Wrong command" << std::endl;
			continue;
		}
		command = getEnum[input];
		switch (command)
		{
		case cm::start: {
			break;
		}
		case cm::levelDisigner: {
			levelDisigner disigner;
			disigner.start();
			break;
		}
		}

		system("cls");
	}
	return 0;
}