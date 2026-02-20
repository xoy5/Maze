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
	player(maze, "Files/Images/Sprites/mouse.bmp", 100.0f, 40, 40, 3, 0.16f, true),
	enemy(maze, &player, "Files/Images/Sprites/ghost.bmp", 70.0f, 40, 40, 2, 0.08f, false)
{
	myMessageBox.SetButtons(MyMessageBox::Buttons::Ok);
	myMessageBox.SetText("Error");

	mazeCharacters.push_back(&player);
	mazeCharacters.push_back(&enemy);
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
	if (flagGameEnd == false)
	{
		Vec2 dir = { 0.0f, 0.0f };
		if (wnd.kbd.KeyIsPressed('A')) dir += {-1.0f, 0.0f};
		if (wnd.kbd.KeyIsPressed('D')) dir += {1.0f, 0.0f};
		if (wnd.kbd.KeyIsPressed('W')) dir += {0.0f, -1.0f};
		if (wnd.kbd.KeyIsPressed('S')) dir += {0.0f, 1.0f};
		player.SetSprintMode(wnd.kbd.KeyIsPressed(VK_SPACE));

		player.SetMovementDirection(dir, maze);
	}
	///////////////////////////////////////
	///////////////////////////////////////



	//////////////// MOUSE ////////////////
	while (!wnd.mouse.IsEmpty())
	{
		const auto e = wnd.mouse.Read();
		if (flagGameEnd)
		{
			MyMessageBox::ValueButton value = myMessageBox.ProcessMouse(e);

			switch (value)
			{
				case MyMessageBox::ValueButton::Ok:
					flagGameEnd = false;
					maze.ResetToDefault();
					for (auto* mc : mazeCharacters)
					{
						mc->ResetToDefault(maze);
					}
					myMessageBox.SetText("Error");
			}
		
		}
	}
	///////////////////////////////////////
}

void Game::UpdateModel(float dt)
{
	if (flagGameEnd == false)
	{

		for (auto* mc : mazeCharacters)
		{
			mc->Update(dt, maze);
		}
		maze.CheckAndCollectCheese(player.GetTilePos()); // If more than one player, use 'if'


		if (enemy.IsTargetCaught())
		{
			flagGameEnd = true;
			myMessageBox.SetText("You GOT CAUGHT!");
		}
		if (maze.GetExitTilePos() == player.GetTilePos() && maze.GetNumberOfCheeses() == 0)
		{
			flagGameEnd = true;
			myMessageBox.SetText("You WIN!");
		}
	}
}

void Game::ComposeFrame()
{
	maze.Draw(gfx);

	// DEBUG
	/*maze.DrawTileHighlightAt(gfx, player.GetTilePos(), Colors::Aqua);
	maze.DrawTileHighlightAt(gfx, player.GetNextTilePos(), Colors::PeachPuff);*/

	for (auto* mc : mazeCharacters)
	{
		mc->Draw(gfx);
	}
	

	if (flagGameEnd)
	{
		myMessageBox.Draw(gfx);
	}
	// Draw FPS
	const std::string fpsText = "FPS: " + std::to_string(FPS);
	fontXs.DrawText(fpsText, Vei2{ 10, 10 }, Colors::White, gfx);

}