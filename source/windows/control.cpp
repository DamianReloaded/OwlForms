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

#include "../control.h"
#include "control.imp.h"

using namespace owl::forms;

control::control (control* _parent)
{
    m_created           = false;
    m_parent            = _parent;
	m_imp               = new implementation();
	m_imp->m_control    = this;

    m_x                 = 0;
    m_y                 = 0;
    m_width             = 640;
    m_height            = 480;

    backcolor(212,208,200);
}

control::~control ()
{

	delete m_imp;
}

bool control::create (control* _parent)
{
    if (_parent!=NULL) m_parent = _parent;
    return m_imp->create();
}

void control::update()
{
    implementation::update();
}

void control::on_init ()
{

}

void control::on_paint ()
{

}

void control::on_destroy ()
{

}

const bool& control::created ()
{
    return m_created;
}

void control::parent (control* _parent)
{
    m_imp->set_parent(_parent);
}

void control::location (const int& _x, const int& _y)
{
    m_x = _x;
    m_y = _y;
    m_imp->move_size (m_x, m_y, m_width, m_height);
}

void control::size (const int& _width, const int& _height)
{
    m_width = _width;
    m_height = _height;
    m_imp->move_size (m_x, m_y, m_width, m_height);
}

void control::x (const int& _value)
{
    m_x = _value;
    m_imp->move_size (m_x, m_y, m_width, m_height);
}

void control::y (const int& _value)
{
    m_y = _value;
    m_imp->move_size (m_x, m_y, m_width, m_height);
}

void control::width (const int& _value)
{
    m_width = _value;
    m_imp->move_size (m_x, m_y, m_width, m_height);
}

void control::height (const int& _value)
{
    m_height = _value;
    m_imp->move_size (m_x, m_y, m_width, m_height);
}

const int& control::x ()
{
    return m_x;
}

const int& control::y ()
{
    return m_y;
}

const int& control::width ()
{
    return m_width;
}

const int& control::height ()
{
    return m_height;
}

void control::backcolor (const int& _red, const int& _green, const int& _blue, const int& _alpha)
{
    m_imp->backcolor(_red, _green, _blue, _alpha);
}

void control::refresh ()
{
    UpdateWindow(m_imp->m_hwnd);
    //on_paint();
}

void control::draw (image* _source, const int& _xdest, const int& _ydest)
{
    m_imp->m_backbuffer.draw(_source, _xdest, _ydest);
    refresh();
}
