#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "nbody.c"

// #define VIEW_WIDTH (800)
// #define VIEW_HEIGHT (600)

//THIS IS A SDL2 TEMPLATE

/**
 * Updates the x based on where it is, will invert the  
 * velocity when it reaches out of bounds.
 */
void update_x(int* cx, int* velocity, int width) {
	if(cx && velocity) {
		if(*cx <= 0) {
			*cx = 0;
			*velocity = 1;
		} else if(*cx >= width) {
			*cx = width;
			*velocity = -1;
		}
		*cx = *cx + *velocity;
	}
}

int main(int argc, char** argv) {

	//Your SDL variables and variables for circle data and finished state
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Event event;
	int finished = 0;
	// int circle_x = 100;
	// int vel = 1;

	int res_width;
	int res_height; 

	//console commands 
	int iter;
	int no_of_bodies; 
	double dt;
	//double e;
	char filename[256];

	struct body** body_list;

	if(argc < 6){

		printf("Invalid arguments.\n");
		printf("Expected: ./nbody-gui <resolution_width> <resolution_height> <iterations> <dt> (-b or -f) \n");
		printf("-b <bodies> or -f <filename> \n");
		exit(0);
	}

	if(argc > 7){
		
		printf("Too many arguments expected 6, got %d try again. \n",argc-1);
		exit(0);
	}

	if(argc == 6){

		if((strcmp(argv[5],"-b")) == 0){
			
			printf("Please input the amount of bodies \n");
			exit(0);	
	
		}

		if((strcmp(argv[5],"-f")) == 0){
			
			printf("Please input a file name \n");
			exit(0);	
	
		}
	}
	
	if (argc == 7){

		if((strcmp(argv[5],"-b")) == 0){

			res_width = atoi(argv[1]);
			res_height = atoi(argv[2]);

			if(res_width < 0 || res_height < 0){

				printf("invalid resolution side. Try again. \n");
				exit(0);
			}

			iter = atoi(argv[3]);
			dt = atof(argv[4]);

			if((strcmp(argv[6],"0")) == 0 || (strcmp(argv[6],"1")) == 0) {

				printf("Please enter a number greater than 1 \n");
				exit(0);
			} 
		
			no_of_bodies = atoi(argv[6]);
			body_list = generate_bodies(no_of_bodies);

			for(int a = 0; a < iter; a++){

				step(dt,body_list);
				//e = energy(body_list);

				//printf("Energy at iteration :%d :%g \n",a,e);
			}

		}

		if((strcmp(argv[5],"-f")) == 0){

			res_width = atoi(argv[1]);
			res_height = atoi(argv[2]);
			iter = atoi(argv[3]);
			dt = atof(argv[4]);

			if(res_width < 0 || res_height < 0){

				printf("invalid resolution side. Try again. \n");
				exit(0);
			}

			strcpy(filename, argv[6]);
			body_list = parse_csv(filename);

			for(int b = 0; b < iter; b++){

				step(dt,body_list);
				//e = energy(body_list);

				//printf("Energy at iteration :%d :%lf \n",b,e);
			}
	}

	double max = 1e+12; 

	//Initialises SDL Video Acceleration
	//Check if return value is < 0
	SDL_Init(SDL_INIT_VIDEO);
	
	/**
	 * Creates a window to display
	 * Allows you to specify the window x,y position
	 * Allows you to specify the width and height of the window
	 * Option for flags for the window and potentially the different instances
	 * you may want to use with it. SHOWN, makes the window visible
	 * Should check if it returns NULL
	 */
	window = SDL_CreateWindow("N-Body Simulation",
		SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
		res_width, res_height, SDL_WINDOW_SHOWN);
	
	/**
	 * Create Renderer retrieves the 2D rendering context of the window
	 * You will need to use this to use with SDL2_gfx library 
	 * This will give you access to the underlying rederer of the window
	 * Should check if it returns NULL
	 */
	renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED 
		| SDL_RENDERER_PRESENTVSYNC);

	/**
	 * Render loop of your application
	 * You will perform your drawing in this loop here
	 */
	while(!finished) {

		//Sets the background colour 
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
		
		//Clears target with a specific drawing colour (prev function defines colour)
		SDL_RenderClear(renderer);
		
		//Draws a circle using a specific colour
		//Pixel is RGBA (0x(RED)(GREEN)(BLUE)(ALPHA), each 0-255
		//filledCircleColor(renderer, circle_x, 100, 7, 0xFF0000FF);

		//filledCircleColor(renderer,res_width/2,res_height/2,30,0xFFFFFFFF); //center point reference 

		/**
		 * Scaling idea taken from ed post: https://edstem.org/courses/4818/discussion/331643
		 */
		for(int i =0; i<len;i++){

			double x = body_list[i]->x/max * res_width+res_width/2;
			double y = -1*body_list[i]->y/max * res_height+res_height/2; //-1 to rotate counter-clockwise 

			filledCircleColor(renderer,x,y,7,0xFF0000FF); 
			
			step(dt,body_list);			

			printf("Current coordinates for body %d: x: %g, current y: %g, current z: %g \n",i,body_list[i]->x,body_list[i]->y,body_list[i]->z);
		}

		//Updates the screen with newly renderered image
		SDL_RenderPresent(renderer);
		
		//Updates the x coordinate of circle
		//update_x(&body, &vel, VIEW_WIDTH);

		//Retrieves the events captured from SDL (just watching for windows close)
		if(SDL_PollEvent(&event)) {
			finished = (event.type == SDL_QUIT);
		}		
	}

	//Clean up functions
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();	
	}

	//Free the the elements in the list.
	for(int n = 0; n < len; n++){
		free(body_list[n]);
	}

	//Free the actual body_list. 
	free(body_list);
	
	return 0;	
}
