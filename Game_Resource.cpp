#include "Game_Resource.h"


int Route_Select(int room, int select)
{
	if (room == 0 && select == 1) return 10;
	else if (room == 0 && select != 1) return 20;
}