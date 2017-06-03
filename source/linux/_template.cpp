#include "../class_name.h"
#include "class_name.imp.h"

using namespace owl::forms;

class_name::class_name	()
{
	m_imp = new implementation();
}

class_name::~class_name	()
{

	delete m_imp;
}
