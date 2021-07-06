#include <iostream>
#include "framesequence.h"
#include "action.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "coordinates.h"
#include "assignment.h"// implementation of methods
namespace FrameSequence {
    //no arg constructor
FrameSequence::FrameSequence(void){
} 

//destructor
FrameSequence::~FrameSequence(){
    //size of rows is equal to frame width
    int arrSize = Object.width;
    for(int x = 0; x<imageSequence.size();x++){
        for( int f = 0 ; f<arrSize; f++){
            //deallocate memory for each pointer inner arrays
            delete [] imageSequence[x][f];
        }
        //deallocate memory for each pointer array for each sequence 
        delete [] imageSequence[x];
    }

}
//string for printing the header information
std::string FrameSequence::headerGen(structure::Coordinates obj){
    std::string header = "P5\n"+std::to_string(obj.width)+ " "+std::to_string(obj.height)+"\n255\n";
     return header;           

}
//read the file from the command line inputs
void FrameSequence::read(std::string filename, structure::Coordinates o){
    Object = o;
    std::vector<char> bytes;
    char byte = 0;
    int start_index;
    //line is defined by y = mx + c
    //find the slope from the coordinates
    mdnave001::findSlope(o);
    //find the c-value 
    mdnave001::findCVal(o);
    //open the file
    std::ifstream input_file(filename);
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '"
             << filename << "'" << std::endl;
        
    }
     
    int index = 0;
    bool foundStart = false;
    //read all the chars in the file into a 1d vector
    while (input_file.get(byte)) {
        index++;
        bytes.push_back(byte);
        if(byte=='5'){
            if(bytes[index-1]=='5'){
                if(bytes[index-2]=='2'  && foundStart==false){
                    start_index = index + 1;
                    foundStart = true;
                }
            }
        }
    }
    //rows and cols for the image
    std::string rows, cols;
    int space_count = 0;
    int last_col_char = start_index-5;

    //extract the rows and the cols from the file byte by byte
    while(space_count<2){
        if(bytes[last_col_char]==' ' || bytes[last_col_char]=='\n'){
            space_count++;
            last_col_char--;
            continue;
        }
        if(space_count<1){
            cols = bytes[last_col_char]+cols;
        }
        else{
            rows = bytes[last_col_char]+rows;
        }
        last_col_char--;
    }
    
    
    std::stringstream rowify(rows),colify(cols);
    
    int nrows = 0;
    int ncols = 0;
    rowify >> nrows;
    colify >> ncols;
    start_index = start_index+2;
    input_file.close();
    //create a 2d array from the 1d char vector
    unsigned char ** picture= new  unsigned char*[nrows];
    for(int xval=0; xval<nrows;++xval ){
        picture[xval] = new  unsigned char[ncols];
        for(int y = 0; y<ncols ; y++){
            *(picture[xval]+y) = bytes[start_index++];
        }
    } 
    //exctract the frame images from the 2d array
    int sequencenumber = 0;
    for(int xval=0; xval<nrows;xval++ ){
        for(int y = 0; y<ncols ; y++){
            if(xval>=o.x1 && xval<o.x2 && mdnave001::onLine(o,xval,y)){
                //add the current frame image to the frame sequence
                imageSequence.push_back(new unsigned char*[o.width]);
                for(int index_x = xval; index_x<xval+o.width;index_x++){
                    //
                    imageSequence[sequencenumber][index_x-xval] = new unsigned char[o.height];
                    for(int index_y = y; index_y<y+o.height;index_y++){
                        imageSequence[sequencenumber][index_x-xval][index_y-y]=picture[index_x][index_y];
                    }
                }
                sequencenumber++;
            }
           
        }
        
    }
    
    //deallocate the memory for the image exctracted from the 1d char vector
    for( int f = 0 ; f<nrows; f++){
       delete [] picture[f];
    }
    delete [] picture;
    
  
    
 
}
//make the filenumbers at least 4 digits long
std::string FrameSequence::numGen(int filenumber){
    std::string fourNumString;
    if(filenumber<10){
        fourNumString ="000" +std::to_string(filenumber);
    }
    else if(filenumber<100){
        fourNumString ="00" +std::to_string(filenumber);
    }
    else if(filenumber<1000){
        fourNumString ="0" +std::to_string(filenumber);
    }
    else{
        fourNumString = std::to_string(filenumber);
    }
    return fourNumString;
}
// function for none option
//all functions that follow have the same algorithm 

void FrameSequence::none(actions::Action action){
    //for each frame image in the sequence output the sequence to the corresponding sequence number
    
        for(int filenumber = 0; filenumber<imageSequence.size();filenumber++){
            //open the file corresponding to the file number
            std::ofstream file(action.filename+numGen(filenumber)+".pgm");
            //append the header to the open file
            file << headerGen(Object);
            for(int index_x = 0; index_x<Object.width;index_x++){
                for(int index_y = 0; index_y<Object.height;index_y++){
                            //append the characters to the file
                            file<<char(imageSequence[filenumber][index_x][index_y]);
                }
            }
            //close the file
            file.close();
        }
}
void FrameSequence::reverse(actions::Action action){
        for(int filenumber = imageSequence.size()-1; filenumber>-1;filenumber--){
            std::ofstream file(action.filename+numGen(filenumber)+".pgm");
            file<< headerGen(Object);
            for(int index_x = 0; index_x<Object.width;index_x++){
                for(int index_y = 0; index_y<Object.height;index_y++){
                            file<<char(imageSequence[filenumber][index_x][index_y]);
                }
            }
            file.close();
        }
}
void FrameSequence::reinvert(actions::Action action){
        for(int filenumber = imageSequence.size()-1; filenumber>-1;filenumber--){
            std::ofstream file(action.filename+numGen(filenumber)+".pgm");
            file<< headerGen(Object);
            //start from the end of the file
            for(int index_x = 0; index_x<Object.width;index_x++){
                for(int index_y = 0; index_y<Object.height;index_y++){
                    //invert  the pixel at that point
                            file<<char(255-imageSequence[filenumber][index_x][index_y]);
                }
            }
            file.close();
        }
}
void FrameSequence::invert(actions::Action action){
        for(int filenumber = 0; filenumber<imageSequence.size();filenumber++){
            std::ofstream file(action.filename+numGen(filenumber)+".pgm");
            file<< headerGen(Object);
            for(int index_x = 0; index_x<Object.width;index_x++){
                for(int index_y = 0; index_y<Object.height;index_y++){
                            file<<char(255-imageSequence[filenumber][index_x][index_y]);
                }
            }
            file.close();
        }
}

} // end namespace