#pragma once
#include "Singleton.h"
#include <map>
#include <vector>

class Touch:public Singleton<Touch>
{
	friend Singleton<Touch>;
	class info
	{
	public:
		info();
		int x;
		int y;
		int device;
		bool flag;

		int pressing;
		int releasing;
	};
	std::map<int, info> infos;
	std::vector<int>ids;
public:
	int getID(size_t index);
	int getX(int id);
	int getY(int id);
	int getDrevice(int id);
	int getPressingCount(int id);
	int getReleasingCount(int id);
	size_t size();
	bool update();
};