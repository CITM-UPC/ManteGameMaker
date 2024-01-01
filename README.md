# FrogGameMaker

Frog Game Maker is a game engine: 
Github: https://github.com/CITM-UPC/FrogGameMaker

### Made by: 
Victor Martin (Github: VicMarBall (https://github.com/VicMarBall))

Ari Sevcik (Github: AriSevcik (https://github.com/AriSevcik))

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

 - Information about Frog Engine
