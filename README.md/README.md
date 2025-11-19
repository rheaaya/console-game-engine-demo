# Console Game Engine Demo 🎮

This project is a simple **console-based game engine** built to demonstrate rendering, movement, and game-loop logic entirely inside a terminal window.  
It is lightweight, educational, and easy to extend.

## 🔥 Features
- Text/sprite rendering in the terminal  
- Player movement (WASD or arrow keys depending on version)  
- Basic game loop with framerate timing  
- Clear and modular code structure  
- Cross-platform (Linux, Windows, macOS compatible)  
- Great as a starting point for ASCII games  

## 📁 Project Structure
console-game-engine-demo/
├── main.c
├── engine.c
├── engine.h
├── assets/ ← optional (sprites, maps, etc.)
└── README.md

## ⚙️ How to Build (Linux / macOS)
```bash
gcc main.c engine.c -o game
gcc main.c engine.c -o game.exe
./game.exe
Controls

W → Move up

A → Move left

S → Move down

D → Move right
Requirements

GCC or Clang compiler

Terminal capable of ASCII rendering

Standard C library

Riya Sondhi
Console Game Engine Demo