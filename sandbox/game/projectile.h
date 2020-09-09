//
// Created by uberdever on 07.09.2020.
//

#ifndef IND3D_PROJECTILE_H
#define IND3D_PROJECTILE_H

#include "../util.h"

void projectile_create(v2 pos, v2 dir);
void projectile_update();
void projectile_render(vi2 offset);

#endif //IND3D_PROJECTILE_H
