#ifndef LOADBAR_H
#define LOADBAR_H
#include <iostream>

void showLoadingBar(int currentPixel, int totalPixels) {
    const int barWidth = 50; // Width of the loading bar (adjust as needed)
    
    // Calculate progress as a percentage
    float progress = static_cast<float>(currentPixel) / totalPixels;
    
    // Calculate how many characters to fill in the bar
    int filled = static_cast<int>(progress * barWidth);
    
    // Create the loading bar string
    std::string bar = "[" + std::string().assign(filled, '#') + std::string().assign(barWidth - filled, ' ') + "]";
    
    // Print the loading bar, and overwrite it with the next one
    std::cout << "\r" << bar << " " << int(progress * 100) << "%" << std::flush;
}


#endif