#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <gdiplus.h>
#include <cmath>
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

const int POINTSIZE = 3;
bool isCreated = false;

Image* img = nullptr;
HINSTANCE hInstanceGlobal;

float cx = 300.0f;
float cy = 300.0f;

float width = 100.0f;
float height = 100.0f;

Point points[POINTSIZE];

Point basePoints[POINTSIZE] = {
    Point(-width / 2, -height / 2),
    Point(width / 2, -height / 2),
    Point(-width / 2, height / 2)
};

float angle = 0.0f;
const float ROTATEANGLE = 5.0f;
HCURSOR hCurs3; // cursor handle 

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void UpdatePoints()
{
    float angleRad = angle * 3.14159265f / 180.0f;

    // x_new = cx + (x' - cx) * cos(angleRad) - (y' - cy) * sin(angleRad)
    // y_new = cy + (x' - cx) * sin(angleRad) + (y' - cy) * cos(angleRad)

    for (int i = 0; i < POINTSIZE; i++)
    {
        float dx = basePoints[i].X;
        float dy = basePoints[i].Y;
        points[i].X = cx + dx * cos(angleRad) - dy * sin(angleRad);
        points[i].Y = cy + dx * sin(angleRad) + dy * cos(angleRad);
    }
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
    HINSTANCE hinst = hInstance; // handle to current instance  
    HCURSOR hCurs1, hCurs2; // cursor handles 


    // Yin-shaped cursor AND mask (32x32x1bpp)
    BYTE ANDmaskCursor[] =
    {
        0xFF, 0xFC, 0x3F, 0xFF, // ##############----##############
        0xFF, 0xC0, 0x1F, 0xFF, // ##########---------#############
        0xFF, 0x00, 0x3F, 0xFF, // ########----------##############
        0xFE, 0x00, 0xFF, 0xFF, // #######---------################
        0xF8, 0x01, 0xFF, 0xFF, // #####----------#################
        0xF0, 0x03, 0xFF, 0xFF, // ####----------##################
        0xF0, 0x03, 0xFF, 0xFF, // ####----------##################
        0xE0, 0x07, 0xFF, 0xFF, // ###----------###################
        0xC0, 0x07, 0xFF, 0xFF, // ##-----------###################
        0xC0, 0x0F, 0xFF, 0xFF, // ##----------####################
        0x80, 0x0F, 0xFF, 0xFF, // #-----------####################
        0x80, 0x0F, 0xFF, 0xFF, // #-----------####################
        0x80, 0x07, 0xFF, 0xFF, // #------------###################
        0x00, 0x07, 0xFF, 0xFF, // -------------###################
        0x00, 0x03, 0xFF, 0xFF, // --------------##################
        0x00, 0x00, 0xFF, 0xFF, // ----------------################
        0x00, 0x00, 0x7F, 0xFF, // -----------------###############
        0x00, 0x00, 0x1F, 0xFF, // -------------------#############
        0x00, 0x00, 0x0F, 0xFF, // --------------------############
        0x80, 0x00, 0x0F, 0xFF, // #-------------------############
        0x80, 0x00, 0x07, 0xFF, // #--------------------###########
        0x80, 0x00, 0x07, 0xFF, // #--------------------###########
        0xC0, 0x00, 0x07, 0xFF, // ##-------------------###########
        0xC0, 0x00, 0x0F, 0xFF, // ##------------------############
        0xE0, 0x00, 0x0F, 0xFF, // ###-----------------############
        0xF0, 0x00, 0x1F, 0xFF, // ####---------------#############
        0xF0, 0x00, 0x1F, 0xFF, // ####---------------#############
        0xF8, 0x00, 0x3F, 0xFF, // #####-------------##############
        0xFE, 0x00, 0x7F, 0xFF, // #######----------###############
        0xFF, 0x00, 0xFF, 0xFF, // ########--------################
        0xFF, 0xC3, 0xFF, 0xFF, // ##########----##################
        0xFF, 0xFF, 0xFF, 0xFF // ################################
    };

    // Yin-shaped cursor XOR mask (32x32x1bpp)
    BYTE XORmaskCursor[] =
    {
        0x00, 0x00, 0x00, 0x00, // --------------------------------
        0x00, 0x03, 0xC0, 0x00, // --------------####--------------
        0x00, 0x3F, 0x00, 0x00, // ----------######----------------
        0x00, 0xFE, 0x00, 0x00, // --------#######-----------------
        0x03, 0xFC, 0x00, 0x00, // ------########------------------
        0x07, 0xF8, 0x00, 0x00, // -----########-------------------
        0x07, 0xF8, 0x00, 0x00, // -----########-------------------
        0x0F, 0xF0, 0x00, 0x00, // ----########--------------------
        0x1F, 0xF0, 0x00, 0x00, // ---#########--------------------
        0x1F, 0xE0, 0x00, 0x00, // ---########---------------------
        0x3F, 0xE0, 0x00, 0x00, // --#########---------------------
        0x3F, 0xE0, 0x11, 0x00, // --#########---------------------
        0x3F, 0xF0, 0x11, 0x00, // --##########--------------------
        0x7F, 0xF0, 0x22, 0x00, // -###########--------------------
        0x7F, 0xF8, 0x22, 0x00, // -############-------------------
        0x7F, 0xFC, 0x22, 0x00, // -#############------------------
        0x7F, 0xFF, 0x22, 0x00, // -###############----------------
        0x7F, 0xFF, 0x80, 0x00, // -################---------------
        0x7F, 0xFF, 0xE0, 0x00, // -##################-------------
        0x3F, 0xFF, 0xE0, 0x00, // --#################-------------
        0x3F, 0xC7, 0xF0, 0x00, // --########----######------------
        0x3F, 0x83, 0xF0, 0x00, // --#######------#####------------
        0x1F, 0x83, 0xF0, 0x00, // ---######------#####------------
        0x1F, 0x83, 0xE0, 0x00, // ---######------####-------------
        0x0F, 0xC7, 0xE0, 0x00, // ----######----#####-------------
        0x07, 0xFF, 0xC0, 0x00, // -----#############--------------
        0x07, 0xFF, 0xC0, 0x00, // -----#############--------------
        0x01, 0xFF, 0x80, 0x00, // -------##########---------------
        0x00, 0xFF, 0x00, 0x00, // --------########----------------
        0x00, 0x3C, 0x00, 0x00, // ----------####------------------
        0x00, 0x00, 0x00, 0x00, // --------------------------------
        0x00, 0x00, 0x00, 0x00 // --------------------------------
    };

    // Create a custom cursor at run time. 

    hCurs3 = CreateCursor(hinst, // app. instance 
                          19, // horizontal position of hot spot 
                          2, // vertical position of hot spot 
                          32, // cursor width 
                          32, // cursor height 
                          ANDmaskCursor, // AND mask 
                          XORmaskCursor); // XOR mask 

    hInstanceGlobal = hInstance;

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"Sample Window Class";

    // wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hCursor = hCurs3;    

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        L"Sample Window Class",
        L"Rotate & Move PNG",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL
    );


    if (!hwnd) return 0;

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (img) delete img;
    GdiplusShutdown(gdiplusToken);
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            HDC memDC = CreateCompatibleDC(hdc);

            RECT rc;
            GetClientRect(hwnd, &rc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);

            HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

            Graphics gr(memDC);

            gr.Clear(Color::Green);

            if (img && isCreated)
            {
                UpdatePoints();
                gr.DrawImage(img, points, POINTSIZE);
            }

            BitBlt(hdc, 0, 0, rc.right - rc.left + 100, rc.bottom - rc.top + 100, memDC, 0, 0, SRCCOPY);

            SelectObject(memDC, oldBitmap);
            DeleteObject(memBitmap);
            DeleteDC(memDC);

            EndPaint(hwnd, &ps);
            break;
        }

    case WM_KEYDOWN:
        {
            if ((GetKeyState(VK_CONTROL) & 0x8000) && wParam == 'Z') // Ctrl + Z
            {
                PostQuitMessage(0); // закрытие приложения
                return 0;
            }

            if (wParam == 'C') // Создание картинки
            {
                if (!isCreated)
                {
                    img = new Image(L"D:\\study\\5 semestr\\SP\\lab1\\images\\cubes.png");
                    isCreated = true;
                }
            }
            else if (wParam == VK_UP || wParam == 'W') cy -= 3.0f;
            else if (wParam == VK_DOWN || wParam == 'S') cy += 3.0f;
            else if (wParam == VK_LEFT || wParam == 'A') cx -= 3.0f;
            else if (wParam == VK_RIGHT || wParam == 'D') cx += 3.0f;
            else if (wParam == 'E') angle = fmod(angle + ROTATEANGLE, 360.0f);
            else if (wParam == 'Q') angle = fmod(angle - ROTATEANGLE, 360.0f);
            else if (wParam == 'R') // Сброс позиции и угла
            {
                cx = 300.0f;
                cy = 300.0f;
                angle = 0.0f;
            }

            InvalidateRect(hwnd, nullptr, TRUE);
            return 0;
        }
    case WM_MOUSEWHEEL:
        {
            auto delta = GET_WHEEL_DELTA_WPARAM(wParam);
            if (delta > 0)
            {
                //MK_SHIFT
                if (wParam & 0x0004)
                    cx++;
                else
                    cx--;

                cy--;
            }
            else
            {
                //MK_SHIFT
                if (wParam & 0x0004)
                    cx--;
                else
                    cx++;

                cy++;
            }

            InvalidateRect(hwnd, nullptr, TRUE);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
