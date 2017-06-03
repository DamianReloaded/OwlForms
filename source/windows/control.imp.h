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

#ifndef owl_forms_control_implementation_h
#define owl_forms_control_implementation_h

#include "../control.h"
#include "image.imp.h"
#include "bmputil.h"

namespace owl {
namespace forms {

class control::implementation
{
	public:
                                        implementation		();
		virtual						    ~implementation		();

        bool                            create              ();
        void                            destroy             ();

		static void                     update              ();

        void                            set_window_ptr      ();
        static implementation*          get_window_ptr      (const HWND& _hwnd);

        void                            set_parent          (control* _parent);
        void                            move_size           (const int& _x, const int& _y, const int& _width, const int& _height);

        void                            set_title           (const std::string& _title);

        void                            backcolor           (const int& _red, const int& _green, const int& _blue, const int& _alpha=255);
        void                            backcolor_reset     ();

        static LRESULT CALLBACK         wndproc             (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

        control*                        m_control;

        image                           m_backbuffer;

        HINSTANCE				        m_hinstance;
		HWND					        m_hwnd;
		HBRUSH					        m_background_brush;

		bmputil*				        m_bmpu;
};

}}

#endif // owl_forms_control_implementation_h
