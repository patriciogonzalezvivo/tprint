
#include <string>
#include <iostream>

#include "thermalPrinter.h"

static inline bool haveExt(const std::string& file, const std::string& ext){
    return file.find("."+ext) != std::string::npos;
}

ThermalPrinter printer;

// Main program
//============================================================================
int main(int argc, char **argv){

    printer.open("/dev/cu.usbserial");
    // printer.open("/dev/tty.PL2303-00002014");

    std::string text = "";
    // Load files to watch
    for (uint i = 1; i < argc ; i++){
        std::string argument = std::string(argv[i]);

        if (    haveExt(argument,"png") || haveExt(argument,"PNG") ||
                haveExt(argument,"jpg") || haveExt(argument,"JPG") || 
                haveExt(argument,"jpeg") || haveExt(argument,"JPEG") ){
            // Load Image and print it
            printer.printImg(argument);
            text = "";
        } else {
            text += argument + " ";
        }
    }

    if (text.size() > 0){
        printer.print(text);
    }


    return 0;
}