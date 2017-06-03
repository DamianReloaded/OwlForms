#ifndef owl_forms_class_name_h
#define owl_forms_class_name_h

#include <vector>

namespace owl {
namespace forms {

class class_name
{
	public:
									class_name		();
		virtual						~class_name		();

		class 						implementation;
		friend class				implementation;
		
	protected:

		implementation* m_imp;
};

}}

#endif //owl_forms_class_name_h