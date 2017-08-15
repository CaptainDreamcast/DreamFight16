#include "mugensound.h"

MugenSound makeMugenSound(int tGroup, int tItem)
{
	MugenSound ret;
	ret.mGroup = tGroup;
	ret.mItem = tItem;
	return ret;
}
