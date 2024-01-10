## Description of the project:
[MantelGameMaker](https://github.com/CITM-UPC/MantelGameMaker) is a 3D GameEngine created by Mantel, a group of 2 students of Videogame Design and Development from CITM - Barcelona.
Specifically, this 3D GameEngine focus on the High-Level System of audio.

## New Features: 

Camera Refactoring

Bounding Boxes

Frustum Culling

Mouse Picking

Hierarchy Editing

Own File Format (.sht)

Resources loaded in /Library

PLAY and STOP

## How To Use: 
### Move Camera 
Right Click and WASD to move

Shift to move faster

Right Click and move the mouse to look around

Alt + Left Click and move the mouse to orbit around

F to Focus on the selected object

Right-Click to an object to select it

In the menu General < PLAY / STOP to play the game

### Debug: 

K to change the Frustum Culling from the screen camera to the camera created at the scene and viceversa

### Drag & Drop 
Drag and drop .FBX files to create a Game Object and load the mesh

Drag and drop .PNG and .DDS to apply a texture to the selected Game Object

## Windows
### Hierarchy Window 
You can see the scene with all the Game Objects in it

You can select a game object and see its properties in the Inspector

Press the arrows to see the children of the Game Object

### Inspector Window 
You can see the Transform, Mesh and Texture of the Game Object

Transform: 

 - You can see the Position, Rotation and Scale

Mesh: 

 - You can see the file name and its path
 - You can see the number of vertex and faces
 - You can check the checkbox to see the mesh with a checkboard texture

Texture: 
 - You can see the file name and its path
 - You can see the file size

Camera: 
 - You can edit the parameters of a camera (FOV, zNear, zFar, Aspect Ratio)

### Console Window
See console logs

### General Menu
Activate/Deactivate Editor

Go To GitHub

Quit

### Create Menu
Create Basic Shapes

### Configuration Menu
Configuration of:

Window: 

 - Screen size and aspect ratio
    
 - Fullscreen, Resizable and Borderless

Renderer:

Input:

 - Inputs and Outputs

### Info Menu
Hardware Information: 

 - CPU's Cores
 - System RAM
 - GPU info
 - VRAM information

FPS Log

### Help Menu
Demo Window: ImGui Demo Window

About: 

 - Information about [Mantel Engine](https://github.com/CITM-UPC/MantelGameMaker)


## Made by: 

![](https://raw.githubusercontent.com/CITM-UPC/MantelGameMaker/High-Level-System/docs/Assets/MantelLogo.png)

Our organization is called Mantel and it's formed by 2 students of UPC in Barcelona:

----

  ![](https://raw.githubusercontent.com/CITM-UPC/MantelGameMaker/High-Level-System/docs/Assets/LuisGonzalez.png)
- [Luis González Ramírez](https://github.com/punto16)

----

  ![](https://raw.githubusercontent.com/CITM-UPC/MantelGameMaker/High-Level-System/docs/Assets/AdrianPonce.png)
- [Adrian Gonzalo Ponce Jara](https://github.com/AdrianGPJ)

----

This project is based on Frog Game Maker code on their 2nd release:
[Github Project](https://github.com/CITM-UPC/FrogGameMaker)
- [Victor Martin](https://github.com/VicMarBall)
- [Ari Sevcik](https://github.com/AriSevcik)

## [Click here to Download latest version of the engine](https://github.com/CITM-UPC/MantelGameMaker/releases/download/Release_v.2.0/MantelGameMaker_v.2.0.zip)

## License 
This project is licensed under an unmodified MIT license, which is an OSI-certified license that allows static linking with closed source software. Check [LICENSE](docs/LICENSE.html) for further details.
