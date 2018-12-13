//*****************************************************************************
// main.c
// Author: jkrachey@wisc.edu
// Modified: Allie
//*****************************************************************************
#include "lab_buttons.h"
#include "BuckyBadger.black.h"		// important information about the image size in here.
#include "BuckyBadger2.black.h"
#include "Invader.black.h"
#include <stdio.h>
#include <stdlib.h>

bool isCollidingWith(uint16_t x_pos, uint16_t y_pos, uint16_t obs_x, uint16_t obs_y);
bool isCollidingWithLong(uint16_t x_pos, uint16_t y_pos, uint16_t obs_x, uint16_t obs_y);
;/******************************************************************************
 * Global Constants and Variables
 *****************************************************************************/
#define MOVE_PIXELS   	1							// Number of Pixels to move each time.
#define MOVE_PIXELS2   	1							// Number of Pixels to move each time.
#define MOVE_PIXELS3   	2							// Number of Pixels to move each time.
#define MOVE_PIXELS4   	1							// Number of Pixels to move each time.
#define LCD_SIZE_X			240						// X Size of LCD screen in pixels
#define LCD_SIZE_Y			320						// Y Size of LCD screen in pixels
#define LCD_MAX_DRAW_X	LCD_SIZE_X-1	// 0 to LCD_MAX_DRAW_X is = LCD_SIZE_X
#define LCD_MAX_DRAW_Y	LCD_SIZE_Y-1	// 0 to LCD_MAX_DRAW_Y is = LCD_SIZE_Y
#define LCD_HALF_SIZE_X	LCD_SIZE_X/2	// X center of screen in pixels
#define LCD_HALF_SIZE_Y	LCD_SIZE_Y/2	// Y center of screen in pixels
#define	LCD_INIT_X			LCD_HALF_SIZE_X - INVADER_WIDTH_PXL/2 - 50		// Start image in the center of the screen X
#define	LCD_INIT_Y			LCD_SIZE_Y - INVADER_HEIGHT_PXL	// Start image in the bottom of the screen Y

#define LOCAL_ID     0x144
#define REMOTE_ID    0x149

#define UP_BUTTON 0x01
#define DOWN_BUTTON 0x02
#define LEFT_BUTTON 0x03
#define RIGHT_BUTTON 0x04

uint16_t	obs_height = BUCKY2_HEIGHT_PXL;;
uint16_t	obs_width = BUCKY2_WIDTH_PXL;
uint16_t	obs_width_long = BUCKY_WIDTH_PXL;
uint16_t	width = INVADER_WIDTH_PXL;
uint16_t 	height = INVADER_HEIGHT_PXL;
uint16_t	led_counter = 0;

uint32_t rx_data; //received data
uint32_t tx_data; //transfer data

//*****************************************************************************
//*****************************************************************************
int 
main(void)
{
  uint16_t 	x_pos = LCD_INIT_X;		// x_pos holds the x position of the right corner of the image
	uint16_t 	x2_pos = LCD_INIT_X + 50;	
  uint16_t 	y_pos = LCD_INIT_Y;		// y_pos holds the y position of the lower corner of the image
	uint16_t 	y2_pos = LCD_INIT_Y;		
	uint16_t	obs1_x = LCD_SIZE_X - BUCKY_WIDTH_PXL;
	uint16_t	obs1_y = LCD_HALF_SIZE_Y + 100;
	uint16_t	obs2_x = LCD_SIZE_X - BUCKY2_WIDTH_PXL;
	uint16_t	obs2_y = LCD_HALF_SIZE_Y + 50;
	uint16_t	obs3_x = 0;
	uint16_t	obs3_y = LCD_HALF_SIZE_Y;
	uint16_t	obs4_x = 0;
	uint16_t	obs4_y = LCD_HALF_SIZE_Y - 50;
	const uint8_t*p_image = invader_bitmap;
	const uint8_t*p_imageObstacleLong = bucky_bitmap;
	const uint8_t*p_imageObstacle = bucky_2_bitmap;
	
	bool keyPressed = false;
  
  ece210_initialize_board();
	
	ece210_lcd_add_msg("FROGGER", TERMINAL_ALIGN_CENTER, LCD_COLOR_GREEN);
	ece210_lcd_add_msg("PRESS ANY BUTTON TO START GAME", TERMINAL_ALIGN_CENTER, LCD_COLOR_RED);
	ece210_wireless_init(LOCAL_ID,REMOTE_ID);

	while (!keyPressed) {
		if(AlertButtons)
    {
      AlertButtons = false;
			keyPressed = true;
			ece210_lcd_draw_rectangle  (0, LCD_SIZE_X, 0, LCD_SIZE_Y, LCD_COLOR_BLACK);
    }
	}
	
  while(1)
  { 	
		//1st Moving obstacle(blue): right to left
		ece210_lcd_draw_image(obs1_x ,obs_width_long, obs1_y,obs_height,p_imageObstacleLong ,LCD_COLOR_BLUE, LCD_COLOR_BLACK);
		ece210_lcd_draw_rectangle  (obs1_x+obs_width_long-MOVE_PIXELS, MOVE_PIXELS, obs1_y, obs_height, LCD_COLOR_BLACK);
		obs1_x = obs1_x - MOVE_PIXELS;
		if (obs1_x == 0) {
			ece210_lcd_draw_rectangle  (0, obs_width_long, obs1_y, obs_height, LCD_COLOR_BLACK);
			obs1_x = LCD_SIZE_X - obs_width_long;
			obs1_y = (rand() % LCD_SIZE_Y) - obs_height;
		}
		
		//2nd Moving obstacle(red): right to left
		ece210_lcd_draw_image(obs2_x ,obs_width, obs2_y,obs_height,p_imageObstacle ,LCD_COLOR_RED, LCD_COLOR_BLACK);
		ece210_lcd_draw_rectangle  (obs2_x+obs_width-MOVE_PIXELS2, MOVE_PIXELS2, obs2_y, obs_height, LCD_COLOR_BLACK);
		obs2_x = obs2_x - MOVE_PIXELS2;
		if (obs2_x == 0) {
			ece210_lcd_draw_rectangle  (0, obs_width, obs2_y, obs_height, LCD_COLOR_BLACK);
			obs2_x = LCD_SIZE_X - obs_width;
			obs2_y = (rand() % LCD_SIZE_Y) - obs_height;
		}
		
		//3rd Moving obstacle(green): left to right
		ece210_lcd_draw_image(obs3_x ,obs_width, obs3_y,obs_height,p_imageObstacle ,LCD_COLOR_GREEN, LCD_COLOR_BLACK);
		ece210_lcd_draw_rectangle  (obs3_x, MOVE_PIXELS3, obs3_y, obs_height, LCD_COLOR_BLACK);
		obs3_x = obs3_x + MOVE_PIXELS3;
		if (obs3_x == (LCD_SIZE_X - obs_width)) {
			ece210_lcd_draw_rectangle  (obs3_x, obs_width, obs3_y, obs_height, LCD_COLOR_BLACK);
			obs3_x = 0;
			obs3_y = (rand() % LCD_SIZE_Y) - obs_height;
		}
		
		//4th Moving obstacle(yellow): left to right
		ece210_lcd_draw_image(obs4_x ,obs_width, obs4_y,obs_height,p_imageObstacle ,LCD_COLOR_YELLOW, LCD_COLOR_BLACK);
		ece210_lcd_draw_rectangle  (obs4_x, MOVE_PIXELS4, obs4_y, obs_height, LCD_COLOR_BLACK);
		obs4_x = obs4_x + MOVE_PIXELS4;
		if (obs4_x == (LCD_SIZE_X - obs_width)) {
			ece210_lcd_draw_rectangle  (obs4_x, obs_width, obs4_y, obs_height, LCD_COLOR_BLACK);
			obs4_x = 0;
			obs4_y = (rand() % LCD_SIZE_Y) - obs_height;
		}
		
		//Draw user frogger image
		ece210_lcd_draw_image(x_pos,width, y_pos,height, p_image,LCD_COLOR_GREEN, LCD_COLOR_BLACK);	
		
		//Draw user frogger image
		ece210_lcd_draw_image(x2_pos,width, y2_pos,height, p_image,LCD_COLOR_YELLOW, LCD_COLOR_BLACK);	
		
	

		//Check for collisions
		if (isCollidingWithLong(x_pos, y_pos, obs1_x, obs1_y) || isCollidingWith(x_pos, y_pos, obs2_x, obs2_y)
			|| (isCollidingWith(x_pos, y_pos, obs3_x, obs3_y)) || (isCollidingWith(x_pos, y_pos, obs4_x, obs4_y))) {
				//Terminate program
			while(1) {
				//Draw dead frogger (red)
			ece210_lcd_draw_image(x_pos,width, y_pos,height, p_image,LCD_COLOR_RED, LCD_COLOR_BLACK);
			}
		}
		

		//Change user frogger position
		if( AlertButtons)
		{
			AlertButtons = false; }
			//Check which direction to move frogger
			if( btn_up_pressed())
			{
				if(y_pos > MOVE_PIXELS-1)														// if not moving past the bottom edge
				{
					ece210_lcd_draw_rectangle  (x_pos, width, y_pos+height-MOVE_PIXELS, MOVE_PIXELS, LCD_COLOR_BLACK);
					y_pos = y_pos - MOVE_PIXELS;
					ece210_wireless_send(UP_BUTTON);
				}
			}
			if( btn_right_pressed())
			{
				if((x_pos+width) < LCD_SIZE_X - MOVE_PIXELS)	// if not moving past the left edge
				{
					ece210_lcd_draw_rectangle  (x_pos, MOVE_PIXELS, y_pos, height, LCD_COLOR_BLACK); 
					x_pos = x_pos + MOVE_PIXELS;																													
				}				
			}
			if( btn_left_pressed())
			{
				if(x_pos > MOVE_PIXELS-1)														// if not moving past the right edge
				{
					ece210_lcd_draw_rectangle  (x_pos+width-MOVE_PIXELS, MOVE_PIXELS, y_pos, height, LCD_COLOR_BLACK);
					x_pos = x_pos - MOVE_PIXELS;
				}			
			}	
			
			if( btn_down_pressed())
			{
				if((y_pos+height) < LCD_SIZE_Y-MOVE_PIXELS)	// if not moving past the top edge
				{
					ece210_lcd_draw_rectangle  (x_pos, width, y_pos, MOVE_PIXELS, LCD_COLOR_BLACK);
					y_pos = y_pos + MOVE_PIXELS;
				}			
			}
			
			//Change opponent frogger position
			if(ece210_wireless_data_avaiable())
			{
				rx_data = ece210_wireless_get();
				if( rx_data == UP_BUTTON)
					if(y2_pos > MOVE_PIXELS-1)														// if not moving past the bottom edge
					{
						ece210_lcd_draw_rectangle  (x2_pos, width, y2_pos+height-MOVE_PIXELS, MOVE_PIXELS, LCD_COLOR_BLACK);
						y2_pos = y2_pos - MOVE_PIXELS;
					}
			}
			
			//Check if frogger reaches the end
			if (y_pos == 0) {
				ece210_ws2812b_write(led_counter, 20, 20, 20);
				ece210_lcd_draw_rectangle  (x_pos, width, 0, height, LCD_COLOR_BLACK);
				y_pos = LCD_SIZE_Y - height;
				led_counter++;
				//User has reached the reached the end the maximum number of times
				if(led_counter == 8) {
					while(1) {}
					}
			}

		
		ece210_lcd_draw_image(x_pos,width, y_pos,height, p_image,LCD_COLOR_GREEN, LCD_COLOR_BLACK);
				ece210_wait_mSec(10);
						
				
				
  }	// end while 1
}	// end main


bool isCollidingWith(uint16_t x_pos, uint16_t y_pos, uint16_t obs_x, uint16_t obs_y) {
	//Boundary conditions for obstacle
	uint16_t obs_top = obs_y;
	uint16_t obs_bottom = obs_y + obs_height;
	uint16_t obs_left = obs_x;
	uint16_t obs_right = obs_x + obs_width;
	
	//Boundary conditions for frogger
	uint16_t frogger_top = y_pos;
	uint16_t frogger_bottom = y_pos + height;
	uint16_t frogger_left = x_pos;
	uint16_t frogger_right = x_pos + width;
	
	//Top with right and left
	if ((frogger_top <= obs_bottom && frogger_top >= obs_top)
		&& ((frogger_left >= obs_left && frogger_left <= obs_right)
		|| (frogger_right >= obs_left && frogger_right <= obs_right)))
		return true;
	//Bottom with right and left
	else if ((frogger_bottom <= obs_bottom && frogger_bottom >= obs_top)
		&& ((frogger_left >= obs_left && frogger_left <= obs_right)
		|| (frogger_right >= obs_left && frogger_right <= obs_right)))
		return true;
	else 
		return false;
}

bool isCollidingWithLong(uint16_t x_pos, uint16_t y_pos, uint16_t obs_x, uint16_t obs_y) {
	//Boundary conditions for obstacle
	uint16_t obs_top = obs_y;
	uint16_t obs_bottom = obs_y + obs_height;
	uint16_t obs_left = obs_x;
	uint16_t obs_right = obs_x + obs_width_long;
	
	//Boundary conditions for frogger
	uint16_t frogger_top = y_pos;
	uint16_t frogger_bottom = y_pos + height;
	uint16_t frogger_left = x_pos;
	uint16_t frogger_right = x_pos + width;
	
	//Top with right and left
	if ((frogger_top <= obs_bottom && frogger_top >= obs_top)
		&& ((frogger_left >= obs_left && frogger_left <= obs_right)
		|| (frogger_right >= obs_left && frogger_right <= obs_right)))
		return true;
	//Bottom with right and left
	else if ((frogger_bottom <= obs_bottom && frogger_bottom >= obs_top)
		&& ((frogger_left >= obs_left && frogger_left <= obs_right)
		|| (frogger_right >= obs_left && frogger_right <= obs_right)))
		return true;
	else 
		return false;
}