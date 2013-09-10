#ifdef _PAD_H_
#define _PAD_H_

#include "Object.h"

namespace Logic
{
	class Pad: public Object
	{
	public:
		Pad() : Object();
		~Pad();
		void update();
	private:
	}
}
#endif