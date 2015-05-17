#include "ThermalPrinter.h"

#include <unistd.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "std/stb_image.h"

ThermalPrinter::ThermalPrinter(): bConnected(false){
}

ThermalPrinter::~ThermalPrinter() {
}

bool ThermalPrinter::open(const std::string& _portName){
    try {
        port = SharedSerial(new serial::Serial( _portName,
                                               BAUDRATE,
                                               serial::Timeout::simpleTimeout(1000),
                                               serial::eightbits,
                                               serial::parity_none,
                                               serial::stopbits_one,
                                               serial::flowcontrol_none ));
    }
    
    catch (const std::exception& exc){
        std::cout << "ThermalPrinter:: Fail to open" << exc.what() << std::endl;
        bConnected = false;
        return bConnected;
    }
    
    bConnected = true;
    usleep(50000);
    reset();
    usleep(50000);
    
    setControlParameter();
    setPrintDensity();
    setStatus(true);
    
    port->flushOutput();
    
    // setReverse(true);
    // println("Reverse ON");
    setReverse(false);
    
    return bConnected;
}

void ThermalPrinter::write(const uint8_t &_a){
    if(bConnected){
        port->write(&_a, 1);
        usleep(BYTE_TIME);
    }
}

void ThermalPrinter::write(const uint8_t &_a,const uint8_t &_b ){
    const uint8_t command[2] = { _a, _b };
    write(command, 2);
    usleep(BYTE_TIME*2);
}

void ThermalPrinter::write(const uint8_t &_a, const uint8_t &_b, const uint8_t &_c ){
    const uint8_t command[3] = { _a, _b, _c };
    write(command, 3);
    usleep(BYTE_TIME*3);
}

void ThermalPrinter::write(const uint8_t &_a, const uint8_t &_b, const uint8_t &_c, const uint8_t &_d){
    const uint8_t command[4] = { _a, _b, _c, _b };
    write(command, 4);
    usleep(BYTE_TIME*4);
}

void ThermalPrinter::write(const uint8_t *_array, int _size){
    if(bConnected){
        port->write(_array, _size);
        usleep(BYTE_TIME*_size);
    }
}

void ThermalPrinter::close(){
    if(bConnected){
        port->close();
    }
}

// reset the printer
void ThermalPrinter::reset() {
    write(27,'@');
}

// sets the printer online (true) or ofline (false)
void ThermalPrinter::setStatus(bool state) {
    write(27,61,state);
}

// set control parameters: heatingDots, heatingTime, heatingInterval
void ThermalPrinter::setControlParameter(uint8_t heatingDots, uint8_t heatingTime, uint8_t heatingInterval) {
    write(27,55);
    write(heatingDots);
    write(heatingTime);
    write(heatingInterval);
}

// set sleep Time in seconds, time after last print the printer should stay awake
void ThermalPrinter::setSleepTime(uint8_t seconds) {
    write(27, 56, seconds);
    write(0xFF);
}

// set double width mode: on=true, off=false
void ThermalPrinter::setDoubleWidth(bool state) {
    write(27, state?14:20);
}


// set the print density and break time
void ThermalPrinter::setPrintDensity(uint8_t printDensity, uint8_t printBreakTime) {
    write(18, 35, (printBreakTime << 5) | printDensity );
}

// set the used character set
void ThermalPrinter::setCharacterSet(CharacterSet set) {
    write(27, 82, set);
}

// set the used code table
void ThermalPrinter::setCodeTable(CodeTable table) {
    write(27, 116, table);
}

// feed single line
void ThermalPrinter::feed(void) {
    write(10);
}

// feed <<lines>> lines
void ThermalPrinter::feed(uint8_t lines) {
    write(27, 74, lines);
}

// set line spacing
void ThermalPrinter::setLineSpacing(uint8_t spacing) {
    write(27, 51, spacing);
}

// set Align Mode: LEFT, MIDDLE, RIGHT
void ThermalPrinter::setAlign(AlignMode align) {
    write(27, 97, align);
}

// set how many blanks should be kept on the left side
void ThermalPrinter::setLeftBlankCharNums(uint8_t space) {
    if (space >= 47) space = 47;
    write(27, 66, space);
}

// set Bold Mode: on=true, off=false
void ThermalPrinter::setBold(bool state) {
    write(27, 32, (uint8_t)state);
    write(27, 69, (uint8_t)state);
}

// set Reverse printing Mode
void ThermalPrinter::setReverse(bool state) {
    write(29, 66, (uint8_t)state);
}

// set Up/Down Mode
void ThermalPrinter::setUpDown(bool state) {
    write(27, 123, (uint8_t)state);
}

// set Underline printing
void ThermalPrinter::setUnderline(bool state) {
    write(27, 45, (uint8_t) state);
}

// enable / disable the key on the frontpanel
void ThermalPrinter::setKeyPanel(bool state) {
    write( 27, 99, 53, (uint8_t) state );
}

// where should a readable barcode code be printed
void ThermalPrinter::setBarcodePrintReadable(PrintReadable n) {
    write(29, 72, n);
}

// sets the height of the barcode in pixels
void ThermalPrinter::setBarcodeHeight(uint8_t height) {
    if (height <= 1) height = 1;
    write(29, 104, height);
}

// sets the barcode line widths (only 2 or 3)
void ThermalPrinter::setBarCodeWidth(uint8_t width) {
    if (width <= 2) width=2;
    else if (width >= 3) width=3;
    
    write(29, 119, width);
}

void ThermalPrinter::print(const std::string& text){
    if(bConnected){
        port->write(text);
        usleep(BYTE_TIME*text.size());
    }
}


void ThermalPrinter::println(const std::string& text){
    print(text+"\n");
}

// prints a barcode
void ThermalPrinter::printBarcode(const std::string &data, BarcodeType type) {
    if(bConnected){
        write(29, 107, type);
        port->write(data);
        usleep(BYTE_TIME*data.size());
        write(0);
    }
}

float getBrightness(float _r,  float _g, float _b){
    return 0.212655 * _r + 0.715158 * _g + 0.072187 * _b;
}

void ThermalPrinter::printImg(const std::string &_path, int _threshold){

    int width = 0;
    int height = 0;
    int comp;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* pixels = stbi_load(_path.c_str(), &width, &height, &comp, STBI_rgb);
    
    int GrayArrayLength = width * height;
    unsigned char * GrayArray = new unsigned char[GrayArrayLength];
    memset(GrayArray,0,GrayArrayLength);
    
    for (int y = 0; y < height;y++) {
        std::vector<bool> data;
        for (int x = 0; x < width; x++) {
            int index = (y*width)*comp+x*comp;


            float brightTemp = 0.0;

            if (comp == 1 || comp == 2){
                brightTemp = pixels[index];
            } else if (comp == 3 || comp == 4 ){
                float r = (float)pixels[index]/255.0f;
                float g = (float)pixels[index+1]/255.0f;
                float b = (float)pixels[index+2]/255.0f;

                brightTemp = getBrightness(r,g,b)*255.0;
                if(brightTemp>255){
                    brightTemp = 255;
                }
            } 
            
            // Brightness correction curve:
            brightTemp =  sqrt(255) * sqrt (brightTemp);
            if (brightTemp > 255) brightTemp = 255;
            if (brightTemp < 0) brightTemp = 0;
            
            int darkness = 255 - floor(brightTemp);
            
            int idx = y*width + x;
            darkness += GrayArray[idx];
            
            if( darkness >= _threshold){
                darkness -= _threshold;
                data.push_back(true);
            } else {
                data.push_back(false);
            }
            
            int darkn8 = round(darkness / 8);
            
            // Atkinson dithering algorithm:  http://verlagmartinkoch.at/software/dither/index.html
            // Distribute error as follows:
            //     [ ]  1/8  1/8
            //1/8  1/8  1/8
            //     1/8
            
            if ((idx + 1) < GrayArrayLength)
                GrayArray[idx + 1] += darkn8;
            if ((idx + 2) < GrayArrayLength)
                GrayArray[idx + 2] += darkn8;
            if ((idx + width - 1) < GrayArrayLength)
                GrayArray[idx + width - 1] += darkn8;
            if ((idx + width) < GrayArrayLength)
                GrayArray[idx + width] += darkn8;
            if ((idx + width + 1) < GrayArrayLength)
                GrayArray[idx + width + 1 ] += darkn8;
            if ((idx + 2 * width) < GrayArrayLength)
                GrayArray[idx + 2 * width] += darkn8;
        }
        printPixelRow(data);
    }
    
    delete []GrayArray;
}

void ThermalPrinter::printPixelRow(std::vector<bool> _line){
    if(bConnected){
        int width = _line.size();
        if(width>384)
            width = 384;
        
        int rowBytes        = (width + 7) / 8;                 // Round up to next byte boundary
        uint8_t rowBytesClipped = (rowBytes >= 48) ? 48 : rowBytes; // 384 pixels max width
        
        uint8_t data[rowBytesClipped];
        memset(data,0x00,rowBytesClipped);
        
        for (int i = 0; i < width; i++) {
            uint8_t bit = 0x00;
            if (_line[i]){
                bit = 0x01;
            }
            data[i/8] += (bit&0x01)<<(7-i%8);
        }
        
        const uint8_t command[4] = {18, 42, 1, rowBytesClipped};
        port->write(command, 4);
        usleep(BYTE_TIME*4);
        
        port->write(data,rowBytesClipped);
        usleep(BYTE_TIME*rowBytesClipped);
    }
}



