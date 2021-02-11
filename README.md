# Ind3D
![Logo](https://github.com/UberDever/ind3d/blob/master/logo.png)

![License](https://img.shields.io/github/license/UberDever/ind3d)

Ind3D is a simple 3D FPS game written in C++ and with OpenGL. It was written as course work for Computer Graphics, so it mostly developed in graphics aspect.

# Gamedesign
The plot is following: you awake on space station in the middle of the universe. In the same time you realise, that you are not alone - there a bunch of mad robots, that want you dead.
So, you must escape space station by escape pod by any means. Also, along the way you should destroy as many robots as possible - this will ensure secure escape.

## Gameplay
Gameplay is similar to old 3D shooters like Wolfenstein 3D - there are no vertical movement and just one floor.
There are two types of weapons - pistol and plasma gun. They are hitscan and projectile based respectively.
Thera are also ammo and medkits as collectables.

About enemies - unfortunately there are one type of enemy, that follows player and attacks him as he get close enough. 
He also dies from one shot of pistol or from one projectile from plasma gun.

Map is divided into roomes and some are locked by colored keys - you must acqire them on level for getting further.

# Graphics
There are some features, they described by type:
* Large level, loaded from a text file
* All walls are textured from four sides, therefore they may be quite diverse
* There are also basic floor and ceiling
* All rooms are lit by white light, that come from ceiling lamps
* North side of station opens view on space skybox
* Weapon have basic animations
* All non-wall objects made by [VoxelSprite](https://github.com/UberDever/VoxelSprite), that specifically developed for this kind of low-poly games
* Parts of plasma gun and its projectiles lit by themselves

## Technically speaking
Project was developed in C++ > 11 and with OpenGL Core 3.3.
For window and input handling used SDL2, for image loading stb_image.
All other parts of game like maths, graphic framework, etc... are developed from scratch.

Graphical aspect of application implements basic OpenGL concepts - model handling and texturing, basic lighting and static transparency.
Also implemented emissive lighting and different lightmaps - diffuse, specular and emissive.
No deferred shading / rendering is implemented, so there are not so many light sources on the level - this also helps gameplay purposes.

# Screenshots
Main plot screen

![Plot](https://github.com/UberDever/ind3d/blob/master/screenshots/plot.png)

Robot in all of its glory

![Robot](https://github.com/UberDever/ind3d/blob/master/screenshots/robot.png)

Deep cosmos

![Cosmos](https://github.com/UberDever/ind3d/blob/master/screenshots/cosmos.png)

Eyes from the dark

![Dark](https://github.com/UberDever/ind3d/blob/master/screenshots/eyes.png)

You made it, you reached the exit!

![Entry](https://github.com/UberDever/ind3d/blob/master/screenshots/entry.png)