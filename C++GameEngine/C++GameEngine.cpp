#include <iostream>
#include <memory>
#include <fstream>
#include "Vector2.hpp"

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

#include "TestScreen.h"
#include "Game.h"


int main()
{
	//TestScreen test;
	//test.RunTest();
	Game game("path");
	game.run();
	return 0;
}