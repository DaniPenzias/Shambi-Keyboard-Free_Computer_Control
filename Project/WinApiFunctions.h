#pragma once
#include <iostream>
#include <Windows.h>
#include <WinUser.h>

#define COUNTER_TIMES 3
#define COUNTER_TIMES_FOR_MOUSE 1

class WinApiFunctions
{
public:
	static void convertValues(int x, int y);
	static void moveMouse();
	static void leftClick();
	static void rightClick();
	static void openCloseVirtualKeyboard();

	static int m_x;
	static int m_y;
	static int m_counter_move;
	static int m_counter_left_click;
	static int m_counter_right_click;
	static int m_counter_virtual_keyboard;
};
