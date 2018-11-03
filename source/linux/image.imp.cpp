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

#include "image.imp.h"
#include "../image.h"
#include "../control.h"
#include "control.imp.h"

using namespace owl::forms;

image::implementation::implementation () : m_pixmap(0), m_ximage(0), m_width(0), m_height(0)
{
    m_buffer = nullptr;
    m_buffer_size = 0;
}

image::implementation::~implementation ()
{
    if (m_pixmap!=0) XFreePixmap(control::implementation::m_xlib_display, m_pixmap);
    if (m_ximage!=0) XDestroyImage(m_ximage);
}

void image::implementation::resize (const int& _width, const int& _height)
{
    if (m_width == _width && m_height == _height) return;

    m_width = _width;
    m_height = _height;

    if (m_pixmap!=0) XFreePixmap(control::implementation::m_xlib_display, m_pixmap);

    int depth = 24; // works fine with depth = 24
    int bitmap_pad = 32; // 32 for 24 and 32 bpp, 16, for 15&16
    int bytes_per_line = 0; // number of bytes in the client image between the start of one scanline and the start of the next
    Display *display=control::implementation::m_xlib_display;

    size_t newsize = m_width*m_height*4;
    if (m_buffer_size!=newsize)
    {
        m_buffer_size = newsize;
        delete [] m_buffer;
        m_buffer = new unsigned char[m_buffer_size];
    }

    clear();

    m_ximage = XCreateImage(display, CopyFromParent, depth, ZPixmap, 0, (char*)&m_buffer[0], m_width, m_height, bitmap_pad, bytes_per_line);
    m_pixmap = XCreatePixmap(display, XDefaultRootWindow(display), m_width, m_height, depth);

    update();
}


void image::implementation::update ()
{
    int depth = 24;
    int bitmap_pad = 32; // 32 for 24 and 32 bpp, 16, for 15&16
    int bytes_per_line = 0; // number of bytes in the client image between the start of one scanline and the start of the next
    Display *display=control::implementation::m_xlib_display;
    GC gc = XCreateGC(control::implementation::m_xlib_display, m_pixmap, 0, NULL);
    XPutImage(control::implementation::m_xlib_display, m_pixmap, gc, m_ximage, 0,0,0,0, m_width, m_height);
    XFreeGC(control::implementation::m_xlib_display, gc);
}

void image::implementation::clear ()
{
    for (size_t i=0; i<m_buffer_size; i+=4)
    {
        (*((int*)&m_buffer[i])) = m_backcolor.get();
    }
}

void image::implementation::clear (const int& _r, const int& _g, const int& _b, const int& _a)
{
    m_backcolor.set(_r,_g,_b,_a);

    clear();
}
