#include "Terminal.hpp"



Terminal::Terminal()
{
	m_console_handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	m_console_handle_in = GetStdHandle(STD_INPUT_HANDLE);
}

Terminal::~Terminal()
{
	delete m_scr_buf;
}

bool Terminal::initDisplay(int width, int height, int font_size)
{
	m_console_scr_height = height;
	m_console_scr_width = width;
	SMALL_RECT rect_temp;
	rect_temp = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(m_console_handle_out, TRUE, &rect_temp);

	COORD buf_size = { (short)m_console_scr_width, (short)m_console_scr_height };
	if(!SetConsoleScreenBufferSize(m_console_handle_out, buf_size))
		wprintf(L"SetConsoleScreenBufferSize %d", GetLastError());



	//Set Font
	CONSOLE_FONT_INFOEX font_info;
	font_info.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	COORD font_s = { (short)font_size, (short)font_size };
	font_info.dwFontSize = font_s;
	font_info.nFont = 0;
	font_info.FontFamily = FF_DONTCARE;
	font_info.FontWeight = FW_NORMAL;


	wcscpy_s(font_info.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(m_console_handle_out, FALSE, &font_info);



	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(m_console_handle_out, &csbi))
	{
		wprintf(L"ERROR: %s\n\t%s\n", L"GetConsoleScreenBufferInfo");
		return false;
	}
	if (m_console_scr_height > csbi.dwMaximumWindowSize.Y)
	{
		wprintf(L"ERROR: %s\n\t%s\n", L"Screen Height / Font Height Too Big");
		return false;
	}
	if (m_console_scr_width > csbi.dwMaximumWindowSize.X)
	{
		wprintf(L"ERROR: %s\n\t%s\n", L"Screen Width / Font Width Too Big");
		return false;
	}

	m_rectWindow = { 0, 0, (short)m_console_scr_width - 1, (short)m_console_scr_height - 1 };

	//std::string title = L"Terminal Game Engine 2";
	wchar_t s[256];
	swprintf_s(s, 256, L"OneLoneCoder.com - Console Game Engine ");
	//TCHAR titl_t[] = ;
	SetConsoleTitle(s);
	if (!SetConsoleWindowInfo(m_console_handle_out, TRUE, &m_rectWindow))
	{
		wprintf(L"FAILED %d",GetLastError());
	}

	m_scr_buf = new CHAR_INFO[m_console_scr_width*m_console_scr_height];
	memset(m_scr_buf, 0, sizeof(CHAR_INFO)*m_console_scr_width*m_console_scr_height);

	//Set Cursor invisible
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(m_console_handle_out, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(m_console_handle_out, &cursorInfo);

	return true;
}

void Terminal::clear()
{
	memset(m_scr_buf, 0, sizeof(CHAR_INFO)*m_console_scr_width*m_console_scr_height);
}

void Terminal::fill(short col, short sym)
{
	for (int y = 0; y < m_console_scr_height; y++)
	{
		for (int x = 0; x < m_console_scr_width; x++)
		{
			draw(x, y, col, sym);
		}
	}
}

void Terminal::draw(int x, int y, short col, short sym)
{
	if (x >= 0 && x < m_console_scr_width && y >= 0 && y < m_console_scr_height)
	{
		m_scr_buf[y * m_console_scr_width + x].Char.UnicodeChar = sym;
		m_scr_buf[y * m_console_scr_width + x].Attributes = col;
	}
}

std::vector<Vec2D> Terminal::drawLine(int x1, int y1, int x2, int y2, short col, int sym, bool get_points)
{
	
	std::vector<Vec2D> vecs;

	float dx = (x2 - x1);
	float dy = (y2 - y1);
	float limit;
	if (std::abs(dx) >= std::abs(dy)) limit = std::abs(dx);
	else limit = std::abs(dy);
	dx = dx / limit; dy = dy / limit;
	int xdir, ydir;
	if (dx>0) xdir = 1; else xdir = -1;
	if (dy>0) ydir = 1; else ydir = -1;
	int i = 1;

	float tx, ty;
	tx = x1; ty = y1;
	while (i <= limit)
	{
		if (get_points)
		{
			vecs.push_back(Vec2D(tx, ty));
		}
		
		draw((int)tx, (int)ty, col, sym);
		
		tx = tx + dx;
		ty = ty + dy;
		i++;
	}
	return vecs;
}

std::vector<Vec2D> Terminal::drawLine(Vec2D pt1, Vec2D pt2, short col, int sym, bool get_points)
{
	std::vector<Vec2D> vecs = drawLine(pt1.X, pt1.Y, pt2.X, pt2.Y, col, sym, false);
	return vecs;
}

void Terminal::drawTriangleBarycentric(Vec2D pt1, Vec2D pt2, Vec2D pt3, short col, short sym)
{

	auto edgeFunction = [](const Vec2D &a, const Vec2D &b, const Vec2D &c)
	{
		return (c.X - a.X) * (b.Y - a.Y) - (c.Y - a.Y) * (b.X - a.X);
	};
	
	int min_x, max_x, min_y, max_y;
	min_x = min(pt1.X, min(pt2.X, pt3.X));
	min_y = min(pt1.Y, min(pt2.Y, pt3.Y));
	max_x = max(pt1.X, max(pt2.X, pt3.X));
	max_y = max(pt1.Y, max(pt2.Y, pt3.Y));

	float area = edgeFunction(pt1, pt2, pt3);
	bool test_flag = false;
	for (uint32_t j = min_y; j <= max_y; ++j) {
		for (uint32_t i = min_x; i <= max_x; ++i) {
			Vec2D p;
			p.X = i + 0.5f;
			p.Y = j + 0.5f;
			float w0 = edgeFunction(pt2, pt3, p);
			float w1 = edgeFunction(pt3, pt1, p);
			float w2 = edgeFunction(pt1, pt2, p);
			if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
				w0 /= area;
				w1 /= area;
				w2 /= area;
				draw(i, j, col, sym);
				test_flag = true;
			}
		}
	}
	if (!test_flag)
	{
		float test = 0.01f;
	}
}

//https://github.com/OneLoneCoder/videos/blob/master/olcConsoleGameEngine.h
void Terminal::drawTriangleBresenham(Vec2D pt1, Vec2D pt2, Vec2D pt3, short col, short sym)
{
	auto SWAP = [](int &x, int &y) { int t = x; x = y; y = t; };
	auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) draw(i, ny, col, sym); };

	int x1 = pt1.X; int y1 = pt1.Y;
	int x2 = pt2.X; int y2 = pt2.Y;
	int x3 = pt3.X; int y3 = pt3.Y;
	int t1x, t2x, y, minx, maxx, t1xp, t2xp;
	bool changed1 = false;
	bool changed2 = false;
	int signx1, signx2, dx1, dy1, dx2, dy2;
	int e1, e2;
	// Sort vertices
	if (y1>y2) { SWAP(y1, y2); SWAP(x1, x2); }
	if (y1>y3) { SWAP(y1, y3); SWAP(x1, x3); }
	if (y2>y3) { SWAP(y2, y3); SWAP(x2, x3); }

	t1x = t2x = x1; y = y1;   // Starting points
	dx1 = (int)(x2 - x1); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y2 - y1);

	dx2 = (int)(x3 - x1); if (dx2<0) { dx2 = -dx2; signx2 = -1; }
	else signx2 = 1;
	dy2 = (int)(y3 - y1);

	if (dy1 > dx1) {   // swap values
		SWAP(dx1, dy1);
		changed1 = true;
	}
	if (dy2 > dx2) {   // swap values
		SWAP(dy2, dx2);
		changed2 = true;
	}

	e2 = (int)(dx2 >> 1);
	// Flat top, just process the second half
	if (y1 == y2) goto next;
	e1 = (int)(dx1 >> 1);

	for (int i = 0; i < dx1;) {
		t1xp = 0; t2xp = 0;
		if (t1x<t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i<dx1) {
			i++;
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) t1xp = signx1;//t1x += signx1;
				else          goto next1;
			}
			if (changed1) break;
			else t1x += signx1;
		}
		// Move line
	next1:
		// process second line until y value is about to change
		while (1) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;//t2x += signx2;
				else          goto next2;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next2:
		if (minx>t1x) minx = t1x; if (minx>t2x) minx = t2x;
		if (maxx<t1x) maxx = t1x; if (maxx<t2x) maxx = t2x;
		drawline(minx, maxx, y);    // Draw line from min to max points found on the y
									// Now increase y
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y == y2) break;

	}
next:
	// Second half
	dx1 = (int)(x3 - x2); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y3 - y2);
	t1x = x2;

	if (dy1 > dx1) {   // swap values
		SWAP(dy1, dx1);
		changed1 = true;
	}
	else changed1 = false;

	e1 = (int)(dx1 >> 1);

	for (int i = 0; i <= dx1; i++) {
		t1xp = 0; t2xp = 0;
		if (t1x<t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i<dx1) {
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) { t1xp = signx1; break; }//t1x += signx1;
				else          goto next3;
			}
			if (changed1) break;
			else   	   	  t1x += signx1;
			if (i<dx1) i++;
		}
	next3:
		// process second line until y value is about to change
		while (t2x != x3) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;
				else          goto next4;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next4:

		if (minx>t1x) minx = t1x; if (minx>t2x) minx = t2x;
		if (maxx<t1x) maxx = t1x; if (maxx<t2x) maxx = t2x;
		drawline(minx, maxx, y);
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y>y3) return;
	}
}

std::vector<Vec2D> Terminal::drawTriangleWireframe(Vec2D pt1, Vec2D pt2, Vec2D pt3, short col, int sym, bool get_points)
{
	if (!get_points) 
	{
		drawLine(pt1, pt2, col, sym);
		drawLine(pt2, pt3, col, sym);
		drawLine(pt3, pt1, col, sym);
		return {};
	}
	std::vector<Vec2D>vecs1 = drawLine(pt1, pt2, col, sym, get_points);
	std::vector<Vec2D>vecs2 = drawLine(pt2, pt3, col, sym, get_points);
	vecs1.insert(vecs1.end(), vecs2.begin(), vecs2.end());
	vecs2 = drawLine(pt3, pt1, col, sym);
	vecs1.insert(vecs1.end(), vecs2.begin(), vecs2.end());
	return vecs1;
}

//SLOW - VERY SLOW
std::vector<Vec2D> Terminal::drawFilledTriangle(Vec2D pt1, Vec2D pt2, Vec2D pt3, short col, int sym)
{
	std::vector<Vec2D> boundary = drawTriangleWireframe(pt1, pt2, pt3, col, sym, true);
	fillTriangle(boundary, col, sym);
	return boundary;
}

void Terminal::setTerminalTitle(std::string title)
{
	wchar_t s[256];
	std::wstring wide_string = std::wstring(title.begin(), title.end());
	const wchar_t* result = wide_string.c_str();
	swprintf_s(s, 256, result);
	SetConsoleTitle(s);
}

//Do not use
void Terminal::fillTriangle(std::vector<Vec2D> boundary, short col, short sym)
{
	auto vec2d_comparison_fn = [](Vec2D v1, Vec2D v2)
	{
		if ((int)v1.Y == (int)v2.Y) {
			return (v1.X < v2.X);
		}
		return (v1.Y < v2.Y);
	};

	std::sort(boundary.begin(), boundary.end(), vec2d_comparison_fn);

	
	for (std::vector<Vec2D>::iterator iter = boundary.begin(); (iter+1) != boundary.end(); iter++)
	{

		Vec2D v1;
		if (((int)((*iter).Y) == (int)(*(iter + 1)).Y) && (((*(iter+1)).X-(*(iter)).X)>1)) {
			v1.X = (*iter).X + 1; v1.Y = (*iter).Y;
			drawLine(v1, (*(iter + 1)), col, sym);
		}
	}
}

void Terminal::update()
{
	WriteConsoleOutput(m_console_handle_out, m_scr_buf, { (short)m_console_scr_width, (short)m_console_scr_height }, { 0,0 }, &m_rectWindow);
}


//Algorithm by OneLoneCoder (javidx9)
CHAR_INFO Terminal::getColourGrey(float lum)
{
	short bg_col, fg_col;
	wchar_t sym;
	int pixel_bw = (int)(13.0f*lum);
	switch (pixel_bw)
	{
	case 0: bg_col = BG_BLACK; fg_col = FG_BLACK; sym = PIXEL_FULL; break;

	case 1: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_QUARTER; break;
	case 2: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_HALF; break;
	case 3: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_THREEQUARTERS; break;
	case 4: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_FULL; break;

	case 5: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_QUARTER; break;
	case 6: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_HALF; break;
	case 7: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_THREEQUARTERS; break;
	case 8: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_FULL; break;

	case 9:  bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_QUARTER; break;
	case 10: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_HALF; break;
	case 11: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_THREEQUARTERS; break;
	case 12: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_FULL; break;
	default:
		bg_col = BG_BLACK; fg_col = FG_WHITE; sym = PIXEL_FULL;
	}

	CHAR_INFO c_info;
	c_info.Attributes = bg_col | fg_col;
	c_info.Char.UnicodeChar = sym;
	return c_info;
}