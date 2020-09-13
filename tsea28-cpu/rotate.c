// Ugly simplifications have been done to this code. 
// Initial version by Andreas was using a proper coding
// style with structures etc.
// Have chosen to use global variables to simplify
// code structure and help students understand the code
// 
// 2017  Kent Palmkvist

#include <stdint.h>
#include "util.h"
#include "memorymap.h"

#include "config.h"

// The angle we start rendering at.
#define FIRSTANGLE 90

#define MAP_BASE 0x81800000

/* Change this to 0x81000000 and 0x81400000 if you also want to see
   writes to the framebuffer in the logic analyzer. (Although this
   will degrade the performance...) */

#define FB_FRAME0 0x01000000
#define FB_FRAME1 0x01400000


// Global variables, do NOT change
    uint16_t *image_map_addr;   // An array which contains the reference image
	int image_startx;           // Coordinates in 24.8 fixed point format
	int image_starty;           // Coordinates in 24.8 fixed point format
	int image_dx;               // Delta-X in 24.8 fixed point format
	int image_dy;               // Delta-X in 24.8 fixed point format
    uint16_t *current_fb;       // An array which contains the framebuffer


// Calculate index into big image based on x and y in 24.8 format
static inline int fixed_point_xy_to_linear(int x,int y)
{
	return (y/256)*MAP_XSIZE + x/256;
}


// Function that copies one pixel at (x,y) in framebuffer from the big image
// Framebuffer pixel at (0,0) is located at (image_xstart,image_ystart) in the
// big image. Distance between frame buffer pixels (0,0) and (1,0) are image_dx
// and image_dy in the big image (i.e., frame buffer pixel (1,0) is located at
// (image_xstart+image_dx,image_ystart+image_dy) in the big image 
static inline void copy_pixel(int x, int y) 
{
  current_fb[y*FB_XSIZE+x] = image_map_addr[fixed_point_xy_to_linear(
                                               image_startx+x*image_dx-y*image_dy,
                                               image_starty+y*image_dx+x*image_dy)];
}


	
// Render all pixels based on x and y positions in the framebuffer.
// Make a call to copy_pixel with all combination of x and y of the
// framebuffer.  to make the rotation work.
// 
void render_all_lines(void)
{

	// Loop variables creating all necessary coordinates
	int x, y, i;

	// Fastest for 90 deg would be 16 but then 180 lags so 64 is a compromise
	int block_size = 64;
	int amount_of_blocks = FB_XSIZE / block_size;

	for (i = 0; i < amount_of_blocks; i++){
		for (y = 0; y < FB_YSIZE; y++) {
			for (x = i * block_size; x < (i + 1) * block_size; x++){
				if (x == 0) {
					/*if (y == 0) {
					  trigger_logic_analyzer();
					  // small_printf("x=0,y=1, read:%d,%d\n\f",x,y); // example print
					}*/
				}
				copy_pixel(x, y);
			}
		}
	}
	  
/*
  i = 0;
  do {
		y = 0;
		do {
			x = i * block_size;
			do {
				copy_pixel(x,y);
				x++;
			} while (x < (i + 1) * block_size);
			y++;
		} while (y < FB_YSIZE);
		i++;
  } while (i < amount_of_blocks); */
  
 
/* 
 // Initial solution: Copy pixels from left to right, line by line
  y = 0;   // start at the top of the buffer (1st line)
  do {
    x = 0;   // start to the left of the line
    do {

      // Trigger chipscope at position x,y = 0,1 (first pixel on 2nd line)
      // Note: takes time, remove (add // in front of this section to verify
      // that performance requirements are met
      if (x == 1) {
    	if (y == 0) {
    	  trigger_logic_analyzer();
    	  // small_printf("x=0,y=1, read:%d,%d\n\f",x,y); // example print
    	}
      }

      // Copy pixel to position x,y in framebuffer from big image 
      copy_pixel(x,y);
                                                      
      x = x + 1;
    } while (x < FB_XSIZE);    // Do this until line completed
    y = y + 1;
  } while (y < FB_YSIZE);      // Do this until full picture completed
*/

}


void redraw_reference_image(void)
{
	// If you believe that you need to do so you can redraw the
	// reference image in this function.
}

/**************************************************************************
 * YOU PROBABLY DON'T NEED TO MODIFY ANYTHING BELOW THIS LINE IN THE LAB! *
 **************************************************************************/

void rotate_image(int angle, int xpos, int ypos, uint16_t *fb)
{

	int cosval = fixed_point_cos(angle);
	int sinval = fixed_point_sin(angle);


	int xr,yr,xs,ys;


	// First we setup the render parameters
	ys = 0 - FB_YSIZE/2;
	xs =  - FB_XSIZE/2;

	// Rotate the start coordinates by multiplying with the rotation matrix
	xr = cosval*xs - sinval*ys;
	yr = sinval*xs + cosval*ys;

	// And translate the start coordinates by adding the current position
	xr = xr  + xpos;
	yr = yr  + ypos;

	image_map_addr = (uint16_t *) MAP_BASE;
	image_startx = xr + 128; // By adding 128 we ensure that we get slightly
	image_starty = yr + 128; // better rounding 
	current_fb = fb;
	image_dx = cosval;
	image_dy = sinval;

	// Then we render all lines using these parameters using the
	// appropriate function.
	render_all_lines();
}

void paintloop(void)
{

	int angle;
	uint16_t *fb;

	uint32_t starttime, endtime;

	int current_buffer = 0;

	int speed = 256;
	int xpos = MAP_XSIZE/2*256;
	int ypos = MAP_YSIZE/2*256;
	angle = FIRSTANGLE;
	int demo_mode = 0;

	while (1) {
		// This part checks that we are not trying to move outside the map itself
		int out_of_bounds = 0;
		int cosval = fixed_point_cos(angle);
		int sinval = fixed_point_sin(angle);

		xpos = xpos + speed*(sinval)/256;;
		ypos = ypos + speed*(-cosval)/256;

		if(xpos < (FB_XSIZE/2)*256){
			xpos = (FB_XSIZE/2)*256+2;
			out_of_bounds = 1;
		}else if(xpos > (MAP_XSIZE - FB_XSIZE-32)*256){
			xpos = (MAP_XSIZE - FB_XSIZE-32)*256-2;
			out_of_bounds = 1;
		}


		if(ypos < (FB_YSIZE/2)*256){
			ypos = (FB_YSIZE/2)*256+2;
			out_of_bounds = 1;
		}else if(ypos > (MAP_YSIZE - FB_XSIZE-32)*256){
			ypos = (MAP_YSIZE - FB_XSIZE-32)*256-2;
			out_of_bounds = 1;
		}

		if(out_of_bounds){
			small_printf("Going too close to the border!\r\n");
			speed=0;
		}
		
		// If we are in demo mode we should make sure the
		// screen rotates every frame
		if(demo_mode){
			angle = angle + 1;
			if(angle >= 360){
				angle = 0;
			}
		}

		// This part implements the user interface.
		// Notably: Pressing D activates demo mode and
		// pressing q exits the program.
		if(uart_has_data()){
			demo_mode = 0;
			switch(uart_getc()){
			case 'D':
				if(demo_mode) {
					demo_mode = 0;
					break;
				}
				small_printf("Activating demo mode, press any key to quit\r\n");
				xpos = 868*256;
				ypos=356*256;
				speed = 256*7;
				angle = 90;
				demo_mode = 1;
				break;
			case ' ':
				speed = 0;
				break;
			case 'q':
				return;
			case 'a':
				
                                angle = angle - 1;
				if(angle < 0){
					angle = 359;
				}
				break;
			case 'd':
				angle = angle + 1;
				if(angle > 359){
					angle = 0;
				}
				break;
			case 's':
				speed = speed - 16;
				if(speed < 0){
					speed = 0;
				}
				break;
			case 'w':
				speed = speed + 16;
				if(speed > 256*32){
					speed = 256*32;
				}
				break;
			}
		}
		// Flush remaining piled up characters to increase responsiveness
		while(uart_has_data()){
			uart_getc();
		}

		// Figure out the correct buffer to render to
		if(current_buffer == 1){
			fb = (uint16_t *) FB_FRAME1;
			current_buffer = 0;
		}else{
			fb = (uint16_t *) FB_FRAME0;
			current_buffer = 1;
		}

		/* This part is responsible for the main bulk of the runtime cost */
		starttime = get_timer();
		rotate_image(angle, xpos, ypos, fb);
		Flush_DCache(); // Ensure all data hits the framebuffer!
		endtime = get_timer(); // Only measure time after all data has hit the framebuffer.

		// Wait for current VSync and swap the currently showing framebuffer
		int lag; // Tells whether we skipped a frame because rendering took too long.
		if(current_buffer == 1){
			lag = framebuffer_swap(FB_FRAME0);
		}else{
			lag = framebuffer_swap(FB_FRAME1);
		}

		
		// Note: We are running the UART at 115200 baud. This
		// corresponds to about 11520 characters per second
		// (at the default setting of 8N1).
		//
		// This means that we can print about 11520/60 = 192
		// characters per iteration of this loop (assuming the
		// loop runs at full framerate).
		//
		// However, we are further limited by the UART FIFO
		// which only contains 64 characters, all in all. Thus
		// we are limited to a maximum of 64 characters per
		// loop unless we want our performance to degradate
		// severely. (This constraint could be lifted if we
		// used an IRQ driven UART, although this is way
		// too complicated for our current needs.)
		small_printf("Angle: %d, time: %d useconds%s\r\n", angle, 
			     (endtime-starttime)*2/533,
			     lag ? "(LAG!)":"");
			
	}
}


int main(void)
{
#ifdef SDL_ROTATE
	initialize_sdl_rotate();
#endif

	start_timer();
	redraw_reference_image();
	paintloop();
	return 0;
}


