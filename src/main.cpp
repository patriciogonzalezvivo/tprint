
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
    std::string port = "NONE";
    std::vector<serial::PortInfo> ports = serial::list_ports();
    for (uint i = 0; i < ports.size(); i++){
        // std::cout << ports[i].port << " - " << ports[i].description << " - " << ports[i].hardware_id << std::endl;
        std::string::size_type found = ports[i].description.find("Prolific Technology Inc. USB-Serial Controller");
        if (found != std::string::npos){
            port = ports[i].port;
            break;
        }
    }

    // Contect the printer to the port
    std::cout << "Connecting to port [" << port << "] ";
    if (printer.open(port)){
        std::cout << "successfully."<< std::endl;
    } else {
        std::cout << "error."<< std::endl;
        return 0;
    }

    // Load files to watch
    for (uint i = 1; i < argc ; i++){

        std::string argument = std::string(argv[i]);
        if (    haveExt(argument,"png") || haveExt(argument,"PNG") ||
                haveExt(argument,"jpg") || haveExt(argument,"JPG") || 
                haveExt(argument,"jpeg") || haveExt(argument,"JPEG") ){
            // Load Image and print it
            printer.printImg(argument);
        } else if ( haveExt(argument,"txt") || haveExt(argument,"TXT") ||
                    haveExt(argument,"md") || haveExt(argument,"MD") ){
            // Load Text to print
            std::string text = "";
            loadFromPath(argument,&text);
            printer.print(text+"\n");
        } else if (argument == "-s") {
            std::string text = "";
            for (uint j = i+1; j < argc ; j++){
                text += std::string(argv[j]) + " ";
            }
            printer.print(text+"\n");
            break;
        }
    }

    printer.close();
    return 0;
}