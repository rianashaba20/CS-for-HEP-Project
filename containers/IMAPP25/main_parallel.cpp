#include <SFML/Graphics.hpp>
#include <complex>
#include <iostream>
#include <vector>
#include <string>
#include <tbb/tbb.h>
#include <chrono>  // to measure time
#include <atomic>  // to count tasks
#include <fstream> // to create files

using Complex = std::complex<double>;

// Mandelbrot iteration function
int mandelbrot(Complex const& c)
{
  int i = 0;
  auto z = c;
  for (; i != 256 && norm(z) < 4.; ++i) {      
    z = z * z + c;
  }
  return i;
}

// the number of iterations is returned by 'k' from mandelbrot function
// map iterations count to colour
auto to_color(int k)
{
  return k < 256 ? sf::Color{static_cast<sf::Uint8>(10 * k), 0, 0}     
                 : sf::Color::Black;
}

int main()
{
  // fix the size of the output image with 800x800 pixels
  int const display_width{800};
  int const display_height{800};

  // fix the complex plane bounds 
  Complex const top_left{-2.2, 1.5};
  Complex const lower_right{0.8, -1.5};
  auto const diff = lower_right - top_left;
  
  // map pixels to corresponding complex numbers
  auto const delta_x = diff.real() / display_width;
  auto const delta_y = diff.imag() / display_height;

  // create the image
  sf::Image image;

  /* vector for grain sizes to experiment with
     we choose integers that are powers of 2 because it is easy to see
     the pattern of how the grain size affects the parallelisation.
     adding additional large number of 400 to see performance trend, 
     and 800 for the full row.
  */
  std::vector<int> grain_sizes = {1, 2, 4, 8, 16, 32, 64, 128, 256, 400, 800};

  // create a text file to store the grain size, execution time (in seconds) obtained with that grain size and number of executed tasks
  std::ofstream results("results.csv");
  results << "GrainSize,Time[s],NumTasks\n";    // add header for csv file

  // loop over the multiple grain sizes
  for(auto grain : grain_sizes){

    // count the number of executed tasks
    std::atomic<int> exec_tasks{0};     // use atomic to not risk a race condition

    image.create(display_width, display_height);

    // measure time needed to process the image 
    auto start = std::chrono::high_resolution_clock::now();

    tbb::parallel_for(
        tbb::blocked_range2d<int>(0, display_height, grain, 
                                  0, display_width, grain),
        [&](const tbb::blocked_range2d<int>& r){
            exec_tasks++;   // count one task per each block
            
            // identify the top-left pixel of the block
            int block_start_row = r.rows().begin();
            int block_start_col = r.cols().begin();

            for(int row = r.rows().begin(); row != r.rows().end(); ++row){
                for(int column = r.cols().begin(); column != r.cols().end(); ++column){

                    // draw the grid lines: the top row and left column of each block
                    if((row == block_start_row && row != 0) || (column == block_start_col && column !=0)){
                      image.setPixel(column, row, sf::Color::White);
                    } else {
                        auto k = mandelbrot(top_left + Complex{delta_x * column, delta_y * row});
                        image.setPixel(column, row, to_color(k));
                    }
                }
            }
        },
        tbb::simple_partitioner()
    );

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_interval = end - start;

    // write the results to the text file
    results << grain << ", " << time_interval.count() << ", " << exec_tasks.load() << "\n";

    // print the results for each grain size
    std::cout << "Grain size: " << grain << ": " << "; " << time_interval.count() <<  "s, " << exec_tasks.load() << " tasks \n";

    // to visualise the thread divisions, we can save an image per grain size
    std::string filename = "mandelbrot_grid_grain_" + std::to_string(grain) + ".png";
    image.saveToFile(filename);
  }

  // save the image
  image.saveToFile("mandelbrot.png");

  // close the file
  results.close();

  return 0;
}