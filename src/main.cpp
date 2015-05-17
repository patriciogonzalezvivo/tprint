
#include <string>
#include <fstream>
#include <iostream>

#include "thermalPrinter.h"

static inline bool loadFromPath(const std::string& path, std::string* into) {
    std::ifstream file;
    std::string buffer;

    file.open(path.c_str());
    if(!file.is_open()) return false;
    while(!file.eof()) {
        getline(file, buffer);
        (*into) += buffer + "\n";
    }

    file.close();
    return true;
}

static inline bool haveExt(const std::string& file, const std::string& ext){
    return file.find("."+ext) != std::string::npos;
}

ThermalPrinter printer;

// Main program
//============================================================================
int main(int argc, char **argv){

    //  Get a list of ports
    std::vector<serial::PortInfo> ports = serial::list_ports();
    // for (uint i = 0; i < ports.size(); i++){
    //     std::cout << ports[i].port << " - " << ports[i].description << " - " << ports[i].hardware_id << std::endl;
    // }

    //  Find the one with the Thermo-Printer
    //
    std::string port = "/dev/cu.usbserial"; //"/dev/tty.PL2303-00002014"

    // Contect the printer to the port
    //printer.open(port);

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
        } else if ( haveExt(argument,"txt") || haveExt(argument,"TXT") ||
                    haveExt(argument,"md") || haveExt(argument,"MD") ){
            // Load Text to print
            loadFromPath(argument,&text);
        } else {
            text += argument + " ";
        }
    }

    if (text.size() > 0){
        printer.print(text);
    }


    return 0;
}