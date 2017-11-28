// PaintingBorad.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "PaintingBorad.h"
#include "ShapeClass.h"

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PAINTINGBORAD, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAINTINGBORAD));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAINTINGBORAD));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PAINTINGBORAD);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

POINT pos1_before_move,pos2_before_move;	//移动前主对角线两端点
bool bIsOnShape = false, one_click = false;
Base_shape *head = NULL, *tmp, *cursor;
Shape shape;
//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	static POINT init_pos, current_pos, shift_amount;
	
    switch (message)
    {
	case WM_CREATE:
		
		static HWND hStatic_x = CreateWindow(L"static", L"X:",
			WS_CHILD | WS_VISIBLE | WS_BORDER | SS_LEFT | SS_CENTERIMAGE,
			0, 0, 60, 15, hWnd, (HMENU)1, hInst, NULL);//X坐标静态文本框
		static HWND hStatic_y = CreateWindow(L"static", L"Y:",
			WS_CHILD | WS_VISIBLE | WS_BORDER | SS_LEFT | SS_CENTERIMAGE,
			0, 20, 60, 15, hWnd, (HMENU)2, hInst, NULL);//Y坐标静态文本框

		break;
	case WM_LBUTTONUP: {
		bIsOnShape = false;
		one_click = false;
		break;
	}

	case WM_LBUTTONDOWN: {
		init_pos = TranslatePos(lParam);
		
		//插入新图形到链表中
		if (!head) {
			switch (shape) {
			case LINE_:
				head = new Line(lParam);
				break;
			case RECT_:
				head = new Rect(lParam);
				break;
			case CIRCLE_:
				head = new Circle(lParam);
				break;
			}
		}
		else {	
			//打印鼠标指针位置
			wchar_t buf[20];
			wsprintf(buf, L"X:%ld", init_pos.x);
			SetWindowText(hStatic_x, buf);
			wsprintf(buf, L"Y:%ld", init_pos.y);
			SetWindowText(hStatic_y, buf);

			//鼠标左键点击时，遍历
			cursor = head;
			while (cursor && !(bIsOnShape = cursor->isAboveShape(TranslatePos(lParam))))
				cursor = cursor->Next;

			//如果没点在图形上，则bIsOnShape为false,此时则插入图形
			if (!bIsOnShape) {
				tmp = head;
				switch (shape) {
				case LINE_:
					head = new Line(lParam);
					break;
				case RECT_:
					head = new Rect(lParam);
					break;
				case CIRCLE_:
					head = new Circle(lParam);
					break;
				}
				head->Next = tmp;
			}
		}

		break;
	}
		
	case WM_MOUSEMOVE:
		{
		if (wParam & MK_LBUTTON) {
			if (bIsOnShape) {
				//如果点在图形上，则移动图形

				//这个if的作用是在一次移动中只获取一次移动前位置
				if (!one_click) {
					cursor->GetPos(pos1_before_move, pos2_before_move);
					one_click = true;
				}

				//计算偏移量
				current_pos = TranslatePos(lParam);
				shift_amount.x = current_pos.x - init_pos.x;
				shift_amount.y = current_pos.y - init_pos.y;

				cursor->SetShiftAmount(shift_amount, pos1_before_move,pos2_before_move);
			}
			else
				//如果没点在图形上，则在绘制图形中（因为点击操作只有两种情况，绘图和移动）
				//在绘制图形中，则下面的函数是随鼠标移动不断更改第二点坐标（第一点由第一次鼠标左击确定）
				head->SetLastPos(lParam);	
			
			//强制重绘界面
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_ERASE);
			}
		}
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		//如果图形链表非空，遍历链表，执行每个图形对应的绘图函数
		if (head){
			Base_shape *tmp_paint;
			tmp_paint = head;
			do{
				tmp_paint->Paint(hdc);
			} while (tmp_paint = tmp_paint->Next);
		}

		EndPaint(hWnd, &ps);
	}
	break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {

			//----------------------切换图形----------------------
			case SHAPE_LINE:
				shape = LINE_;
				break;
			case SHAPE_CIRCLE:
				shape = CIRCLE_;
				break;
			case SHAPE_RECT:
				shape = RECT_;
				break;
			//----------------------------------------------------

			case IDM_OPEN: {
				
				OpenPaintingBoard(hWnd, head);

				//强制重绘界面
				RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_ERASE);
				break;
			}

			case IDM_SAVE:
				SavePaitingBoard(hWnd, head);
				break;

            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
