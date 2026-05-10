# CS-for-HEP-Project

Project worked in pairs for the course of Computer Science for High Energy Physics during the third semester of IMAPP at University of Bologna. 

## Abstract
The project description and all its details can be found in [Project Description](https://github.com/rianashaba20/CS-for-HEP-Project/blob/main/Project%20Description.md).

Additional helpful notes with step-by-step guidance can be found in [Notes](Notes.md).

## Content
Everything is inside the [containers](containers) folder.

- Docker files: [Docker Build Mode File](containers/Dockerfile_build) and [Docker Release Mode File](containers/Dockerfile_release)
- CMake file: [CMakeLists.txt](containers/IMAPP25/CMakeLists.txt)
- Code file with parallelisation: [main_parallel.cpp](containers/IMAPP25/main_parallel.cpp)
- All the generated images for each grain can be found in the folder: [builds](containers/IMAPP25/output_results/builds)
- Jupyter Notebook for plotting time against grain size can be found in: [pythonPlotCode](containers/IMAPP25/output_results/plot_timeVSgrain.ipynb)
- File with the number of tasks and time for each grain size can be found in: [results.csv](containers/IMAPP25/output_results/results.csv)
