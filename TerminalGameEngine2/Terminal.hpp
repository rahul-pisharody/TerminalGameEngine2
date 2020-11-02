#pragma once
/*
GRAPHICS LIBRARY SPECIFIC

Manages the terminal display and related functions

A lot of the code here is from Javidx9's Console Game Engine.
https://github.com/OneLoneCoder/videos/blob/master/olcConsoleGameEngine.h

License
~~~~~~~
One Lone Coder Console Game Engine  Copyright (C) 2018  Javidx9
This program comes with ABSOLUTELY NO WARRANTY.
This is free software, and you are welcome to redistribute it
under certain conditions; See license for details.
Original works located at:
https://www.github.com/onelonecoder
https://www.onelonecoder.com
https://www.youtube.com/javidx9
GNU GPLv3
https://github.com/OneLoneCoder/videos/blob/master/LICENSE

*/
#include <Windows.h>

#include <iostream>
#include <algorithm>
#include <vector>
#include <thread>

#include  "Core\Math\Vec.hpp"
#include "Display.hpp"

#ifndef UNICODE
#error Please enable UNICODE for your compiler! VS: Project Properties -> General -> \
Character Set -> Use Unicode. - javidx9
#endif
enum COLOUR
{
	FG_BLACK = 0x0000,
	FG_DARK_BLUE = 0x0001,
	FG_BLUE = 0x0009,
	FG_DARK_GREEN = 0x0002,
	FG_GREEN = 0x000A,
	FG_DARK_CYAN = 0x0003,
	FG_CYAN = 0x000B,
	FG_DARK_RED = 0x0004,
	FG_RED = 0x000C,
	FG_DARK_MAGENTA = 0x0005,
	FG_MAGENTA = 0x000D,
	FG_DARK_YELLOW = 0x0006,
	FG_YELLOW = 0x000E,
	FG_GREY = 0x0007,
	FG_DARK_GREY = 0x0008,
	FG_WHITE = 0x000F,

	BG_BLACK = 0x0000,
	BG_DARK_BLUE = 0x0010,
	BG_DARK_GREEN = 0x0020,
	BG_DARK_CYAN = 0x0030,
	BG_DARK_RED = 0x0040,
	BG_DARK_MAGENTA = 0x0050,
	BG_DARK_YELLOW = 0x0060,
	BG_GREY = 0x0070,
	BG_DARK_GREY = 0x0080,
	BG_BLUE = 0x0090,
	BG_GREEN = 0x00A0,
	BG_CYAN = 0x00B0,
	BG_RED = 0x00C0,
	BG_MAGENTA = 0x00D0,
	BG_YELLOW = 0x00E0,
	BG_WHITE = 0x00F0,
	FILL_BLACK = 0x0000,
	FILL_DARK_BLUE = 0x0011,
	FILL_DARK_GREEN = 0x0022,
	FILL_DARK_CYAN = 0x0033,
	FILL_DARK_RED = 0x0044,
	FILL_DARK_MAGENTA = 0x0055,
	FILL_DARK_YELLOW = 0x0066,
	FILL_GREY = 0x0077,
	FILL_DARK_GREY = 0x0088,
	FILL_BLUE = 0x0099,
	FILL_GREEN = 0x00AA,
	FILL_CYAN = 0x00BB,
	FILL_RED = 0x00CC,
	FILL_MAGENTA = 0x00DD,
	FILL_YELLOW = 0x00EE,
	FILL_WHITE = 0x00FF,
};

enum PIXEL_SYM
{
	PIXEL_FULL = 0x2588,
	PIXEL_THREEQUARTERS = 0x2593,
	PIXEL_HALF = 0x2592,
	PIXEL_QUARTER = 0x2591,
};


class Terminal : public Display
{
private:
	void fillTriangle(std::vector<Vec2D> boundary, short col, short sym);

public:
	Terminal();
	~Terminal();
	bool initDisplay(int width, int height, int font_size);
	void update();
	void clear();
	void fill(short col, short sym = PIXEL_FULL);
	void draw(int x, int y, short col = FG_WHITE, short sym = PIXEL_FULL);


	std::vector<Vec2D> drawLine(int x1, int y1, int x2, int y2, short col = FG_WHITE, int sym = PIXEL_FULL, bool get_points = false);
	std::vector<Vec2D> drawLine(Vec2D pt1, Vec2D pt2, short col = FG_WHITE, int sym = PIXEL_FULL, bool get_points = false);

	std::vector<Vec2D> drawTriangleWireframe(Vec2D pt1, Vec2D pt2, Vec2D pt3, short col = FG_WHITE, int sym = PIXEL_FULL, bool get_points = false);
	void drawTriangleBarycentric(Vec2D pt1, Vec2D pt2, Vec2D pt3, short col = FG_WHITE, short sym = PIXEL_FULL);
	void drawTriangleBresenham(Vec2D pt1, Vec2D pt2, Vec2D pt3, short col = FG_WHITE, short sym = PIXEL_FULL);


	std::vector<Vec2D> drawFilledTriangle(Vec2D pt1, Vec2D pt2, Vec2D pt3, short col, int sym = PIXEL_FULL);

	int getDisplayWidth() { return m_console_scr_width; }
	int getDisplayHeight() { return m_console_scr_height; }

	CHAR_INFO getColourGrey(float lum);
	void setTerminalTitle(std::string title);
private:
	HANDLE m_console_handle_out;
	HANDLE m_console_handle_in;
	int m_console_scr_height;
	int m_console_scr_width; 
	SMALL_RECT m_rectWindow;

	CHAR_INFO *m_scr_buf;
};