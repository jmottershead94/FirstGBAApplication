#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "font.h"
#include "gba.h"

// Variables used across multiple functions //
int PlayerSpriteX = 100;					
int PlayerSpriteY = 130;
int PlayerSpriteXR = 4;
int EnemySpriteX = 1;
int EnemySpriteY = 130;
char KEY;													
int PlatSide;							// Defining the height of the platform from the bottom of the screen.
int PlatEdgeL;
int PlatEdgeR;
int GrappleHookX = PlayerSpriteX + 4;	
int GrappleHookXR = PlayerSpriteXR - 4;
int GrappleHookXRS = PlayerSpriteX - 1;
void DrawChar8(int, int, int, char);	
void DrawString8(int, int, int, const char *s);

void Draw_Rectangle(int GroundLevelX, int GroundLevelY, int width, int height, int colour)
{
	int originalGroundLevelY = GroundLevelY;
	int originalGroundLevelX = GroundLevelX;
	
	while (GroundLevelY <= originalGroundLevelY + height)		// Plots the height of the rectangle.
	{
		while (GroundLevelX <= originalGroundLevelX + width)	// Plots the width of the rectangle.
		{
			PlotPixel8(GroundLevelX, GroundLevelY, colour);		
			GroundLevelX++;										// Increments the X coordinate for the pixel points, so it draws a line.
		}
		GroundLevelX = originalGroundLevelX;					// When the X coordinate equals the new X coordinate, reset the process for the next Y coordinate.
		GroundLevelY++;											// Draws a new line of pixels.
	}
}

void Grapple_Hook()
{
	double DifferenceXL; 	// Difference between the edge of the platforms, and the grapple hook starting point.
	double DifferenceXR;
	double DifferenceY; 	// Difference between the height of the platform and the grapple hook starting point.
	float HypL;		 		// Calculates the hypotenuse of the triangle for the grapple.
	float HypR;
	float AddXL;
	float AddXR;
	float AddY;
	int GrappleHookY = PlayerSpriteY - 1;

	// Limiting x-axis movement on platforms //
	if(PlayerSpriteY == 130)
	{
		if((PlayerSpriteX >= 0 && PlayerSpriteX <= 40) || (PlayerSpriteX >= 60 && PlayerSpriteX <= 80))
		{
			PlatEdgeL = 40;
			PlatEdgeR = 60;
			PlatSide = 100;
		}
		
		if((PlayerSpriteX >= 81 && PlayerSpriteX <= 150) || (PlayerSpriteX >= 170 && PlayerSpriteX <= 233))
		{
			PlatEdgeL = 150;
			PlatEdgeR = 170;
			PlatSide = 90;
		 }
	}
	else
	{
		if(PlayerSpriteY != 130)
		{
			if(PlayerSpriteX >= 40 && PlayerSpriteX <= 60)
			{
				PlatEdgeL = 5;
				PlatEdgeR = 25;
				PlatSide = 30;
			}
		
			if(PlayerSpriteX >= 150 && PlayerSpriteX <= 170)
			{
				PlatEdgeL = 190;
				PlatEdgeR = 210;
				PlatSide = 10;
			}
		}
	}
	
	// Calculations //
	GrappleHookX = PlayerSpriteX + 4;	
	GrappleHookY = PlayerSpriteY - 1; 											// Grapple Hook starts just before the player and just above them.
	GrappleHookXR = PlayerSpriteX - 1;
	
	DifferenceXL = PlatEdgeL - GrappleHookX;									// Calculating the difference in the x-axis distance from the edge of the platform to the grapple hook.
	DifferenceXR = GrappleHookXR - PlatEdgeR;
	DifferenceY = GrappleHookY - PlatSide;										// Calculating the difference in the y-axis distance from the height of the platform to the grapple hook.
	
	HypL = sqrt((DifferenceXL * DifferenceXL) + (DifferenceY * DifferenceY));
	HypR = sqrt((DifferenceXR * DifferenceXR) + (DifferenceY * DifferenceY));
	
	AddXL = (DifferenceXL / HypL);												// Calculates a single pixel unit of the grapple hook.
	AddXR = (DifferenceXR / HypR);
	AddY = (DifferenceY / HypL);
	
	DifferenceXL = GrappleHookX;
	DifferenceXR = GrappleHookXR;
	DifferenceY = GrappleHookY;
	
	if(PlayerSpriteX <= PlatEdgeL)
	{	
		for(double Grapple_Increment = 0; Grapple_Increment < HypL; Grapple_Increment += 1)
		{
			FlipBuffers();														// Animates the grapple hook to look like it is extending from the player.
		
			PlotPixel8(DifferenceXL, DifferenceY, 3);
			DifferenceXL += AddXL;
			DifferenceY -= AddY;
			WaitVSync();
		}
	}
	if(PlayerSpriteX >= PlatEdgeR)
	{
		for(double Grapple_Increment = 0; Grapple_Increment < HypR; Grapple_Increment += 1)
		{
			FlipBuffers();
	
			PlotPixel8(DifferenceXR, DifferenceY, 3);
			DifferenceXR -= AddXR;
			DifferenceY -= AddY;
			WaitVSync();
		}
	}
}

void Jump()
{
	// Tried to create a proper working funtion, could not accomplish..
	// Just going to use a 'teleportation' function instead.
	
	if(PlayerSpriteY != 130)											
	{
		if(PlayerSpriteY >= 20 && PlayerSpriteY <= 30)
		{
			if(PlayerSpriteX >= 5 && PlayerSpriteX <= 14)
			{
				PlayerSpriteX = 7;
				PlayerSpriteY = 21;
			}
			else
			{
				PlayerSpriteX = 42;
				PlayerSpriteY = 90;
			}
		}
		else
		{
			if(PlayerSpriteY >= 90 && PlayerSpriteY <= 100)				// Checks to see if the player is on the bottom left platform.
			{
				if(PlayerSpriteX >= 40 && PlayerSpriteX <= 49)
				{
					PlayerSpriteX = 1;	
					PlayerSpriteY = 130;	
				}
				else
					{
						PlayerSpriteX = 80;
						PlayerSpriteY = 130;
					}
			}
			else
			{			
				if(PlayerSpriteY >= 80 && PlayerSpriteY <= 90)		// Checks to see if the player is on the bottom right platform.
				{	
					if(PlayerSpriteX >= 150 && PlayerSpriteX <= 159)
					{
						PlayerSpriteX = 80;	
						PlayerSpriteY = 130;
					}
						else
						{
							PlayerSpriteX = 190;
							PlayerSpriteY = 130;
						}	
				}
				else
				{
					if(PlayerSpriteY >= 0 && PlayerSpriteY <= 10)	// Checks to see if the player is on the top right platform.
					{
						if(PlayerSpriteX >= 190 && PlayerSpriteX <= 199)
						{
							PlayerSpriteX = 165;	
							PlayerSpriteY = 80;	
						}
						else
						{
							PlayerSpriteX = 220;	
							PlayerSpriteY = 130;
						}
					}
				}
			}
		}
	}
}

void Enemy_Patrol()
{
	enum Enemy_State {Searching_Right};
	Enemy_State current_state = Searching_Right;
	
	FlipBuffers();
	
	if(EnemySpriteX == 233)
	{
		for(int EnemyDec = 233; EnemyDec > 2; EnemyDec--)
		{
			EnemySpriteX--;
		}
	}

   	switch(current_state)
	{
		case Searching_Right:
		{
			EnemySpriteX++;	break;
		}
	}
	
	WaitVSync();
}

void Game_Over()
{
	ClearScreen8(8);
	
	DrawString8(70, 50, 4, "Game Over");
	DrawString8(30, 100, 4, "Please restart the game.");
	
	FlipBuffers();
	
	exit(1);
}

void Main_Game()
{
	int LimPlatXR;							
	int LimPlatXL;
	
	while (true)
	{		
		ClearScreen8(8);
				
		// Draw the player //
		// --------------- //
		Draw_Rectangle(PlayerSpriteX, PlayerSpriteY, 3, 8, 4);		// This is a temporary placeholder for the player.
			
		// Draw the enemy //
		// -------------- //
		Draw_Rectangle(EnemySpriteX, EnemySpriteY, 3, 8, 2);
	
		// Draw the ground floor //
		// --------------------- //
		Draw_Rectangle(0, SCREEN_HEIGHT - 20, SCREEN_WIDTH, 20, 1);	// This will draw a rectangle at the bottom of the screen, going across the whole width of it.
	
		// Draw the platforms //
		// ------------------ //
		Draw_Rectangle(190, 10, 20, 2, 1);	
		Draw_Rectangle(5, 30, 20, 2, 1);	
		Draw_Rectangle(40, 100, 20, 2, 1);	
		Draw_Rectangle(150, 90, 20, 2, 1);	
		
		// Create enemy movement //
		// --------------------- //
		Enemy_Patrol();
		
		// Player movement //
		if(PlayerSpriteY == 130)
		{
			if((REG_KEYINPUT & KEY_RIGHT) == 0)
			{
				if(PlayerSpriteX <= 233)
				{
					PlayerSpriteX++;
				}
			}
		
			if((REG_KEYINPUT & KEY_LEFT) == 0)	
			{
				if(PlayerSpriteX >= 2)
				{
					PlayerSpriteX--;
				}
			}
		}
		
		if(PlayerSpriteY != 130)											// Checks to see if the player is above the ground level.
		{
			LimPlatXL = 1;
			LimPlatXR = 233;
			
			if(PlayerSpriteX >= 5 && PlayerSpriteX <= 25)					// Checks to see if the player is on the top left platform.
			{
				if(PlayerSpriteY >= 20 && PlayerSpriteY <= 30)
				{
					LimPlatXL = 7;											
					LimPlatXR = 20;												
				}
			}
			else
			{
				if(PlayerSpriteX >= 40 && PlayerSpriteX <= 60)				// Checks to see if the player is on the bottom left platform.
				{
					if(PlayerSpriteY >= 90 && PlayerSpriteY <= 100)
					{
						LimPlatXL = 42;	
						LimPlatXR = 55;	
					}
				}
				else
				{			
					if(PlayerSpriteX >= 150 && PlayerSpriteX <= 170)		// Checks to see if the player is on the bottom right platform.
					{	
						if(PlayerSpriteY >= 80 && PlayerSpriteY <=90)
						{
							LimPlatXL = 152;	
							LimPlatXR = 165;
						}
					}
					else
					{
						if(PlayerSpriteX >= 190 && PlayerSpriteX <= 210)	// Checks to see if the player is on the top right platform.
						{
							if(PlayerSpriteY >= 0 && PlayerSpriteY <= 10)
							{
								LimPlatXL = 192;	
								LimPlatXR = 205;	
							}
						}
					}
				}
			}
		}
		
		// Limits the Player on any of the platforms //
		if(PlayerSpriteY != 130)
		{
			if((REG_KEYINPUT & KEY_RIGHT) == 0)
			{
				if(PlayerSpriteX <= LimPlatXR)		// If the player is within the platform limits as defined above.
				{
					PlayerSpriteX++;
				}
			}
		
			if((REG_KEYINPUT & KEY_LEFT) == 0)
			{
				if(PlayerSpriteX >= LimPlatXL)
				{
					PlayerSpriteX--;
				}
			}
		}
		
		// Grapple Hook Movement //
		// --------------------- //
		if((REG_KEYINPUT & KEY_UP) == 0)
		{
			Grapple_Hook();
			
			if(PlayerSpriteX < GrappleHookX)
			{
				PlayerSpriteX = PlatEdgeR - 5;	// If the player grapples from the right, they will appear on the right hand side of the platform.
			}
			
			if(PlayerSpriteX > GrappleHookXR)
			{
				PlayerSpriteX = PlatEdgeL + 2;	// If the player grapples from the left, they will appear on the left hand side of the platform.
			}
		
			PlayerSpriteY = PlatSide - 10;
		}
		
		// Jump Feature //
		// ------------ //
		if((REG_KEYINPUT & KEY_B) == 0)
		{
			Jump();
		}
					
		// Game Over Conditions //
		// -------------------- //
		if(PlayerSpriteY == 130)
		{
			if(EnemySpriteX == PlayerSpriteX - 5 || EnemySpriteX == PlayerSpriteX + 5)
			{
				Game_Over();
				WaitVSync();
			}
		}
		
		WaitVSync();
	}
}

int main()
{
	REG_DISPCNT = MODE4 | BG2_ENABLE;
	
	SetPaletteBG(1, RGB(31, 31, 31));	// White.
	SetPaletteBG(2, RGB(20, 0, 0));		// Red.
	SetPaletteBG(3, RGB(0, 20, 0));		// Green.
	SetPaletteBG(4, RGB(0, 0, 20));		// Blue.
	SetPaletteBG(5, RGB(20, 0, 20));	// Purple.
	SetPaletteBG(6, RGB(20, 20, 0));	// Lime green.
	SetPaletteBG(7, RGB(0, 20, 20));	// Turquiose.
	SetPaletteBG(8, RGB(0, 0, 0));		// Black.
	
	while(true)
	{			
		// Title Screen //
		// ------------ //
		ClearScreen8(8);
		
		DrawString8(50, 30, 4, "GBA Game");
		DrawString8(20, 80, 4, "By Jason Mottershead");
		DrawString8(20, 120, 4, "Press A to play.");
		DrawString8(20, 130, 4, "Hold down key for controls.");		
	
		if((REG_KEYINPUT & KEY_A) == 0)
		{
			Main_Game();	
		}
		
		if((REG_KEYINPUT & KEY_DOWN) == 0)
		{
			ClearScreen8(8);
		
			DrawString8(60, 30, 1, "Controls");
			DrawString8(20, 50, 1, "B-Teleport.");
			DrawString8(20, 60, 1, "UP-Grapple Hook.");
			DrawString8(20, 70, 1, "RIGHT-Move right.");
			DrawString8(20, 80, 1, "LEFT-Move left.");
			DrawString8(20, 120, 1, "A-Play.");
			DrawString8(20, 130, 1, "Let go of down to return.");
			
			WaitVSync();
		}
		
		WaitVSync();
	}
	
	return 0;
}

// Draw ASCII character c at position (x, y) in colour colour.
void DrawChar8(int x, int y, int colour, char c) 
{
	for (int yo = 0; yo < 8; yo++) 
	{
		for (int xo = 0; xo < 8; xo++) 
		{
			if (font_bold[int(c)][(yo * 8) + xo]) 
			{
				PlotPixel8(x + xo, y + yo, colour);
			}
		}
	}
}

// Draw \0-terminated string s at position (x, y) in colour colour.
void DrawString8(int x, int y, int colour, const char *s) 
{
	while (*s != '\0') 
	{
		DrawChar8(x, y, colour, *s);
		x += 8;
		s++;
	}
}