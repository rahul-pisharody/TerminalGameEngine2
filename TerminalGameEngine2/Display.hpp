
#pragma once
#include <vector>
//TODO remove windows.h depencency
#include <Windows.h>

#include "Core\Math\Vec.hpp"
/*
Base class for displaying graphics (supposed to be Platform independent)
*/
class Display
{
public:
	virtual bool initDisplay(int width, int height, int pix_size) = 0;

	virtual void update() = 0;
	virtual void clear() = 0;
	virtual void fill(short col, short sym) = 0;
	virtual void draw(int x, int y, short col, short sym) = 0;


	virtual std::vector<Vec2D> drawLine(int x1, int y1, int x2, int y2, short col, int sym, bool get_points) = 0;
	virtual std::vector<Vec2D> drawLine(Vec2D pt1, Vec2D pt2, short col, int sym, bool get_points) = 0;

	virtual std::vector<Vec2D> drawTriangleWireframe(Vec2D pt1, Vec2D pt2, Vec2D pt3, short col, int sym, bool get_points) = 0;
	virtual void drawTriangleBarycentric(Vec2D pt1, Vec2D pt2, Vec2D pt3, short col, short sym) = 0;
	virtual void drawTriangleBresenham(Vec2D pt1, Vec2D pt2, Vec2D pt3, short col, short sym) = 0;


	virtual std::vector<Vec2D> drawFilledTriangle(Vec2D pt1, Vec2D pt2, Vec2D pt3, short col, int sym) = 0;

	virtual int getDisplayWidth() = 0;
	virtual int getDisplayHeight() = 0;

	virtual CHAR_INFO getColourGrey(float lum) = 0;//TODO: Change return type
	virtual void setTerminalTitle(std::string title) = 0;
};