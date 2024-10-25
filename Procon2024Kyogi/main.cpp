#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <cpprest/http_client.h>
#include <future>
#include <sstream>

// for convenience
using json = nlohmann::json;

json js;
typedef char masu ;
struct kataType
{
	std::vector<std::vector<bool>> cells;
};
class op
{
	size_t p;
	int x;
	int y;
	int s;

	size_t last;
	size_t n;
	
	int right;
public:
	op(size_t p, int x, int y, int s,size_t last,int right);
	void setRight(int right);
	int getRight();
	size_t getN();
	json getJson();
	json getOps();
};
std::vector<std::vector<masu>> start;
std::vector<std::vector<masu>> goal;
std::vector<std::vector<masu>> now;
std::vector<kataType> kata;
std::vector<op> opv;
size_t last=-1;
size_t best=-1;

std::vector<std::thread> draw;

void fdrawboard(std::vector<std::vector<masu>> target = now)
{
	std::stringstream out;
	for (size_t i = 0,size=target.size(); i < size; i++)
	{
		for (size_t j = 0,ssize=target[i].size(); j < ssize; j++)
		{
			out << (int)target[i][j] << " ";
			
		}
		out << "\n";
	}
	out << "\n";
	std::cout << out.str();
}
void drawboard()
{
	draw.push_back(std::thread(fdrawboard,now));
}
void getboard(json js)
{

	json jsb = js["board"];
	int h = jsb["height"];
	int w = jsb["width"];
	try
	{
		if (h != jsb["start"].size() || h != jsb["goal"].size())
		{
			throw std::exception("height error");
		}
		for (size_t i = 0, size = h; i < size; i++)
		{
			if (w != (jsb["start"][i]).dump().size() - 2 || w != (jsb["goal"][i]).dump().size() - 2)
			{
				throw std::exception("width error");
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	start.resize(h);
	goal.resize(h);
	for (size_t i = 0, size = h; i < size; i++)
	{
		start[i].resize(w);
		goal[i].resize(w);
		for (size_t j = 0; j < w; ++j)
		{
			start[i][j] = std::stoi(std::string{ jsb["start"][i].dump()[j + 1] });
			goal[i][j] = std::stoi(std::string{ jsb["goal"][i].dump()[j + 1] });
		}
	}
	now = start;
}
void getkata(json js)
{
	json jsg = js["general"];
	size_t n = jsg["n"];
	try
	{
		if (jsg["patterns"].size() != n)
		{

			throw std::exception("patterns error");
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	kata.resize(25 + n);
	for (size_t i = 0; i < n; ++i)
	{
		json pt=jsg["patterns"][i];
		size_t p = pt["p"];
		try
		{
			if (p<25)
			{
				throw std::exception("p error");
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
		int w = pt["width"];
		int h = pt["height"];
		try
		{
			if (pt["cells"].size() != h)
			{
				throw std::exception("height error");
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
		kata[p].cells.resize(h);
		for (int j = 0; j < h; ++j)
		{
			json cellr = pt["cells"][j];
			try
			{
				if (cellr.dump().size()!=w+2)
				{
					throw std::exception("width error");
				}
			}
			catch (const std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}

			kata[p].cells[j].resize(w);

			for (int k = 0; k < w; ++k)
			{
				 kata[p].cells[j][k]=cellr.dump()[1 + k] =='1';
			}
		}
	}
	kata[0].cells = { {1} };
	int size = 2;
	for (int i = 1; i < 25; ++i)
	{
		kata[i].cells.resize(size);
		for (int j = 0; j < size; ++j)
		{
			kata[i].cells[j].resize(size);
			if (i % 3 == 2)
			{
				continue;
			}
			for (int k = 0; k < size; ++k)
			{
				if (i % 3 == 0 && k % 2 == 1)
				{

				}
				else
				{
					kata[i].cells[j][k] = 1;
				}
			}
		}
		if (i % 3 == 0)
		{
			size *= 2;
		}
	}
	return;
}
void move(std::vector<std::vector<masu>>&now,int x, int y, int s)
{
	if (x < 0 || y < 0 || y >= now.size() || x >= now[0].size())
	{
		return;
	}
	int mem = now[y][x];
	switch (s)
	{
	case 0:
		for (int i = y+1, size = now.size(); i < size; ++i)
		{
			now[i - 1][x] = now[i][x];
		}
		now[now.size() - 1][x] = mem;
		break;
	case 1:
		for (int i = y, size = now.size()-1; i > 0; --i)
		{
			now[i][x] = now[i - 1][x];
		}
		now[0][x] = mem;
		break;
	case 2:
		for (int i = x+1, size = now[0].size(); i < size; ++i)
		{
			now[y][i - 1] = now[y][i];
		}
		now[y][now[0].size() - 1] = mem;
		break;
	case 3:
		for (int i = x, size = now[0].size() - 1; i > 0; --i)
		{
			now[y][i] = now[y][i - 1];
		}
		now[y][0] = mem;
		break;
	}
}
void usekata(size_t kataindex, int x, int y, int s)
{
	std::vector<std::vector<masu>> buf;
	buf.resize(now.size());
	for (int i = 0,size=now.size(); i < size; i++)
	{
		buf[i].resize(now[0].size(), -1);
	}
	for (int i = 0, size = kata[kataindex].cells.size(); i < size; ++i)
	{
		buf[i].resize(now[0].size(), -1);
		if (i + y < 0 || i + y >= now.size())
		{
			continue;
		}
		for (int j=0,ssize=kata[kataindex].cells[0].size();j<ssize;++j)
		{
			if (j + x<0 || j + x>=now[i + y].size())
			{
				continue;
			}
			if (kata[kataindex].cells[i][j])
			{
				buf[i + y][j + x]= now[i + y][j + x];
				now[i + y][j + x] = -1;
			}
		}
	}
	switch (s)
	{
	case 0:
		for (int i = 0, size = now.size(); i < size; ++i)
		{
			for (int j = 0, ssize = now[0].size(); j < ssize; ++j)
			{
				if (now[now.size() - 1 - i][j] == -1)
				{
					move(now,j, now.size() - 1 - i, 0);
				}
			}
		}
		for (int i = 0, size = buf[0].size(); i < size; ++i)//x
		{
			for (int j = buf.size()-1; j >0;--j)//y
			{
				bool flag = true;
				for (int k = j; k >= 0; --k)
				{
					if (buf[k][i] != -1)
					{
						flag = false;
					}
				}
				if (flag)
				{
					break;
				}
				if (buf[j][i] == -1)
				{
					move(buf,i,j,  1);
					++j;

				}
			}
		}
		break;
	case 1:
		for (int i = 0, size = now.size(); i < size; ++i)
		{
			for (int j = 0, ssize = now[0].size(); j < ssize; ++j)
			{
				if (now[i][j] == -1)
				{
					move(now,j,i, 1);
				}
			}
		}
		for (int i = 0, size = buf[0].size(); i < size; ++i)//x
		{
			for (int j =0,ssize= buf.size(); j <ssize;++j)//y
			{
				bool flag = true;
				for (int k = j; k <ssize; ++k)
				{
					if (buf[k][i] != -1)
					{
						flag = false;
					}
				}
				if (flag)
				{
					break;
				}
				if (buf[j][i] == -1)
				{
					move(buf,i,j,  0);
					--j;

				}
			}
		}
		break;
	case 2:
		for (int i = 0, size = now.size(); i < size; ++i)
		{
			for (int j = 0, ssize = now[0].size(); j < ssize; ++j)
			{
				if (now[i][now[0].size() - 1 - j] == -1)
				{
					move(now,now[0].size() - 1 - j, i, 2);
				}
			}
		}
		for (int i = 0, size = buf.size(); i < size; ++i)//y
		{
			for (int j = buf[0].size()-1; j >0;--j)//x
			{
				bool flag = true;
				for (int k = j; k >= 0; --k)
				{
					if (buf[i][k] != -1)
					{
						flag = false;
					}
				}
				if (flag)
				{
					break;
				}
				if (buf[i][j] == -1)
				{
					move(buf,j,i,  3);
					++j;

				}
			}
		}
		break;
	case 3:
		for (int i = 0, size = now.size(); i < size; ++i)
		{
			for (int j = 0, ssize = now[0].size(); j < ssize; ++j)
			{
				if (now[i][j] == -1)
				{
					move(now,j, i, 3);
				}
			}
		}
		for (int i = 0, size = buf.size(); i < size; ++i)//y
		{
			for (int j =0,ssize= buf[0].size(); j <ssize;++j)//x
			{
				bool flag = true;
				for (int k = j; k <ssize; ++k)
				{
					if (buf[i][k] != -1)
					{
						flag = false;
					}
				}
				if (flag)
				{
					break;
				}
				if (buf[i][j] == -1)
				{
					move(buf,j,i,  2);
					--j;

				}
			}
		}
		break;
	default:
		break;
	}
	buf;
	for (int i = 0, size = buf.size(); i < size; ++i)
	{
		for (int j = 0, ssize = buf[0].size(); j < ssize; ++j)
		{
			if (buf[i][j] != -1)
			{
				now[i][j] = buf[i][j];
			}
		}
	}
	int right = 0;
	for (int i = 0, size = goal.size(); i < size; ++i)
	{
		for (int j = 0, ssize = goal[0].size(); j < ssize; ++j)
		{
			if (now[i][j] == goal[i][j])
			{
				++right;
			}
		}
	}
	
	opv.push_back(op(kataindex, x, y, s, last,right));
	last = opv.size() - 1;
	if (best == -1 || opv[best].getRight() < opv[last].getRight() || (opv[best].getRight() == opv[last].getRight() && opv[best].getN() < opv[last].getN()))
	{
		best = last;
	}
	return;
}
int main(int argc, char** argv)
{
	if (argc<2)
	{
		return 0;
	}
	std::ifstream ifs(argv[1]);
	ifs>>js;
	/*
	web::http::client::http_client client(L"http://example.com/");
	auto response = client.request(web::http::methods::GET).get();
	if (response.status_code() == web::http::status_codes::OK)
	{
		// レスポンスを文字列として取得後、標準出力する
		js=json::parse( response.extract_string().get());
	
	}*/

	getboard(js);
	getkata(js);
	fdrawboard();
	drawboard();
	std::cout << "\n";
	//move(5, 3, 3);
	usekata(1, 5, 3, 3);
	drawboard();
	usekata(1, 5, 3, 3);
	drawboard();
	json rt= opv[best].getJson();
	std::ofstream ofs("output.json");
	ofs << rt;
	for (size_t i = 0, size = draw.size(); i < size; ++i)
	{
		draw[i].join();
	}
	int end;
	std::cin>>end;
	return 0;
}

op::op(size_t p, int x, int y, int s, size_t last,int right)
{
	this->p = p;
	this->x = x;
	this->y = y;
	this->s = s;
	this->last = last;
	if (last == -1)
	{
		n = 1;
	}
	else
	{
		this->n = opv[last].n + 1;
	}
	this->right = right;
}

void op::setRight(int right)
{
	this->right = right;
}

int op::getRight()
{
	return right;
}

size_t op::getN()
{
	return n;
}

json op::getJson()
{
	json rt;
	rt["n"] = this->n;
	size_t index =last;
	rt["ops"]=json::array();
		rt["ops"].insert(rt["ops"].begin(),getOps());
	for (; true;)
	{
		if (index==-1)
		{
			break;
		}
		rt["ops"].insert(rt["ops"].begin(), opv[index].getOps());
		index = opv[index].last;
	}
	return rt;
}

json op::getOps()
{
	json rt;
	rt["p"] = this->p;
	rt["x"] = this->x;
	rt["y"] = this->y;
	rt["s"] = this->s;
	return rt;
}


