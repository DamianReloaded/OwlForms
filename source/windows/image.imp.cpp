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

image::implementation::implementation () : m_width(0), m_height(0)
{

}

image::implementation::~implementation ()
{

}

void image::implementation::resize (const int& _width, const int& _height)
{
    m_width = _width;
    m_height = _height;


    size_t newsize = m_width*m_height*4;
    if (m_buffer.size()!=newsize) m_buffer.resize(newsize,0);

    clear();


    update();
}


void image::implementation::update ()
{

}

void image::implementation::clear ()
{
    for (size_t i=0; i<m_buffer.size(); i+=4)
    {
        (*((int*)&m_buffer[i])) = m_backcolor.get();
    }
}

void image::implementation::clear (const int& _r, const int& _g, const int& _b, const int& _a)
{
    m_backcolor.set(_r,_g,_b,_a);

    clear();
}
