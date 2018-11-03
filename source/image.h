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

#ifndef owl_forms_image_h
#define owl_forms_image_h

#include <string>
#include <vector>

namespace owl {
namespace forms {

class control;

class color
{
    public:

        color (const int& _r = 255, const int& _g = 255, const int& _b = 255, const int& _a = 255)
        {
            set (_r, _g, _b, _a);
        }

        void set (const int& _r = 255, const int& _g = 255, const int& _b = 255, const int& _a = 255)
        {
            m_value = _a;
            m_value = (m_value << 8) + _r;
            m_value = (m_value << 8) + _g;
            m_value = (m_value << 8) + _b;
        }

        const int r() const { return (m_value & 0x00FF0000) >> 16; }
        const int g() const { return (m_value & 0x0000FF00) >> 8; }
        const int b() const { return (m_value & 0x000000FF); }

        const int& get() const { return m_value; }

    protected:
        int m_value;
};

class image
{
	public:
									image		();
		virtual						~image		();

        bool                        load        (const std::string& _filepath);

        void                        resize      (const int& _width, const int& _height);

        void                        clear       (const int& _r, const int& _g, const int& _b, const int& _a = 255);
        void                        clear       ();

        void                        draw        (image* _source, const int& _xdest, const int& _ydest);

        const int                   width       ();
        const int                   height      ();

        const color&                backcolor   ();

		class 						implementation;
		friend class				implementation;
		friend class				control;

	protected:

		implementation*             imp;
};

}}

#endif //owl_forms_image_h
