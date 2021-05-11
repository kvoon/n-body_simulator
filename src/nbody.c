#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include <time.h>
#include <pthread.h>

#include "nbody.h"  

#define PI (3.141592653589793)
#define SOLARMASS (4 * PI * PI) 
#define NDAYS (365.25) 
#define GCONST (6.67e-11) 

int len = 0; // Keeps track of the length of the body_list [TODO: Find a better solution if time permits.]

//File size calculation from GeekforGeeks 
/**********************************************
 * Title: C Program to find a size of a File
 * Author: Kanishk_Verma
 * Date: 2018
 * Code Version: 1.0
 * Availability : https://www.geeksforgeeks.org/c-program-find-size-file/#:~:text=The%20idea%20is%20to%20use,is%20actually%20size%20in%20bytes.
 * ********************************************/
int findSize(char filename[]) { 

    // opening the file
    FILE* file = fopen(filename, "r"); 
  
	if(file == NULL){
		printf("Error opening file, be sure to have the correct file name \n");
		exit(0);
	}
  
    fseek(file, 0L, SEEK_END); 
  
    // calculating the size of the file 
    int res = ftell(file); 
  
    return res; 
} 

/**
 * 
 * Method to parse the csv and store the variables into the body struct. 
 *  
 */
struct body **parse_csv(char *filename){

	double x,y,z,x_velocity,y_velocity,z_velocity,mass; //temp variables to store the parsed data.

	int i = 0; //counter

	char line[1024]; // buffer 

	FILE * file; 

	file = fopen(filename,"r");

	long int file_length = findSize(filename);

	//printf("%d \n",file_length);

	if(file == NULL){
		printf("Error opening file \n");
		exit(0);
	}

	// intialise a list to store the body structs
	struct body** body_list = (struct body**)malloc(sizeof(struct body*)* file_length); 

	//Parse the file
	while(fgets(line, sizeof(line), file)){

		struct body* body =(struct body*)malloc(sizeof(struct body)); // initialise the body struct

		sscanf(line,"%lf,%lf,%lf,%lf,%lf,%lf,%lf",&x,&y,&z,&x_velocity,&y_velocity,&z_velocity,&mass);
		
		body->x = x;
		body->y = y;
		body->z = z;
		body->velocity_x = x_velocity;
		body->velocity_y = y_velocity;
		body->velocity_z = z_velocity;
		body->mass = mass; 

		body_list[i] = body; 
			
		// printf("x:%f, y:%f, z:%f, vel_x:%f, vel_y:%f, vel_z:%f, mass:%f \n",body_list[i]->x,body_list[i]->y,body_list[i]->z,
		// body_list[i]->velocity_x,body_list[i]->velocity_y,body_list[i]->velocity_z,body_list[i]->mass);	
		++i;
	}
	
	len = i; 

	return body_list;
}

// Generates n (body_count) amount of bodies with random values between -100 and 100.

	//Random number generation formula taken from stackoverflow.
	/**********************************************
 	* Title: Generate-a-random-double-between-1-and-1
 	* Author: cp.engr
 	* Date: 2015
 	* Code Version: N/A (code snippit)
 	* Availability : https://stackoverflow.com/questions/33058848/generate-a-random-double-between-1-and-1
 	*********************************************/
struct body** generate_bodies(int body_count){

	struct body** body_list = (struct body**)malloc(sizeof(struct body*)* body_count); 

	srand(time(NULL));
	
	double max = 100; 
	double min = -100;
	double range = max-min; 
	double div = RAND_MAX/range;

	for(int i = 0; i< body_count;i++){

		struct body* body =(struct body*)malloc(sizeof(struct body)); 

		body->x = min + ((double) rand()/div);
		body->y = min + ((double) rand()/div);
		body->z = min + ((double) rand()/div);
		body->velocity_x = min + ((double) rand()/div);
		body->velocity_y = min + ((double) rand()/div);
		body->velocity_z = min + ((double) rand()/div);
		body->mass = min + ((double) rand()/div);

		body_list[i] = body; 
		
		// printf("x:%f, y:%f, z:%f, vel_x:%f, vel_y:%f, vel_z:%f, mass:%f \n",body_list[i]->x,body_list[i]->y,body_list[i]->z,
		// body_list[i]->velocity_x,body_list[i]->velocity_y,body_list[i]->velocity_z,body_list[i]->mass);	

	}
	len = body_count;

	return body_list; 
}

double dist_calc(double dx, double dy, double dz){

	double distance;

	return distance = sqrt( (dx*dx) + (dy*dy) + (dz*dz) );

}


/**
 * Step function interpreted in the specs.  
 * Structure based on: https://edstem.org/courses/4818/discussion/331643
 * double dt, struct body** body_list
 */
void step(double dt, struct body** body_list){

	double dx,dy,dz,dist;
	double sum_acceleration_x, sum_acceleration_y, sum_acceleration_z;
	double magnitude_ij; 
	double unit_vec_x,unit_vec_y,unit_vec_z;
	double a_x, a_y, a_z;

	for(int i = 0; i< len;i++){
	//	printf("Start x: %g, y: %g, z: %g \n",body_list[i]->x,body_list[i]->y,body_list[i]->z);	
		sum_acceleration_x = 0;
		sum_acceleration_y = 0;
		sum_acceleration_z = 0;
		
		for(int j = 0; j<len;j++){

			if(j!=i){
			
				dx = (body_list[j]->x)-(body_list[i]->x);
				dy = (body_list[j]->y)-(body_list[i]->y);
				dz = (body_list[j]->z)-(body_list[i]->z);

				// distance formula
				dist = dist_calc(dx,dy,dz);
				//printf("dist: %lf \n",dist);

				// magnitude formula 
				magnitude_ij = GCONST*((body_list[i]->mass*body_list[j]->mass)/(dist*dist));
				// printf("Mag: %g \n",magnitude_ij);

				// working out the unit vector of each component. 
				unit_vec_x = dx/dist;
				unit_vec_y = dy/dist;
				unit_vec_z = dz/dist;
				// printf("unit vec: %f,%f,%f \n",unit_vec_x,unit_vec_y,unit_vec_z);

				//calculating acceleration for each component. 
				a_x = (unit_vec_x * magnitude_ij)/body_list[i]->mass;
				a_y = (unit_vec_y * magnitude_ij)/body_list[i]->mass;
				a_z = (unit_vec_z * magnitude_ij)/body_list[i]->mass;
				// printf("%g,%g,%g \n",a_x,a_y,a_z);	
			
				// sum of acceleration * dt calculations for each component. 
				sum_acceleration_x += (a_x *dt);
            	sum_acceleration_y += (a_y *dt); 
            	sum_acceleration_z += (a_z *dt);
				
			}

		}

		//apply acceleration - vel_i (t+1) = velocity(t) + Sum(ax*dt, ay*dt, az*dt)
		body_list[i]->velocity_x += sum_acceleration_x;
		body_list[i]->velocity_y += sum_acceleration_y;
		body_list[i]->velocity_z += sum_acceleration_z;

		// printf("For I :px: %g,py: %g,pz: %g \n",pos_x_i,pos_y_i,pos_z_i);

		//update the position
		body_list[i]->x += body_list[i]->velocity_x * dt;
		body_list[i]->y += body_list[i]->velocity_y * dt;
		body_list[i]->z += body_list[i]->velocity_z * dt;
		//printf("End x: %g, y: %g, z: %g \n",body_list[i]->x,body_list[i]->y,body_list[i]->z);
	}


}

double energy(struct body** body_list){

	double e,sum_U;

	for(int i = 0; i < len; i++){
		
		sum_U = 0;
		
		e = 0;
		double vel_x_i = body_list[i]->velocity_x;
		double vel_y_i = body_list[i]->velocity_y;
		double vel_z_i = body_list[i]->velocity_z;

		double vel_i = sqrt((vel_x_i*vel_x_i) + (vel_y_i* vel_y_i) + (vel_z_i * vel_z_i));
		
		double e_k = (1/2)*body_list[i]->mass *(vel_i*vel_i); // kinetic energy (1/2mv^2)

		for(int j = 0; j < len; j++){
			
			if(j != i){

			double dx = (body_list[j]->x)-(body_list[i]->x);
			double dy = (body_list[j]->y)-(body_list[i]->y);
			double dz = (body_list[j]->z)-(body_list[i]->z);

			double dist  = dist_calc(dx,dy,dz); //distance formula 

			double U = -GCONST * ( (body_list[i]->mass*body_list[j]->mass)/dist); // potential energy [-G(mi*mk)/distance]

			sum_U += U;
			
			}

			e += e_k - sum_U;
		
		}
	}

		return e;
}

