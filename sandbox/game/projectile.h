//
// Created by uberdever on 07.09.2020.
//

#ifndef IND3D_PROJECTILE_H
#define IND3D_PROJECTILE_H

#include "../core.h"

void projectile_init(v_Projectile_t *projectiles);

void projectile_create(v_Projectile_t *projectiles, v2 pos, v2 dir);
void projectile_update(Map *map, v_Projectile_t *projectiles);
void projectile_render(v_Projectile_t *projectiles, vi2 offset);

void projectile_clean(v_Projectile_t *projectiles);

#endif //IND3D_PROJECTILE_H
