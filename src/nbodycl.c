#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h> 
#include <time.h>
#include <pthread.h>

#include "nbody.c"
#define THREAD_LIMIT 4
/**
 * Struct to take in the multiple aruguments into 1 variable.
 * So that it can be used in the pthread_create() method.
 */
struct thread_args{
	double dt;
	struct body** body_list; 

};

	/**********************************************
	* Idea on how to pass arguments into pthread was taken from the source below. 
 	* Title: Passing arguments to pthread function
 	* Author: "Department of Computer Science and Engineering - Chinese University of Hong Kong"
 	* Date: n.d.
 	* Code Version: N/A (code snippit)
 	* Availability : http://www.cse.cuhk.edu.hk/~ericlo/teaching/os/lab/9-PThread/Pass.html
 	*********************************************/
void* thread_step(void* input){

	double dt = ((struct thread_args*)input)->dt;
	struct body** body_list = ((struct thread_args*)input)->body_list;

	step(dt,body_list);

	return NULL; 
}

int main(int argc, char** argv) {

    int iter;
	int no_of_bodies; 
	double dt; 
	double e;
	int thread_count = 0;

	char filename[256];

	struct body** body_list;
	struct thread_args* args = (struct thread_args*)malloc(sizeof(struct thread_args));

	pthread_t threads[THREAD_LIMIT]; //max 4 threads - since most commericial computers will have 4 cores. 

	if(argc < 4){

		printf("Invalid amount of arguments. \n");
        printf("Expected: ./nbody-gui <iterations> <dt> <flag> <filename/body count> <thread count> \n");
		printf("<flag> :-b <bodies> or -f <filename> \n");
		printf("<thread count> : number from 0 to 4 \n");
		exit(0);
	}

	if(argc > 6){
		
		printf("Too many arguments expected 5, got %d try again. \n",argc-1);
		exit(0);
	}
	
	if(argc == 4){

		if((strcmp(argv[3],"-b")) == 0){
			
			printf("Please input the amount of bodies. \n");
			exit(0);	
	
		}

		if((strcmp(argv[3],"-f")) == 0){
			
			printf("Please input a file name. \n");
			exit(0);	
	
		}
	}

	if(argc == 5){
		printf("Please enter a number between 0 and 4 for the number of threads. \n");
		exit(0);
	}
	
	if (argc == 6){

		if((strcmp(argv[3],"-b")) == 0){

			iter = atoi(argv[1]);
			dt = atof(argv[2]);
			thread_count = atoi(argv[5]);


			if((strcmp(argv[4],"0")) == 0 || (strcmp(argv[4],"1")) == 0) {

				printf("Please enter a number greater than 1\n");
				exit(0);
			}
			if(thread_count > 4){

				printf("Too many threads add a number between 0 and 4");
				exit(0);
			} 
		
			no_of_bodies = atoi(argv[4]);
			body_list = generate_bodies(no_of_bodies);

			for(int a = 0; a < iter; a++){

				args->dt = dt;
				args->body_list = body_list; 

				for(int t = 0; t < thread_count; t++){

					pthread_create(threads+t,NULL,thread_step,args);

				}

				for(int tj = 0; tj < thread_count; tj++){

					pthread_join(threads[tj],NULL);

				}

				//step(dt,body_list);
				e = energy(body_list);
  
				printf("Energy at iteration :%d :%g \n",a,e);
			}

			for(int n = 0; n < len;n++){

			free(body_list[n]);

			}

			free(body_list);
			free(args);
			pthread_exit(NULL);

		}

		if((strcmp(argv[3],"-f")) == 0){

			iter = atoi(argv[1]);
			dt = atof(argv[2]);
			strcpy(filename, argv[4]);
			thread_count = atoi(argv[5]);

			if(thread_count > 4){

				printf("Too many threads add a number between 0 and 4\n");
				exit(0);
			} 

			if(thread_count < 0){

				printf("Please enter a valid thread count between 0 and 4\n");
				exit(0);
			}
		

			body_list = parse_csv(filename);

			for(int b = 0; b < iter; b++){
				
				args->dt = dt;
				args->body_list = body_list; 

				for(int ti = 0; ti < thread_count; ti++){

					pthread_create(threads+ti,NULL,thread_step,args);

				}

				for(int tji = 0; tji < thread_count; tji++){

					pthread_join(threads[tji],NULL);

				}
				//step(dt,body_list);
				e = energy(body_list);

				printf("Energy at iteration :%d :%g \n",b,e);
			}

		for(int n = 0; n < len;n++){

			free(body_list[n]);

		}

		free(body_list);
		free(args);
		pthread_exit(NULL);
		}

	}
	
	return 0;
}
