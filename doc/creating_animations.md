# **Creating Animatons**

### Table of Contents
[**Requirements**](#requirements)   
[**How to create new animation**](#how-to-create-new-animation)    
[**Available tools for creating animations**](#available-tools-for-creating-animations)    
[**Update led states**](#update-led-states)


## **Requirements**

* Building environment prepared -> [environment_setup_linux](environment_setup_linux.md)

## **How to create new animation**

You can use script, which will does all module initialization in an automated way.

The scriot is located under `tools/animation_init/animation_init.py` directory.

**Usage:**
```bash
python3 tools/animation_init/animation_init.py -n animation_name
```
**IMPORTANT: `-n/--name` parameter value needs to follow the snake case naming convention.**

After calling an above command, all necessary files will be created and all needed configuration will be done. 

You can start developing the animation. Animation module will be located under `animations/animation_name` directory. The whole logic of an animation needs to be put in `AnimationName::run()` method body.

## Available tools for creating animations

All animations can use matrix operations, to manipulate led states in led matrix.

#### **EnableAll**
Enable all leds from (0, 0, 0) position to position given through `CartesianCoordinates`.

e.g.
```cpp
MatrixOperation enable_all = EnableAll(CartesianCoordinates(3, 3, 3));
cube.action(enable_all, LedSwitch::ENABLE, Color::YELLOW);
cube.render();
``` 

Leds can be also disabled in this way `LedSwitch::DISABLE`.

#### **EnableSingle**
Enable single led with position given in `CartesianCoordinates`.

e.g.
```cpp
MatrixOperation enable_single = EnableSingle(CartesianCoordinates(1, 2, 2));
cube.action(enable_single, LedSwitch::ENABLE, Color::BLUE);
cube.render();
``` 

#### **EnableColumn**
Enable single column, which is being described by `ColumnCoordinates`.
ColumnCoordinates parameters:
1. Base plane
2. First coordinate (depending on plane)
3. Second coordinate (depending on plane)
4. Column height
5. Column start (optional, default=0)

e.g.
```cpp
MatrixOperation enable_column = EnableColumn(ColumnCoordinates(Plane::Y, 1, 1, 4));
cube.action(enable_column LedSwitch::ENABLE, Color::GREEN);
cube.render();
``` 

#### **EnablePlane**
Enable single plane, which can be described by `PlaneCoordinates`.
e.g.
PlaneCoordinates parameters:
1. Plane
2. Height (optional, default=0)

e.g.
```cpp
MatrixOperation enable_plane = EnablePlane(PlaneCoordinates(Plane::Z, 2));
cube.action(enable_plane, LedSwitch::ENABLE, Color::CYAN);
cube.render();
``` 

#### **EnableCuboid**
Enable single cuboid, which can be described by `CuboidCoordinates`, which consists of two `CartesianCoordinates` (Position of two opposite corners).

e.g.
```cpp
CartesianCoordinates X(1, 1, 1);
CartesianCoordinates S(2, 2, 2);
MatrixOperation enable_cuboid = EnableCuboid(CuboidCoordinates(X, S));
cube.action(enable_cuboid LedSwitch::ENABLE, Color::RED);
cube.render();
``` 

## **Update led states**

To update the actual state of the leds depending on LedMatrix state, you can use `LedCube.render()` method.
It will write all the leds data to LedCube hardware.

```cpp
cube.render();
```