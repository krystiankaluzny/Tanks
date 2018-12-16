# Tanks

Implementation of Battle City / Tank 1990.
Game was written in C++11 and SDL2 2D graphic library.

![Start menu](resources/img/start.png)
![Stage one](resources/img/stage_1.png)

## Controls:

 - Player 1: arrows and right CTRL to fire
 - Player 2: WSAD and left CTRL to fire
 - Pause: ENTER
 - Jump to next stage: n
 - Jump to previous stage: b
 - Show targets of enemies: t

## Enemies
Each enemy may fire only one bullet in the same time.
If bullet hits target, brick or stage border and explodes then enemy may fire next one.
Enemies may have one of four different armour levels. Each level have a different colour.
After bullet hit armour level decrease.
If the armour level falls to zero, then enemy is destroyed.

If enemy blinks, each hit create new bonus item on a map.

### Enemy types

 - ![Enemy A](resources/img/enemy_a.png) A:
    - target: closest player or eagle; 
    - speed: normal: 
    - behaviour: 80% to move towards the target, 20% to move in random direction,
    - constantly fires in movement direction
 - ![Enemy B](resources/img/enemy_b.png) B: 
    - target: eagle; 
    - speed: 1.3 * normal; 
    - behaviour: 50% to move towards the target, 50% to move in random direction, 
    - constantly fires in movement direction
 - ![Enemy C](resources/img/enemy_c.png) C: 
    - target: eagle; 
    - speed: normal;
    - behaviour: 50% to move towards the target, 50% to move in random direction, 
    - constantly fires in movement direction
 - ![Enemy D](resources/img/enemy_d.png) D:
    - target: closest player or eagle;
    - speed: normal;
    - behaviour: 50% to move towards the target, 50% to move in random direction,
    - fires if target is in front of


## Bonus items

 - ![Bonus grenade](resources/img/bonus_grenade.png) Grenade: all enemies are destroyed
 - ![Bonus helmet](resources/img/bonus_helmet.png) Helmet: active player shield for 10 seconds
 - ![Bonus clock](resources/img/bonus_clock.png) Clock: freeze all enemies for 8 seconds
 - ![Bonus shovel](resources/img/bonus_shovel.png) Shovel: create stone wall around eagle for 15 seconds
 - ![Bonus tank](resources/img/bonus_tank.png) Tank: increase player lives count 
 - ![Bonus star](resources/img/bonus_star.png) Star: increase player speed, each next one increase max bullets count
 - ![Bonus gun](resources/img/bonus_gun.png) Gun: same as three starts
 - ![Bonus boat](resources/img/bonus_boat.png) Boat: allows to move on the water

## Build

### Linux

#### Requirements

 - make
 - libsdl2-dev
 - libsdl2-ttf-dev
 - libsdl2-image-dev
 - doxygen-gui - _for docs generation (optional)_
 - doxygen - _for docs generation (optional)_

On Debian based systems you can run (**apt** can by replaced with **apt-get** or **aptitude**):

`sudo apt install libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev` 

#### Compilation

In the project directory call:

`make clean install`

As a result **build** directory shoudl be created.
In build/**bin** there will be **tanks** binary file with all necessary resources files.

#### Documentation in Polish

In the project directory call:

`make doc`