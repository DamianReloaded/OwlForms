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

#include "../image.h"
#include "image.imp.h"
#include "control.imp.h"
#include "../dep/lodepng.h"

using namespace owl::forms;

image::image	()
{
	imp = new implementation();
}

image::~image	()
{

	delete imp;
}

void image::resize (const int& _width, const int& _height)
{
    imp->resize(_width, _height);
}


bool image::load (const std::string& _filepath)
{
    unsigned width, height;
    std::vector<unsigned char>  buffer;
    unsigned error = lodepng::decode(buffer, width, height, _filepath);
    if (error != 0) return false;

    imp->m_width = width;
    imp->m_height = height;

    imp->m_buffer = buffer;


    imp->update();

    return true;
}

void image::clear (const int& _r, const int& _g, const int& _b, const int& _a)
{
    imp->clear(_r, _g, _b, _a);
}

void image::clear ()
{
    imp->clear();
}

const color& image::backcolor ()
{
    return imp->m_backcolor;
}

const int image::width ()
{
    return imp->m_width;
}

const int image::height ()
{
    return imp->m_height;
}

void image::draw (image* _source, const int& _xdest, const int& _ydest)
{
    if (_xdest>=width()) return;
    if (_ydest>=height()) return;
    if (_xdest+_source->width()<=0) return;
    if (_ydest+_source->height()<=0) return;

    int xs1 = 0, ys1 = 0, xs2 = 0, ys2 = 0;
    int xd1 = 0, yd1 = 0, xd2 = 0, yd2 = 0;

    if(_xdest<0) { xd1 = 0; xd2 = _source->width()+_xdest; xs1=-_xdest; xs2=_source->width()+_xdest;}
    else { xd1 = _xdest; xd2 = _xdest+_source->width(); xs1=0; xs2=_source->width();}

    if(_ydest<0) { yd1 = 0; yd2 = _source->height()+_ydest; ys1=-_ydest; ys2=_source->height()+_ydest;}
    else { yd1 = _ydest; yd2 = _ydest+_source->height(); ys1=0; ys2=_source->height();}

    int tmpx2 = _xdest+_source->width();
    if (tmpx2>width()) {xd1=_xdest; xd2=width(); xs1=0; xs2=_source->width()-(tmpx2-width()); }
    int tmpy2 = _ydest+_source->height();
    if (tmpy2>height()) {yd1=_ydest; yd2=height(); ys1=0; ys2=_source->height()-(tmpy2-height()); }


    for (int yd=yd1, ys=ys1; yd<yd2; yd++, ys++)
    {
        for (int xd=xd1, xs=xs1; xd<xd2; xd++, xs++)
        {
            int pixs = (xs+(ys*_source->width()))*4;
            int pixd = (xd+(yd*width()))*4;
            imp->m_buffer[pixd+0] = _source->imp->m_buffer[pixs+0];
            imp->m_buffer[pixd+1] = _source->imp->m_buffer[pixs+1];
            imp->m_buffer[pixd+2] = _source->imp->m_buffer[pixs+2];
            imp->m_buffer[pixd+3] = _source->imp->m_buffer[pixs+3];
        }
    }

    imp->update();
}
