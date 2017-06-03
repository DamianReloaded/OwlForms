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

#ifndef owl_forms_image_implementation_h
#define owl_forms_image_implementation_h

#include "../image.h"

#include <X11/X.h>    /* X11 constant (e.g. TrueColor) */
#include <X11/keysym.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xcms.h>

namespace owl {
namespace forms {

class image::implementation
{
	public:
								implementation  ();
		virtual					~implementation ();

        void                    resize          (const int& _width, const int& _height);

        void                    update          ();

        void                    clear           ();
        void                    clear           (const int& _r, const int& _g, const int& _b, const int& _a = 255);

        Pixmap                  m_pixmap;
        XImage*                 m_ximage;
        int                     m_width;
        int                     m_height;
        color                   m_backcolor;

        std::vector<unsigned char>  m_buffer;
};

}}

#endif // owl_forms_image_implementation_h
