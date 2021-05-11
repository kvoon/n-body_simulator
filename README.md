## Project Description 
As per Assignment specs :  
You are tasked with constructing an application for an n-body simulation. This is a common
astrophysics simulation, modelling orbiting bodies within our solar system and galaxy. Your
application will simulate the orbiting patterns of celestial bodies within your system. This may be
set data provided to you via command line arguments or randomly generated particles.
On top of modelling this application, you will need to write a report and produce a graphical
application which will visualise simulation using the SDL library and render the data. You will be
provided sample test data involving four bodies.
You should aim to not have any locks in your codebase and your application should clearly benefit
from multiple threads processing it.

## Build Instructions
* Navigate to the src folder and use the following commands: 
  * To compile the GUI application:`make nbody-gui`
  * To compile the Command-line application: `make nbody-cl`
  
## Running Instructions
* To run the GUI application use the following arguments when running in terminal:
  * `./nbody-gui <resolution-width> <resolution-height> <iterations> <dt> <-f filename |"-b number of bodies>`
 * Some Examples:
   * Planets orbitting: `./nbody-gui 800 600 100 1000 -f 2dnbody.csv` 
    * Alternatively run the script *run_orbit.sh* 
   * Random bodies scattering: `./nbody-gui 800 600 100 100000 -b 10`
    * Alternatively run the script *run_body_scatter.sh*
 * To run the Command-line application use the following arguments when running in terminal:
   * `./nbody-gui <iterations> <dt> <-f filename |"-b number of bodies> <thread count>`
    * Example configurations are in scripts: *run_cl_0.sh* and *run_planets_csv.sh*
    * note: thread count has to be from 0 to 4 
   
## Cleaning instructions:
 * Simply call `make clean`.
