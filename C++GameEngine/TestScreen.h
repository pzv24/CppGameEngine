#pragma once
#include <iostream>
#include <memory>
#include <fstream>
#include "Vector2.hpp"

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

class TestScreen
{
public:
	TestScreen() = default;

	void LoadFromFile(const std::string& filepath);
	void RunTest();
};

