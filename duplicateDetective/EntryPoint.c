#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <shlobj_core.h>
#include "findDuplicate.h"


#define WND_HEIGHT 750
#define WND_WIDTH 450

#define MAX_PATH 260
#define PATHMAXSIZE 2048

#define MENU_EXIT 31 
#define MENU_SCAN 27
#define MENU_SAVE 28
#define MENU_ABOUT 30
#define MENU_CLEAR_RESULTS 24

HWND liboDup;
HMENU hMenu;
HINSTANCE my_hInst;
HWND dirWnd;
FILES* duplicates = NULL;

void SetControls(HWND);
void MainMenu(HWND hWnd);
void outputToFile(FILES* files, char* path);
void scanDir();
void viewResults();
void saveResults();
char* getDirectory();
DWORD WINAPI scanThread(LPVOID lpParam);
void startScanning();


LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_COMMAND:
		switch (wParam) {
		case MENU_SAVE:
			saveResults();
			break;
		case MENU_SCAN:
			startScanning();
			break;
		case MENU_CLEAR_RESULTS:
			SendMessageW(liboDup, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
			duplicates = NULL;
			break;
		case MENU_EXIT:
			MessageBeep(MB_OK);
			if (MessageBoxW(hWnd, L"Are you sure?", L"Exit", MB_YESNO) == IDYES)
				DestroyWindow(hWnd);
			break;
		case MENU_ABOUT:
			MessageBoxW(NULL, L"A software tool that allows to find duplicate files in a given directory.", L"Info", MB_OK);
			break;
		}
		break;
	case WM_CREATE:
		MainMenu(hWnd);
		SetControls(hWnd);
		break;
	case WM_CLOSE:
		MessageBeep(MB_OK);
		if (MessageBoxW(hWnd, L"Are you sure?", L"Exit", MB_YESNO) == IDYES)
			DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return DefWindowProcW(hWnd, uMsg, wParam, lParam);
		break;
	default:
		return DefWindowProcW(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSEX wcex;
	HWND hWnd;
	MSG msg;
	memset(&wcex, 0, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = 0;
	wcex.lpfnWndProc = WindowProcedure;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"MyWindowClass";
	wcex.hIconSm = 0;

	RegisterClassEx(&wcex);

	hWnd = CreateWindowExW(0, L"MyWindowClass", L"Dublicate Detective", 
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
		0, 0, WND_HEIGHT, WND_WIDTH, 0, 0, hInstance, NULL);

	while (GetMessage(&msg, 0, 0, 0))
	{
		DispatchMessage(&msg);
	}

	return 0;
}

void MainMenu(HWND hWnd)
{
	hMenu = CreateMenu();
	HMENU hFileMenu = CreateMenu();
	HMENU hAboutMenu = CreateMenu();
	//Menu File:	
	AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
	AppendMenuW(hFileMenu, MF_STRING, MENU_SCAN, L"Scan...");
	AppendMenuW(hFileMenu, MF_STRING, MENU_SAVE, L"Save...");
	AppendMenuW(hFileMenu, MF_SEPARATOR, (UINT_PTR)NULL, NULL);
	AppendMenuW(hFileMenu, MF_STRING, MENU_EXIT, L"Exit");

	// Menu About:
	AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hAboutMenu, L"About");
	AppendMenuW(hAboutMenu, MF_STRING, MENU_ABOUT, L"Info");

	SetMenu(hWnd, hMenu);
}

// Create controls
void SetControls(HWND hWnd)
{
	CreateWindowW(L"Button", L"Scan", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 100, WND_WIDTH - 115, 120,
		30, hWnd, (HMENU)MENU_SCAN, NULL, NULL);

	CreateWindowW(L"button", L"Clear results", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 300, WND_WIDTH - 115, 120,
		30, hWnd, (HMENU)MENU_CLEAR_RESULTS, NULL, NULL);

	CreateWindowW(L"button", L"Save results", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 500, WND_WIDTH - 115, 120,
		30, hWnd, (HMENU)MENU_SAVE, NULL, NULL);


	CreateWindowW(L"Static", L"Duplicates:", WS_CHILD | WS_VISIBLE | WS_BORDER, 5, 5, WND_HEIGHT - 30,
	WND_WIDTH - 150, hWnd, NULL, NULL, NULL);

	// Adding a ListBoxes.
	liboDup = CreateWindowExW(WS_EX_CLIENTEDGE,
		L"LISTBOX",
		NULL,
		WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | WS_VSCROLL | WS_HSCROLL,
		5,
		25,
		WND_HEIGHT - 30,
		WND_WIDTH - 150,
		hWnd,
		NULL,
		my_hInst,
		NULL);

	SendMessageW(liboDup, LB_SETHORIZONTALEXTENT, (WPARAM)PATHMAXSIZE, (LPARAM)0);	
}

// Scan directiry for duplicates
void scanDir()
{
	//MessageBoxW(NULL, L"The program processes files only with names in Latin letters.", L"Info", MB_ICONINFORMATION | MB_OK);
	SendMessageW(liboDup, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
	
	char* path;

	path = getDirectory();
	SendMessageA(liboDup, LB_ADDSTRING, (WPARAM)0, (LPARAM)"Scanning in progress...");
	duplicates = findDup(path);
	if (duplicates != NULL)
	{
		viewResults(duplicates);
	}
	else
	{
		MessageBox(GetActiveWindow(), L"Directory selection error. Try again.",L"Error", MB_ICONERROR);
		SendMessageW(liboDup, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
	}
	free(path);
}

void startScanning()
{
	DWORD dwThreadId, dwThrdParam = 1;
	HANDLE hThread;
	hThread = CreateThread(NULL, 0, scanThread, &dwThrdParam, 0, &dwThreadId);
}

DWORD WINAPI scanThread(LPVOID lpParam)
{
	scanDir();
	return 0;
}

void viewResults()
{
	SendMessageW(liboDup, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
	if (duplicates->count == 0)
	{
		SendMessageA(liboDup, LB_ADDSTRING, (WPARAM)0, (LPARAM)"No duplicates found");
		return;
	}

	int i = 0;
	while (i < duplicates->count - 1)
	{
		if (duplicates->list[i].hash == duplicates->list[i + 1].hash)
		{
			int j = i;
			while (duplicates->list[j].hash == duplicates->list[i].hash)
			{
				SendMessageA(liboDup, LB_ADDSTRING, (WPARAM)0, (LPARAM)duplicates->list[j].name);
				j++;
			}
			SendMessageA(liboDup, LB_ADDSTRING, (WPARAM)0, (LPARAM)"----------------------------");
			i += j - i - 1;
		}
		i++;
	}
}

void saveResults()
{
	if (duplicates == NULL)
	{
		MessageBox(GetActiveWindow(), L"No information to save", L"Error", MB_ICONERROR);
		return;
	}

	MessageBoxW(NULL, L"Please, select a directiry whose name consists only of Latin letters.", L"Info", MB_ICONINFORMATION | MB_OK);
	char* path;
	path = getDirectory();


	if (!isDirExist(path))
	{
		MessageBox(GetActiveWindow(), L"Directory selection error. Try again.", L"Error", MB_ICONERROR);
		return;
	}
	
	outputToFile(duplicates, path);
	MessageBoxW(NULL, L"Information successfully saved.", L"Info", MB_ICONINFORMATION | MB_OK);
}

char* getDirectory()
{
	BROWSEINFOA bi;
	bi.hwndOwner = dirWnd;
	bi.iImage = 0;
	bi.lParam = 0;
	bi.lpfn = 0;
	bi.lpszTitle = 0;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = 0;
	bi.ulFlags;

	LPBROWSEINFO* lpbi;
	lpbi = &bi;

	PIDLIST_ABSOLUTE idList;
	idList = SHBrowseForFolderA(lpbi);
	WCHAR WPath[MAX_PATH];
	SHGetPathFromIDList(idList, WPath);

	char* path;
	path = (char*)malloc(MAX_PATH * sizeof(char));

	int i = 0;
	while (WPath[i] != '\0')
	{
		path[i] = (char)WPath[i];
		i++;
	}
	path[i] = '\0';

	return path;
}

void outputToFile(FILES* files, char* path)
{
	char fileName[20] = "//Duplicates.txt";
	strcat(path, fileName);

	FILE* f;
	f = fopen(path, "w");

	if (files->count == 0)
	{
		fputs("No duplicates found", f);
		fclose(f);
		return;
	}

	int i = 0;
	while (i < files->count - 1)
	{
		if (files->list[i].hash == files->list[i + 1].hash)
		{
			int j = i;
			while (files->list[j].hash == files->list[i].hash)
			{
				fputs(files->list[j].name, f);
				fputs("\n", f);
				j++;
			}
			fputs("---------------------------------------------", f);
			fputs("\n", f);
			i += j - i - 1;
		}
		i++;
	}
	fclose(f);
}
