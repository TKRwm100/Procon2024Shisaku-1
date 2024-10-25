#pragma once

#include <vector>

namespace Resource
{
	class Graph
	{
		int handle;
		Graph();
	public:
		Graph(int& handle);
		Graph(int handle);
		Graph(Graph& src);
		Graph* operator=(int);
		Graph* operator=(Graph&);
		operator int();
		static std::vector<Graph> graphs;
	};
	class Font
	{
		int handle;
		Font();
	public:
		Font(int handle);
		Font(Font& src);
		Font* operator=(int);
		Font* operator=(Font&);
		operator int();
		static std::vector<Font> fonts;
	};
	class SoftImage
	{
		int handle;
		SoftImage();
	public:
		SoftImage(int handle);
		SoftImage(SoftImage& src);
		SoftImage* operator=(int);
		SoftImage* operator=(SoftImage&);
		operator int();
		static std::vector<SoftImage> softimages;
	};
}