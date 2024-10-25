#include "Resource.h"
#include "DxLib.h"

namespace Resource
{
	Graph::Graph()
	{
		handle = -1;
	}
	Graph::Graph(int& handle):Graph()
	{
		*this = handle;
		handle = -1;
	}
	Graph::Graph(int handle):Graph()
	{
		*this = handle;
	}
	Graph::Graph(Graph& src):Graph()
	{
		*this = src;
		src.handle = -1;
	}
	Graph* Graph::operator=(int handle)
	{
		if (this->handle != handle)
		{
			DeleteGraph(this->handle);
		}
		this->handle = handle;
		
		return this;
	}
	Graph* Graph::operator=(Graph& src)
	{
		*this = src.handle;
		src.handle = -1;
		return this;
	}
	Graph::operator int()
	{
		return handle;
	}

	std::vector<Graph> Graph::graphs;
	Font::Font()
	{
		handle = -1;
	}
	Font::Font(int handle)
	{
		*this = handle;
	}
	Font::Font(Font& src)
	{
		src.handle = -1;
		*this = src;
	}
	Font* Font::operator=(int handle)
	{
		if (this->handle != handle)
		{
			DeleteFontToHandle(this->handle);
		}
		this->handle = handle;
		
		return this;
	}
	Font* Font::operator=(Font& src)
	{
		*this = src.handle;
		return this;
	}
	Font::operator int()
	{
		return handle;
	}

	std::vector<Font> Font::fonts;
	SoftImage::SoftImage()
	{
		handle = -1;
	}
	SoftImage::SoftImage(int handle)
	{
		*this = handle;
	}
	SoftImage::SoftImage(SoftImage& src)
	{
		src.handle = -1;
		*this = src;
	}
	SoftImage* SoftImage::operator=(int handle)
	{
		if (this->handle != handle)
		{
			DeleteSoftImage(this->handle);
		}
		this->handle = handle;
		
		return this;
	}
	SoftImage* SoftImage::operator=(SoftImage& src)
	{
		*this = src.handle;
		return this;
	}
	SoftImage::operator int()
	{
		return handle;
	}

	std::vector<SoftImage> SoftImage::softimages;
}