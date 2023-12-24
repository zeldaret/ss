#ifndef NW4R_EF_EMFORM_H
#define NW4R_EF_EMFORM_H
#include "types_nw4r.h"

namespace nw4r
{
	namespace ef
	{
		enum EmitFormType
		{
			EmitForm_Disc,
			EmitForm_Line,
			EmitForm_Cube = 5,
			EmitForm_Cylinder = 7,
			EmitForm_Sphere,
			EmitForm_Point,
			EmitForm_Torus
		};
		
		struct EmitFormBuilder
		{
			virtual EmitterForm * Create(EmitFormType);
		};
	}
}

#endif