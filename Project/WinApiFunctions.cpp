#include "WinApiFunctions.h"
#include <exception>

int WinApiFunctions::m_x = 0;
int WinApiFunctions::m_y = 0;
int WinApiFunctions::m_counter_move = 0;
int WinApiFunctions::m_counter_left_click = 0;
int WinApiFunctions::m_counter_right_click = 0;
int WinApiFunctions::m_counter_virtual_keyboard = 0;


/*function convert values of x and y client's window to values of screen*/
void WinApiFunctions::convertValues(int x, int y)
{
	try
	{
		//my variables
		POINT pt{ x, y };
		RECT sizes1;

		int width = GetSystemMetrics(0);
		int height = GetSystemMetrics(1);

		HWND hand1 = GetActiveWindow();

		//printing before convertion
		std::cout << x << " " << y << std::endl;

		//getting rect of window
		GetWindowRect(hand1, &sizes1);

		if (sizes1.right > 0 && sizes1.left > 0 && sizes1.bottom > 0 && sizes1.top > 0)
		{
			//converting x and y values from client window to screen window
			x = int(x * width / (sizes1.right - sizes1.left));
			y = int(y * height / (sizes1.bottom - sizes1.top));
		}

		//printing after convertion
		std::cout << x << " " << y << std::endl;

		//setting x and y members to the right values
		m_x = x;
		m_y = y;
	}
	catch(std::exception &e)
	{
		//just printing the error and continuing so the program won't be interuppted as often
		std::cout << e.what() << std::endl;
	}
}

/*function moves mouse based on x and y of the class*/
void WinApiFunctions::moveMouse()
{
	if (m_counter_move == COUNTER_TIMES_FOR_MOUSE)
	{
		//moving mouse on screen
		SetCursorPos(m_x, m_y);

		//reseting counter
		m_counter_move = 0;
	}
	else
	{
		//increasing counter by 1
		m_counter_move++;

		//reseting other counters
		m_counter_left_click = 0;
		m_counter_right_click = 0;
		m_counter_virtual_keyboard = 0;
	}
}

/*function performs a left click based on x and y from class*/
void WinApiFunctions::leftClick()
{
	if (m_counter_left_click == COUNTER_TIMES || m_counter_left_click == COUNTER_TIMES + 1)
	{
		//left click on mouse with winApi inputs
		INPUT Inputs[2] = {};
		ZeroMemory(Inputs, sizeof(Inputs));

		Inputs[0].type = INPUT_MOUSE;
		Inputs[0].mi.dx = m_x; // desired X coordinate
		Inputs[0].mi.dy = m_y; // desired Y coordinate
		Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

		Inputs[0].type = INPUT_MOUSE;
		Inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

		Inputs[1].type = INPUT_MOUSE;
		Inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

		//sending input for left click on mouse
		UINT uSent = SendInput(ARRAYSIZE(Inputs), Inputs, sizeof(INPUT));

		//checking for errors
		if (uSent != ARRAYSIZE(Inputs))
		{
			std::cout << (L"SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
		}

		//giving ability for double click
		if (m_counter_left_click == COUNTER_TIMES + 1)
		{
			//reseting counter
			m_counter_left_click = 0;
		}
	}
	else
	{
		//increasing counter by 1
		m_counter_left_click++;

		//reseting other counters
		m_counter_move = 0;
		m_counter_right_click = 0;
		m_counter_virtual_keyboard = 0;
	}
}

/*function performs a right click based on x and y from class*/
void WinApiFunctions::rightClick()
{
	if (m_counter_right_click == COUNTER_TIMES)
	{
		//right click on mouse with winApi inputs
		INPUT Inputs[2] = {};
		ZeroMemory(Inputs, sizeof(Inputs));

		Inputs[0].type = INPUT_MOUSE;
		Inputs[0].mi.dx = m_x; // desired X coordinate
		Inputs[0].mi.dy = m_y; // desired Y coordinate
		Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

		Inputs[0].type = INPUT_MOUSE;
		Inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

		Inputs[1].type = INPUT_MOUSE;
		Inputs[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;

		//sending input for right click on mouse
		UINT uSent = SendInput(ARRAYSIZE(Inputs), Inputs, sizeof(INPUT));

		//checking for errors
		if (uSent != ARRAYSIZE(Inputs))
		{
			std::cout << (L"SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
		}

		//reseting counter
		m_counter_right_click = 0;
	}
	else
	{
		//increasing counter by 1
		m_counter_right_click++;

		//reseting other counters
		m_counter_move = 0;
		m_counter_left_click = 0;
		m_counter_virtual_keyboard = 0;
	}
}

/*function opens/closes virtual keyboard*/
void WinApiFunctions::openCloseVirtualKeyboard()
{
	if (m_counter_virtual_keyboard == COUNTER_TIMES)
	{
		//open/close virtual keyboard with winApi inputs
		INPUT Inputs[6] = {};
		ZeroMemory(Inputs, sizeof(Inputs));

		Inputs[0].type = INPUT_KEYBOARD;
		Inputs[0].ki.wVk = VK_LWIN;

		Inputs[1].type = INPUT_KEYBOARD;
		Inputs[1].ki.wVk = VK_CONTROL;

		Inputs[2].type = INPUT_KEYBOARD;
		Inputs[2].ki.wVk = 0x4F; //O key

		Inputs[3].type = INPUT_KEYBOARD;
		Inputs[3].ki.wVk = VK_LWIN;
		Inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

		Inputs[4].type = INPUT_KEYBOARD;
		Inputs[4].ki.wVk = VK_CONTROL;
		Inputs[4].ki.dwFlags = KEYEVENTF_KEYUP;

		Inputs[5].type = INPUT_KEYBOARD;
		Inputs[5].ki.wVk = 0x4F; //O key
		Inputs[5].ki.dwFlags = KEYEVENTF_KEYUP;

		//sending input for open/close virtual keyboard click on mouse
		UINT uSent = SendInput(ARRAYSIZE(Inputs), Inputs, sizeof(INPUT));

		//checking for errors
		if (uSent != ARRAYSIZE(Inputs))
		{
			std::cout << (L"SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
		}

		//reseting counter
		m_counter_virtual_keyboard = 0;
	}
	else
	{
		//increasing counter by 1
		m_counter_virtual_keyboard++;

		//reseting other counters
		m_counter_move = 0;
		m_counter_left_click = 0;
		m_counter_right_click = 0;
	}
}
