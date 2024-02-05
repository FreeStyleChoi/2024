#include "update.h"

int Update(const char key, Vector speed, const int frameDelay)
{
	int result = 0;
	if (key == 'x')
	{
		result = (int)(speed.x * frameDelay);
	}
	else if (key == 'y')
	{
		result = (int)(speed.y * frameDelay);
	}
	else
	{
		result = NULL;
	}
	return result;
}