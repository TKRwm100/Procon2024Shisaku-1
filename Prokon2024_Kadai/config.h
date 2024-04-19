#pragma once
#include <array>

class Config
{
	static bool inited;
public:
	static void init();
	static const int fps = 60;
	static const double diving;
	static const int maxDiving = 2445;
	static std::array<int,256> keyConfig;
};