//
// Created by uberdever on 07.09.2020.
//

#ifndef IND3D_PROJECTILE_H
#define IND3D_PROJECTILE_H

#include "../core.h"

void projectile_init(v_Projectile_t *projectiles);

void projectile_create(v_Projectile_t *projectiles, v2_t pos, v2_t dir);
void projectile_update(Map *map, v_Projectile_t *projectiles);

void projectile_clean(v_Projectile_t *projectiles);

#endif //IND3D_PROJECTILE_H
