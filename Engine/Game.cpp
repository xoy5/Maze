/******************************************************************************************
 *	Chili DirectX Framework Version 16.07.20											  *
 *	Game.cpp																			  *
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
#include "MainWindow.h"
#include "Game.h"
#include <assert.h>
#include <algorithm>

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	playerMouse(maze, "Files/Images/Sprites/mouse.bmp", 90.0f, 40, 40, 3, 0.16f, true)
{
	myMessageBox.SetButtons(MyMessageBox::Buttons::Ok);
	myMessageBox.SetText("Error");
}

void Game::Go()
{
	gfx.BeginFrame();
	ProcessInput();
	const float elapsedTime = ft.Mark();
	float time = elapsedTime;
	while (time > 0.0f) {
		const float dt = std::min(precision, time);
		UpdateModel(dt);
		time -= dt;
	}
	numberOfFrames++;
	timeSecond += elapsedTime;
	if (timeSecond >= 1.0f) {
		timeSecond -= 1.0f;
		FPS = numberOfFrames;
		numberOfFrames = 0;
	}

	ComposeFrame();
	gfx.EndFrame();
}

void Game::ProcessInput()
{
	////////////// KEYBOARD ///////////////
	/////////////// TYPING ////////////////
	// Keys
	while (!wnd.kbd.KeyIsEmpty())
	{
		const Keyboard::Event keyPressed = wnd.kbd.ReadKey();
		if (keyPressed.IsValid() && keyPressed.IsPress())
		{

		}
	}
	// Characters
	while (!wnd.kbd.CharIsEmpty())
	{
		const char character = wnd.kbd.ReadChar();
	}
	///////////////////////////////////////
	/////////////// MOVEMENT //////////////
	Vec2 dir = { 0.0f, 0.0f };
	if (wnd.kbd.KeyIsPressed('A')) dir += {-1.0f, 0.0f};
	if (wnd.kbd.KeyIsPressed('D')) dir += {1.0f, 0.0f};
	if (wnd.kbd.KeyIsPressed('W')) dir += {0.0f, -1.0f};
	if (wnd.kbd.KeyIsPressed('S')) dir += {0.0f, 1.0f};

	playerMouse.SetSprintMode(wnd.kbd.KeyIsPressed(VK_SPACE));

	playerMouse.SetDir(dir, maze);
	///////////////////////////////////////
	///////////////////////////////////////



	//////////////// MOUSE ////////////////
	while (!wnd.mouse.IsEmpty())
	{
		const auto e = wnd.mouse.Read();
		if (flag_gameEnd)
		{
			MyMessageBox::ValueButton value = myMessageBox.ProcessMouse(e);

			switch (value)
			{
				case MyMessageBox::ValueButton::Ok:
					flag_gameEnd = false;
					maze.ResetToDefault();
					playerMouse.ResetToDefault(maze);
					myMessageBox.SetText("Error");
			}
		
		}
	}
	///////////////////////////////////////
}

void Game::UpdateModel(float dt)
{
	if (flag_gameEnd == false)
	{
		playerMouse.Update(dt, maze);
		maze.CheckAndCollectCheese(playerMouse.GetTilePos()); // If more than one player, use 'if'
		if (maze.GetExitTilePos() == playerMouse.GetTilePos() && maze.GetNumberOfCheeses() == 0)
		{
			flag_gameEnd = true;
			myMessageBox.SetText("You WIN");
		}
	}
}

void Game::ComposeFrame()
{
	maze.Draw(gfx);
	maze.DrawTileHighlightAt(gfx, playerMouse.GetTilePos(), Colors::Aqua);
	maze.DrawTileHighlightAt(gfx, playerMouse.GetNextTilePos(), Colors::PeachPuff);
	playerMouse.Draw(gfx);

	if (flag_gameEnd)
	{
		myMessageBox.Draw(gfx);
	}
	// Draw FPS
	const std::string fpsText = "FPS: " + std::to_string(FPS);
	fontXs.DrawText(fpsText, Vei2{ 10, 10 }, Colors::White, gfx);

}