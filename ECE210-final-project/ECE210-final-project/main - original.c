//*****************************************************************************
// main.c
// Author: jkrachey@wisc.edu
// Modified: Allie
//*****************************************************************************
#include "lab_buttons.h"
#include "BuckyBadger.black.h"		// important information about the image size in here.
#include "BuckyBadger2.black.h"
#include "Invader.black.h"
/******************************************************************************
 * Global Constants and Variables
 *****************************************************************************/
#define MOVE_PIXELS   	2							// Number of Pixels to move each time.
#define LCD_SIZE_X			240						// X Size of LCD screen in pixels
#define LCD_SIZE_Y			320						// Y Size of LCD screen in pixels
#define LCD_MAX_DRAW_X	LCD_SIZE_X-1	// 0 to LCD_MAX_DRAW_X is = LCD_SIZE_X
#define LCD_MAX_DRAW_Y	LCD_SIZE_Y-1	// 0 to LCD_MAX_DRAW_Y is = LCD_SIZE_Y
#define LCD_HALF_SIZE_X	LCD_SIZE_X/2	// X center of screen in pixels
#define LCD_HALF_SIZE_Y	LCD_SIZE_Y/2	// Y center of screen in pixels
#define	LCD_INIT_X			LCD_HALF_SIZE_X - BUCKY_WIDTH_PXL/2		// Start image in the center of the screen X
#define	LCD_INIT_Y			LCD_HALF_SIZE_Y - BUCKY_HEIGHT_PXL/2	// Start image in the center of the screen Y

//*****************************************************************************
//*****************************************************************************
int 
main(void)
{
  uint16_t 	x_pos = LCD_INIT_X;		// x_pos holds the x position of the right corner of the image
  uint16_t 	y_pos = LCD_INIT_Y;		// y_pos holds the y position of the lower corner of the image
  uint16_t	width_pixels = BUCKY_WIDTH_PXL;		// width of the imsge in pixels
	uint16_t 	height_pixels = BUCKY_HEIGHT_PXL;	// height of the image in pixels
	uint8_t 	direction;
	const uint8_t*p_image = bucky_bitmap;
  
  ece210_initialize_board();
  ece210_lcd_add_msg("Animation TEST CODE\n\r",TERMINAL_ALIGN_CENTER,LCD_COLOR_BLUE);
  ece210_lcd_draw_image(x_pos,width_pixels, y_pos,height_pixels, p_image,LCD_COLOR_RED, LCD_COLOR_BLACK);
  // draws the whole image 
  while(1)
  {
    direction = ece210_ps2_read_position();		// Joystick function to read digital value of x or Y pointing.
																							// One of Up, Down, Left or Right value returned.
				
				//Change picture
				if( AlertButtons)
				{
					AlertButtons = false;
					if( btn_up_pressed())
					{
						ece210_lcd_draw_rectangle  (x_pos, BUCKY_WIDTH_PXL, y_pos, BUCKY_HEIGHT_PXL, LCD_COLOR_BLACK); //erase existing image
						width_pixels = BUCKY_WIDTH_PXL;	
						height_pixels = BUCKY_HEIGHT_PXL;
						p_image = bucky_bitmap;
						ece210_lcd_draw_image(x_pos,width_pixels, y_pos,height_pixels, p_image,LCD_COLOR_RED, LCD_COLOR_BLACK);
					}
					if( btn_right_pressed())
					{
						ece210_lcd_draw_rectangle  (x_pos, BUCKY_WIDTH_PXL, y_pos, BUCKY_HEIGHT_PXL, LCD_COLOR_BLACK); //erase existing image
						width_pixels = BUCKY2_WIDTH_PXL;	
						height_pixels = BUCKY2_HEIGHT_PXL;
						p_image = bucky_2_bitmap;
						ece210_lcd_draw_image(x_pos,width_pixels, y_pos,height_pixels, p_image,LCD_COLOR_RED, LCD_COLOR_BLACK);			
					}
					if( btn_left_pressed())
					{
						ece210_lcd_draw_rectangle  (x_pos, BUCKY_WIDTH_PXL, y_pos, BUCKY_HEIGHT_PXL, LCD_COLOR_BLACK); //erase existing image
						width_pixels = INVADER_WIDTH_PXL;	
						height_pixels = INVADER_HEIGHT_PXL;
						p_image = invader_bitmap;
						ece210_lcd_draw_image(x_pos,width_pixels, y_pos,height_pixels, p_image,LCD_COLOR_GREEN, LCD_COLOR_BLACK);				
					}
				}
			
		
		
    switch (direction)												// based on the joystick direction move the image.
    {
      case PS2_RIGHT:													// Joystick pointing left.ll
      {
        if((x_pos+width_pixels) < LCD_SIZE_X - MOVE_PIXELS)	// if not moving past the left edge
				{
          ece210_lcd_draw_rectangle  (x_pos, MOVE_PIXELS, y_pos, height_pixels, LCD_COLOR_BLACK); // erase part of image
          x_pos = x_pos + MOVE_PIXELS;																														// that won't be redrawn
        }										
        break;
      }
      case PS2_LEFT:
      {
        if(x_pos > MOVE_PIXELS-1)														// if not moving past the right edge
        {
          ece210_lcd_draw_rectangle  (x_pos+width_pixels-MOVE_PIXELS, MOVE_PIXELS, y_pos, height_pixels, LCD_COLOR_BLACK);
          x_pos = x_pos - MOVE_PIXELS;
        }
        break;
      }
      case PS2_DOWN:
      {
        if((y_pos+height_pixels) < LCD_SIZE_Y-MOVE_PIXELS)	// if not moving past the top edge
        {
          ece210_lcd_draw_rectangle  (x_pos, width_pixels, y_pos, MOVE_PIXELS, LCD_COLOR_BLACK);
          y_pos = y_pos + MOVE_PIXELS;
        }
        break;
      }
      case PS2_UP:
      {
        if(y_pos > MOVE_PIXELS-1)														// if not moving past the bottom edge
        {
          ece210_lcd_draw_rectangle  (x_pos, width_pixels, y_pos+height_pixels-MOVE_PIXELS, MOVE_PIXELS, LCD_COLOR_BLACK);
          y_pos = y_pos - MOVE_PIXELS;
        }
        break;
      }
      case PS2_CENTER:
      {
        // Do nothing
        break;
      }
      default:
      {
        break;
      }
    }	// end switch
		
		ece210_lcd_draw_image(x_pos,width_pixels, y_pos,height_pixels, p_image,LCD_COLOR_GREEN, LCD_COLOR_BLACK);
									// draw image at the new position using size and bitmap info and forground and background info.	    
  }	// end while 1
}	// end main
