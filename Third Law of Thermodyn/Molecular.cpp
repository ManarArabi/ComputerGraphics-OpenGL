#include "stdafx.h"
#include "Molecular.h"
#include <stdlib.h>
#include "Color.h"

Molecular::Molecular()
{
	posX = posY = posZ = 0;
	color.setColors(1, 1, 1);
}

Molecular::~Molecular()
{
}

void Molecular::randmize()
{
	int flag = 1;

	posY = (rand() % 8) / -10.0;
	if (flag == 1) { posY *= -1; }
	flag *= -1;
	posX = (rand() % 10) / -10.0;
	posZ = -1.0 * (2 + rand() % 3);
}
