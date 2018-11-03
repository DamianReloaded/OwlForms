#ifndef owl_forms_font_h
#define owl_forms_font_h

#include <vector>

namespace owl {
namespace forms {

class font
{
	public:
									font		();
		virtual						~font		();

		class 						implementation;
		friend class				implementation;

	protected:

		implementation*             imp;
};

}}

#endif //owl_forms_font_h
