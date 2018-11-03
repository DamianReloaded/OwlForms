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

#ifndef owl_forms_control_h
#define owl_forms_control_h

#include "image.h"
#include <iostream>

namespace owl {
namespace forms {

class control
{
	public:
									control		    (control* _parent=NULL);
		virtual						~control		();

        bool                        create          (control* _parent=NULL);
        void                        destroy         ();
		static void                 update          ();
		void                        refresh         ();

		virtual void                on_init         ();
		virtual void                on_paint        ();
		virtual void                on_destroy      ();

        const bool&                 created         ();
        void                        parent          (control* _parent);

        void                        location        (const int& _x, const int& _y);
        void                        size            (const int& _width, const int& _height);

        void                        x               (const int& _value);
        void                        y               (const int& _value);
        void                        width           (const int& _value);
        void                        height          (const int& _value);

        const int&                  x               ();
        const int&                  y               ();
        const int&                  width           ();
        const int&                  height          ();

        void                        backcolor       (const int& _red, const int& _green, const int& _blue, const int& _alpha=255);

        void                        draw            (image* _source, const int& _xdest, const int& _ydest);

        class                       implementation;
        friend class                implementation;

	protected:
        control*                    m_parent;

        int                         m_x;
        int                         m_y;
        int                         m_width;
        int                         m_height;

		control::implementation*    imp;
        bool                        m_created;


};

}}

#endif //owl_forms_control_h
