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

Display* control::implementation::m_xlib_display = 0;
Atom     control::implementation::m_xlib_atom = 0;

control::implementation::implementation ()
{
    m_xlib_window = 0;
}

control::implementation::~implementation ()
{
    destroy();
}

void control::implementation::destroy ()
{
    if (m_xlib_window==0) return;
    XFreeGC(m_xlib_display, m_xlib_gc);
    XDestroyWindow(m_xlib_display, m_xlib_window);
    m_xlib_gc = 0;
    m_xlib_window = 0;
}

int error_handler(Display *, XErrorEvent* ev)
{
    std::cout << "XLib Error: " << ev->error_code << std::endl;
    return 0;
}

bool control::implementation::create ()
{
    if (m_control->m_created) return m_control->m_created;

    if (m_xlib_display==NULL)
    {
        m_xlib_display = XOpenDisplay(NULL);
        XSetErrorHandler(&error_handler);
    }
    if (m_xlib_display==NULL) return m_control->m_created;

    m_xlib_window = XCreateSimpleWindow(m_xlib_display, (m_control->m_parent!=NULL)?m_control->m_parent->m_imp->m_xlib_window : DefaultRootWindow(m_xlib_display), m_control->x(), m_control->y(),  m_control->width(), m_control->height(), 0, 0, m_backbuffer.backcolor().get());

    m_backbuffer.resize(m_control->width(), m_control->height());

    m_xlib_gc = XCreateGC(m_xlib_display, m_backbuffer.m_imp->m_pixmap, 0, NULL);

    XSelectInput(m_xlib_display, m_xlib_window, StructureNotifyMask | ButtonPressMask | KeyPressMask | StructureNotifyMask | ExposureMask);

    XMapWindow(m_xlib_display, m_xlib_window);

    XMoveWindow(m_xlib_display, m_xlib_window, m_control->x(), m_control->y());

    set_window_ptr();

    m_control->m_created = true;

    return m_control->m_created;
}

void control::implementation::update()
{
    XEvent               event;

    do
    {
        XNextEvent(m_xlib_display, &event);
        control::implementation* w = get_window_ptr(event.xany.window);
        if (!w->m_control->m_created) return;

        switch (event.type)
        {
            case DestroyNotify:
            {

                w->m_control->on_destroy();
                w->m_control->m_created = false;
                break;
            }

            case KeyPress:
            {
                KeySym     keysym;
                XKeyEvent *kevent;
                char       buffer[1];
                /* It is necessary to convert the keycode to a
                * keysym before checking if it is an escape */
                kevent = (XKeyEvent *) &event;
                if (   (XLookupString((XKeyEvent *)&event,buffer,1,&keysym,NULL) == 1) && (keysym == (KeySym)XK_Escape) )
                {
                    // notify the window

                }

                break;
            }

            case ButtonPress:
            {
                switch (event.xbutton.button)
                {
                case 1:
                break;
                case 2:
                break;
                case 3:
                break;
                }

                break;
            }

            case ConfigureNotify:
            {
                w->m_control->m_width = event.xconfigure.width;
                w->m_control->m_height = event.xconfigure.height;
                w->m_backbuffer.resize(w->m_control->m_width, w->m_control->m_height);

                //event.xconfigure.width, event.xconfigure.height
                /* fall through... */
            }

            case Expose:
            {
                w->m_control->refresh();
            }
            break;
        }

    } while(XPending(m_xlib_display)); /* loop to compress events */
}

void control::implementation::set_window_ptr ()
{
    if (0 == m_xlib_atom)
    {
        m_xlib_atom = XInternAtom(m_xlib_display, "owl_forms_control_ptr", False);
        if (m_xlib_atom == None)
        {
              //printf("### failed to create atom with name PERSONAL_PROPERTY\n");
              return;
        }

    }

    std::stringstream ss;
    ss<<(unsigned long)this;
    std::string str_window_ptr;
    ss >> str_window_ptr;

    XChangeProperty(m_xlib_display,   /* connection to x server */
                     m_xlib_window,    /* window whose property we want to change */
                     m_xlib_atom,   /* property name */
                     XA_STRING, /* type of property */
                     8,         /* format of prop; can be 8, 16, 32 */
                     PropModeReplace,
                     (unsigned char*)str_window_ptr.c_str(), /* actual data */
                     64         /* number of elements */
                    );

}

control::implementation* control::implementation::get_window_ptr (const Window& _xlib_window)
{
    Atom            da;
    int             di;
    unsigned char*  prop_ret = NULL;
    unsigned long   dl;

    int status = XGetWindowProperty(m_xlib_display, _xlib_window, m_xlib_atom, 0L, 64, False,
                                AnyPropertyType, &da, &di, &dl, &dl, &prop_ret);

    if (status == Success && prop_ret)
    {
        std::string str = (char*)prop_ret;
        std::stringstream ss2;
        ss2 << str;
        unsigned long nthis;
        ss2 >> nthis;
        control::implementation* timp = (control::implementation*)nthis;
        return timp;
    }

    return NULL;
}

void control::implementation::set_parent (control* _parent)
{
    m_control->m_parent = _parent;

    if (_parent!=NULL && _parent->m_imp->m_xlib_window != 0)
    {
        XUnmapWindow(m_xlib_display, m_xlib_window);
        XReparentWindow(m_xlib_display, m_xlib_window, m_control->m_parent->m_imp->m_xlib_window, 0, 0);
        XMapWindow(m_xlib_display, m_xlib_window);
    }
}

void control::implementation::move_size (const int& _x, const int& _y, const int& _width, const int& _height)
{
    if (m_xlib_window==0) return;
    XWindowChanges wc;
    wc.x = _x;
    wc.y = _y;
    wc.width = _width;
    wc.height = _height;
    XConfigureWindow(m_xlib_display, m_xlib_window, CWX | CWY | CWWidth | CWHeight, &wc);
}

void control::implementation::set_title (const std::string& _title)
{
    /* This variable will store the newly created property. */
    XTextProperty window_title_property;

    /* This is the string to be translated into a property. */
    char* window_title = (char*)_title.c_str();

    /* translate the given string into an X property. */
    XStringListToTextProperty(&window_title, 1, &window_title_property);

    /* assume that window_title_property is our XTextProperty, and is */
    /* defined to contain the desired window title.                   */
    XSetWMName(m_xlib_display, m_xlib_window, &window_title_property);

    //in order to set the name of the iconized version of our window, we will use the XSetWMIconName() function in a similar way:
    /* this time we assume that icon_name_property is an initialized */
    /* XTextProperty variable containing the desired icon name.      */
    //XSetWMIconName(display, win, &icon_name_property);
}
