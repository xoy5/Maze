/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#pragma once

#include <vector>
#include <random>

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "FrameTimer.h"
#include "Font.h"
#include "Button.h"
#include "TextBox.h"
#include "MyMessageBox.h"


#include "Maze.h"
#include "PlayerMouse.h"
#include "Character.h"

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();

private:
	void ProcessInput();
	void UpdateModel(float dt);
	void ComposeFrame();
	/********************************/
	/*  User Functions              */

	/********************************/

private:
	MainWindow& wnd;
	Graphics gfx;

	////////// FONTS /////////
	const Font fontXs = Font("Files/Fonts/font8x14.bmp");
	const Font fontSm = Font("Files/Fonts/font12x21.bmp");
	const Font fontBase = Font("Files/Fonts/font16x28.bmp");
	const Font fontLg = Font("Files/Fonts/font18x32.bmp");
	const Font fontXl = Font("Files/Fonts/font20x35.bmp");
	const Font font2Xl = Font("Files/Fonts/font24x42.bmp");
	const Font font3Xl = Font("Files/Fonts/font32x56.bmp");
	///////////////////////////

	std::mt19937 rng = std::mt19937(std::random_device{}());
	const float precision = 0.0025f;
	FrameTimer ft;
	MyMessageBox myMessageBox = MyMessageBox(&fontLg);

	/////////// FPS ///////////
	int FPS = 0;
	float timeFrame = 0.0f;
	float timeSecond = 0.0f;
	int numberOfFrames = 0;
	///////////////////////////

	/********************************/
	/*  User Variables  */
	Maze maze;
	PlayerMouse playerMouse;
	bool flag_gameEnd = false;
	/********************************/
};
