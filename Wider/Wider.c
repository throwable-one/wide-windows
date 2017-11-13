#include <Windows.h>
#include <stdio.h>
// Run app, set cursor over some window and hold Shift
// Window will be resized to cover your virtual screen (desktop)


static HWND GetWindowToResize()
{
	POINT p;

	while (1)
	{
		if (!GetCursorPos(&p))
		{
			fprintf(stderr, "GetCursorPos error %ld", GetLastError());
			return NULL;
		}
		if (GetAsyncKeyState(VK_SHIFT))
		{
			return WindowFromPoint(p);
		}
		Sleep(500); //busy loop do suck, use window + cross-cursor like procexp and winspector
	}
}

static HWND GetOverlappedWindow(HWND _In_ window)
{
	HWND parent = window;
	while (window)
	{
		parent = window;
		window = GetParent(window);
	}
	return parent;
}

// Border is here to stress that window is restored, not maxamized
#define BORDER_AROUND 4

int main()
{	
	int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	
	if (!width || !height)
	{
		return -1; //TODO: Report errors to event log, check other functions for errors 
	}
	HWND window = GetOverlappedWindow(GetWindowToResize());
	ShowWindow(window, SW_RESTORE);
	SetWindowPos(window, NULL, 0, 0, width - BORDER_AROUND, height - BORDER_AROUND, SWP_SHOWWINDOW);
	return 0;
}
