#include "DxLib.h"
#include "Resource.h"
#include "config.h"
#include "Keyboard.h"
#include "State.h"
#include "Mouse.h"
#include "Touch.h"
#include <cmath>

#define DEBUG

#ifdef DEBUG

int main()
#else

#ifdef WINDOWS_DESKTOP_OS
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif // WINDOWS_DESKTOP_OS

#ifdef __ANDROID__
int android_main(void)
#endif // __ANDROID__

#ifdef __APPLE__
int ios_main(void)
#if TARGET_OS_IPHONE
//#include "DxFunctioniOS.h"
#endif
#endif // __APPLE__
#endif // DEBUG
{
#ifdef WINDOWS_DESKTOP_OS
	ChangeWindowMode(true);

#endif // WINDOWS_DESKTOP_OS
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	Config::init();
	Resource::Graph::graphs.push_back(LoadGraph("dat\\takaashi_gani.png"));
	Resource::Graph::graphs.push_back(LoadGraph("dat\\bg_natural_ocean.jpg"));
	Resource::Graph::graphs.push_back(LoadGraph("dat\\takaashi_ganiS.png"));
	Resource::SoftImage::softimages.push_back(LoadSoftImage("dat\\takaashi_gani.png"));
	Resource::Font::fonts.push_back(CreateFontToHandle("BIZ UDƒSƒVƒbƒN", 50, 10));
	state state = state::startMenu;
	int time = 0;
	int count = 0;
	double fukasa = 0;
	for (; ProcessMessage() == 0 && Keyboard::getIns()->update() && Mouse::getIns()->update() && ScreenFlip() == 0 && ClearDrawScreen() == 0;)
	{
		switch (state)
		{
		case state::startMenu:
		{
			int x = 0;
			int y = 0;
			int w = 0;
			int h = 0;
			int l = 0;

			GetDrawScreenSize(&x, &y);
			GetDrawStringSizeToHandle(&w, &h, &l, "Start!", 7, Resource::Font::fonts[0]);
			w /= 2;
			h /= 2;
			y = y - 150;
			bool b = (Mouse::getIns()->getPressingCount(MOUSE_INPUT_LEFT) == 1);
			bool b1 = (std::sqrt(std::pow(x / 2 - w - h - Mouse::getIns()->getX(), 2) + std::pow(y - Mouse::getIns()->getY(), 2)) < h);
			bool b2 = (std::sqrt(std::pow(x / 2 + w + h - Mouse::getIns()->getX(), 2) + std::pow(y - Mouse::getIns()->getY(), 2)) < h);
			bool b3 = (((x - w<Mouse::getIns()->getX() || x + w>Mouse::getIns()->getX()) && (y - h<Mouse::getIns()->getY() || y + h>Mouse::getIns()->getY())));
			if (Keyboard::getIns()->getPressingCount(Config::keyConfig[KEY_INPUT_SPACE]) == 1 || (b && (b1 || b2 || b3)))
			{
				state = state::beforeclick;
				time = Config::fps * 4;
			}
			for (size_t i = 0, size = Touch::getIns()->size(); i < size; ++i)
			{
				int id = Touch::getIns()->getID(i);
				bool b = (Touch::getIns()->getPressingCount(id) == 1);
				bool b1 = (std::sqrt(std::pow(x / 2 - w - h - Touch::getIns()->getX(id), 2) + std::pow(y - Touch::getIns()->getY(id), 2)) < h);
				bool b2 = (std::sqrt(std::pow(x / 2 + w + h - Touch::getIns()->getX(id), 2) + std::pow(y - Touch::getIns()->getY(id), 2)) < h);
				bool b3 = (((x - w<Touch::getIns()->getX(id) || x + w>Touch::getIns()->getX(id)) && (y - h<Touch::getIns()->getY(id) || y + h>Touch::getIns()->getY(id))));
				if ((b && (b1 || b2 || b3)))
				{
					state = state::beforeclick;
					time = Config::fps * 4;
				}

			}
			int rt = DrawGraph(0, 0, Resource::Graph::graphs[1], true);
			DrawGraph(0, 0, Resource::Graph::graphs[0], true);

			int c = GetColor(0, 255, 255);
			DrawCircle(x / 2 - w - h, y, h, c, true);
			DrawBox(x / 2 - w - h, y - h, x / 2 + w + h, y + h + 1, c, true);
			DrawCircle(x / 2 + w + h, y, h, c, true);
			DrawStringToHandle(x / 2 - w, y - h, "Start!", GetColor(255, 255, 255), Resource::Font::fonts[0], GetColor(0, 0, 0));
			DrawFormatString(0, 0, GetColor(255, 255, 255), "%d,%d",Resource::Graph::graphs[0], rt);
			break;
		}
		case state::beforeclick:
		{
			if (time == 0)
			{
				state = state::click;
			}
			--time;
			DrawGraph(0, 0, Resource::Graph::graphs[1], true);
			DrawGraph(0, 0, Resource::Graph::graphs[0], true);
			if (time / Config::fps > 0)
			{
				DrawFormatStringToHandle(0, 0, GetColor(255, 255, 255), Resource::Font::fonts[0], "%d", time / Config::fps);
			}
			else
			{
				DrawFormatStringToHandle(0, 0, GetColor(255, 255, 255), Resource::Font::fonts[0], "Go!");
			}
			break;
		}
		case state::click:
		{
			if (time < Config::fps * 5)
			{
				count += (Keyboard::getIns()->getPressingCount(Config::keyConfig[KEY_INPUT_RETURN]) == 1);
			}
			if (time == Config::fps * 5)
			{
				state = state::dive;
				time = 0;
			}
			++time;
			DrawGraph(0, 0, Resource::Graph::graphs[1], true);
			DrawGraph(0, 0, Resource::Graph::graphs[0], true);
			DrawFormatString(0, 0, GetColor(256, 0, 256), "%d,%d", time, count);
			break;
		}
		case state::dive:
		{
			if (time >= Config::fps * Config::diving)
			{
				state = state::watch;
			}
			fukasa = (double)Config::maxDiving / 40 * (count + 0.1) / Config::fps / 1 * time;
			if (fukasa > Config::maxDiving)
			{
				fukasa = Config::maxDiving;
			}
			++time;
			DrawGraph(0, 0, Resource::Graph::graphs[1], true);
			DrawGraph(0, 0, Resource::Graph::graphs[0], true);
			DrawFormatStringToHandle(0, 0, GetColor(256, 0, 256), Resource::Font::fonts[0], "%fm", fukasa);
			break;
		}
		case state::watch:
		{
			int r;
			int g;
			int b;
			int a;
			GetPixelSoftImage(Resource::SoftImage::softimages[0], Mouse::getIns()->getX()-25,Mouse::getIns()->getY()-23.5,&r,&g,&b,&a);
			
			DrawGraph(0, 0, Resource::Graph::graphs[1], true);
			if (a != 0)
			{
				DrawGraph(0, 0, Resource::Graph::graphs[2], true);
			}
			
			DrawGraphF(25, 23.5, Resource::Graph::graphs[0], true);
			DrawFormatStringToHandle(0, 0, GetColor(256, 0, 256), Resource::Font::fonts[0], "%fm", fukasa);

			break;
		}
		case state::battle:
			break;
		default:
			break;

		}
	}
	DxLib_End();

}