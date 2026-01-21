#include <SFML/Graphics.hpp>
#include <complex>
#include <iostream>
#include <vector>
#include <string>
#include <oneapi/tbb.h>
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
     adding additional large numbers dividable by 800 to see performance trend, 
     and 800 for the full row.
  */
  std::vector<int> grain_sizes = {1, 2, 4, 8, 10, 16, 20, 32, 40, 64, 80, 100, 128, 256, 400, 800};

  // create a text file to store the grain size, execution time (in seconds) obtained with that grain size and number of executed tasks
  std::ofstream results("results.csv");
  results << "GrainSize,Time[s],NumTasks\n";    // add header for csv file

  // loop over the multiple grain sizes
  for(auto grain : grain_sizes){

    // count the number of executed tasks
    std::atomic<int> exec_tasks{0};     // use atomic to not risk a race condition

    image.create(display_width, display_height);

    // start measuring time needed to process the image here (measure parallel computation time)
    auto start = std::chrono::high_resolution_clock::now();

    oneapi::tbb::parallel_for(
        oneapi::tbb::blocked_range2d<int>(0, display_height, grain, 
                                  0, display_width, grain),
        [&](const oneapi::tbb::blocked_range2d<int>& r){
            exec_tasks++;   // count one task per each block
            
            // process each pixel in this block to compute mandelbrot values
            for(int row = r.rows().begin(); row != r.rows().end(); ++row){
                
                // precompute the imaginary part of this entire row to avoid
                // computing (delta_y * row) for each pixel
                double im = top_left.imag() + delta_y * row;
                double re = top_left.real(); 

                for(int column = r.cols().begin(); column != r.cols().end(); ++column){
                    
                    // compute the complex number for this pixel
                    Complex c{re + delta_x * column, im};

                    // compute mandelbrot iteration count
                    auto k = mandelbrot(c);

                    // set pixel colour based on the iteration count
                    image.setPixel(column, row, to_color(k));
                }
            }
        },
        oneapi::tbb::simple_partitioner()
    );

    // stop measuring time here (the parallel computation is now done)
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_interval = end - start;

    // we draw grid lines to visualise thread blocks.

    /*
    first horizontal grid lines to separate rows of blocks:
    we start at "grain" because we don't want a line at the very top (row 0).
    step by "grain" to draw lines between each block row
    */

    for(int block_row = grain; block_row < display_height; block_row += grain){
      // for every horizontal grid line, colour all the pixels in that row white
      for(int col{0}; col < display_width; ++col){
        image.setPixel(col, block_row, sf::Color::White);
      }
    }

    /*
    now we draw vertical grid lines to separate columns of blocks:
    we start at "grain" because we don't want a line at the very left (col 0).
    step by "grain" to draw lines between each block column.
    */

    for(int block_col = grain; block_col < display_width; block_col += grain){
      // for every vertical grid line, colour all the pixels in that column white
      for(int row{0}; row < display_height; ++row){
        image.setPixel(block_col, row, sf::Color::White);
      }
    }

    // write the results to the text file
    results << grain << ", " << time_interval.count() << ", " << exec_tasks.load() << "\n";

    // print the results for each grain size
    std::cout << "Grain size: " << grain << ": " << "; " << time_interval.count() <<  "s, " << exec_tasks.load() << " tasks \n";

    // to visualise the thread divisions, we can save an image per grain size
    std::string filename = "mandelbrot_grid_grain_" + std::to_string(grain) + ".png";
    image.saveToFile(filename);
  }

  // save the final image with the last grain size's grid
  image.saveToFile("mandelbrot.png");

  // close the file
  results.close();

  return 0;
}