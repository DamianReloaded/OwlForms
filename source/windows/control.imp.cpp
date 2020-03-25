/*
OwlForms

Copyright (c) 2005-2016 Damian Reloaded

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/

#include "control.imp.h"
#include "image.imp.h"
#include "../control.h"
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <vector>

using namespace owl::forms;

control::implementation::implementation ()
{
    m_hwnd = 0;
    m_hinstance = 0;
    m_background_brush = 0;
    m_bmpu = new bmputil();
}

control::implementation::~implementation ()
{
    destroy();
    delete m_bmpu;
}

void control::implementation::destroy ()
{
    if (m_hwnd==0) return;
	DeleteObject(m_background_brush);
	DestroyWindow(m_hwnd);
	m_bmpu->free_buffer(0);
    m_hwnd = 0;
}

LRESULT CALLBACK control::implementation::wndproc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	control::implementation* imp = (control::implementation*)GetWindowLongPtr(hWnd, GWLP_USERDATA );
	if (imp == NULL) return DefWindowProc(hWnd, message, wParam, lParam);

	switch (message)
	{
	case WM_CREATE:
		{


		}
		break;
	//will be called periodically, if SetTimer function is invoked.
    //In our case the WM_TIMER event occurs every 15 seconds see (A)
	case WM_PAINT:
		{
			imp->m_control->on_paint();

            HDC dc = imp->m_bmpu->get_dc_buffer(0);

            BITMAPINFO info;
            ZeroMemory(&info, sizeof(BITMAPINFO));
            info.bmiHeader.biBitCount = 32;
            info.bmiHeader.biWidth =  imp->m_backbuffer.width();
            info.bmiHeader.biHeight = imp->m_backbuffer.height();
            info.bmiHeader.biPlanes = 1;
            info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            info.bmiHeader.biSizeImage = imp->m_backbuffer.imp->m_buffer.size();
            info.bmiHeader.biCompression = BI_RGB;

            StretchDIBits(dc, 0, 0, imp->m_backbuffer.width(), imp->m_backbuffer.height(), 0, 0, imp->m_backbuffer.width(), imp->m_backbuffer.height(), &imp->m_backbuffer.imp->m_buffer[0], &info, DIB_RGB_COLORS, SRCCOPY);

            RECT rect;
            rect.left = 0;
            rect.top = 0;
            rect.right = imp->m_backbuffer.width();
            rect.bottom = imp->m_backbuffer.height();
            InvalidateRect(hWnd, &rect, false);

			HDC hdc = BeginPaint(hWnd, &ps);
			imp->m_bmpu->copy_to_screen(0);
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		{
			bool cancel = false;
			imp->m_control->on_destroy();
			if (cancel) break;
			PostQuitMessage(0);
			imp->destroy();
		}
		break;
	case WM_SIZE:
		{
			imp->m_control->m_width = LOWORD(lParam);
			imp->m_control->m_height = HIWORD(lParam);
			imp->m_backbuffer.resize(imp->m_control->width(), imp->m_control->height());
			imp->m_bmpu->resize(hWnd);
		}
		break;
    case WM_ERASEBKGND:
        {
            // This flickers as fuck
            /*
            RECT rc;
            HDC hdc = (HDC) wParam;
            GetClientRect(hWnd, &rc);
            FillRect(hdc, &rc, imp->m_background_brush);*/
        }
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

bool control::implementation::create ()
{
    if (m_control->m_created) return m_control->m_created;

    m_backbuffer.resize(m_control->width(), m_control->height());

	m_hinstance = GetModuleHandle(NULL);
    std::wstring classname;
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	if (m_control->m_parent!=NULL)
    {
        classname           = L"control_class";
    }
    else
    {
        classname           = L"window_class";
    }

    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= (WNDPROC)control::implementation::wndproc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= m_hinstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= classname.c_str();
	wcex.hIconSm		= NULL;

	if (!RegisterClassEx(&wcex))
    {
        std::string str = "Possibly class already registered.";
    }

   	if (m_control->m_parent!=NULL)
    {
        m_hwnd = CreateWindow(classname.c_str(), NULL, WS_VISIBLE | WS_CHILD,
        m_control->m_x, m_control->m_y, m_control->m_width, m_control->m_height, m_control->m_parent->imp->m_hwnd, NULL, m_hinstance, NULL);
    }
    else
    {
        m_hwnd = CreateWindow(classname.c_str(), L"window title", WS_OVERLAPPEDWINDOW,
        m_control->m_x, m_control->m_y, m_control->m_width, m_control->m_height, NULL, NULL, m_hinstance, NULL);
    }

	if (!m_hwnd)
	{
        return FALSE;
	}

	SetWindowLongPtr( m_hwnd, GWLP_USERDATA, (LONG_PTR)this);

	backcolor(m_backbuffer.backcolor().r(), m_backbuffer.backcolor().g(), m_backbuffer.backcolor().b());

    m_bmpu->initialize_buffers(m_hwnd,1);
	m_bmpu->create_buffer(0);//(B)

	ShowWindow(m_hwnd, 1);
	UpdateWindow(m_hwnd);

	m_control->on_init();

    set_window_ptr();

    m_control->m_created = true;

    return m_control->m_created;
}

void control::implementation::update()
{
	MSG msg;

	while (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void control::implementation::set_window_ptr ()
{
    SetWindowLongPtr( m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
}

control::implementation* control::implementation::get_window_ptr (const HWND& _hwnd)
{
    return (control::implementation*)GetWindowLongPtr(_hwnd, GWLP_USERDATA);
}

void control::implementation::set_parent (control* _parent)
{
    m_control->m_parent = _parent;

    if (_parent!=NULL && _parent->imp->m_hwnd != 0)
    {

    }
}

void control::implementation::move_size (const int& _x, const int& _y, const int& _width, const int& _height)
{
    m_control->m_x = _x;
    m_control->m_y = _y;
    m_control->m_width = _width;
    m_control->m_height = _height;

    if (m_hwnd==0) return;

SetWindowPos(
  m_hwnd,
  NULL,
  m_control->m_x,
  m_control->m_y,
  m_control->m_x+m_control->m_width,
  m_control->m_y+m_control->m_height,
  NULL
);
}

void control::implementation::set_title (const std::string& _title)
{

}

void control::implementation::backcolor (const int& _red, const int& _green, const int& _blue, const int& _alpha)
{
    m_backbuffer.clear(_red, _green, _blue, _alpha);
    if (m_background_brush!=0) DeleteObject(m_background_brush);
    m_background_brush = CreateSolidBrush(RGB(_red,_green,_blue));
    //move_size(m_control->x(),m_control->y(),m_control->width(), m_control->height());
}

void control::implementation::backcolor_reset ()
{
    m_backbuffer.clear();
    if (m_background_brush!=0) DeleteObject(m_background_brush);
    m_background_brush = CreateSolidBrush(RGB(m_backbuffer.backcolor().r(),m_backbuffer.backcolor().g(),m_backbuffer.backcolor().b()));
    //move_size(m_control->x(),m_control->y(),m_control->width(), m_control->height());
}
