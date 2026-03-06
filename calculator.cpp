#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

HWND hEdit1, hEdit2;
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		
		case WM_CREATE: {
            CreateWindow("STATIC", "Please input two numbers", 
                         WS_VISIBLE | WS_CHILD | SS_CENTER,
                         25, 20, 180, 20, hwnd, NULL, NULL, NULL);

            hEdit1 = CreateWindow("EDIT", "", 
                                  WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                                  25, 50, 180, 20, hwnd, NULL, NULL, NULL);

            hEdit2 = CreateWindow("EDIT", "", 
                                  WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                                  25, 80, 180, 20, hwnd, NULL, NULL, NULL);

            CreateWindow("BUTTON", "+", WS_VISIBLE | WS_CHILD,
                         45, 120, 25, 25, hwnd, (HMENU)1, NULL, NULL);
            CreateWindow("BUTTON", "-", WS_VISIBLE | WS_CHILD,
                         80, 120, 25, 25, hwnd, (HMENU)2, NULL, NULL);
            CreateWindow("BUTTON", "*", WS_VISIBLE | WS_CHILD,
                         115, 120, 25, 25, hwnd, (HMENU)3, NULL, NULL);
            CreateWindow("BUTTON", "/", WS_VISIBLE | WS_CHILD,
                         150, 120, 25, 25, hwnd, (HMENU)4, NULL, NULL);
            break;
        }

        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            
            if (wmId >= 1 && wmId <= 4) {
                char buf1[256], buf2[256];
                
                GetWindowText(hEdit1, buf1, 256);
                GetWindowText(hEdit2, buf2, 256);

                double num1 = atof(buf1);
                double num2 = atof(buf2);
                double result = 0.0;

                switch (wmId) {
                    case 1: result = num1 + num2; break;
                    case 2: result = num1 - num2; break;
                    case 3: result = num1 * num2; break;
                    case 4: 
                        if (num2 == 0) {
                            MessageBox(hwnd, "Cannot divide by zero!", "Error", MB_OK | MB_ICONERROR);
                            return 0;
                        }
                        result = num1 / num2; 
                        break;
                }

                char t[100];
                sprintf(t, "%f", result);
                
                MessageBox(hwnd, t, "Result", MB_OK);
            }
            break;
        }

        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wParam;
            SetTextColor(hdcStatic, RGB(0, 0, 0));
            SetBkColor(hdcStatic, RGB(255, 255, 255));
            return (LRESULT)GetStockObject(WHITE_BRUSH);
        }
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize	 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor	 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = CreateSolidBrush(RGB(42, 52, 204));
	wc.lpszClassName = "WindowClass";
	wc.hIcon	 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm	 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","My Calculator",WS_VISIBLE|WS_CAPTION |WS_SYSMENU,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		250, /* width */
		200, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}
