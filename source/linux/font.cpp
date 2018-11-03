#include "../font.h"
#include "font.imp.h"

using namespace owl::forms;

font::font	()
{
	imp = new implementation();
}

font::~font	()
{

	delete imp;
}
