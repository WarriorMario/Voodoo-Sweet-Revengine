#pragma once
#include "VString.h"
#include <fstream>

struct File
{
  char* data;
  size_t size;
};

// reads everything in the file
bool LoadFileBinary(StringRef file_name, File* file_out)
{
  // open the file
  std::ifstream file(file_name.data(), std::ios::in | std::ios::binary | std::ios::ate);

  // check if the file is open
  if(file.is_open())
  {
    // allocate data
    file_out->size = file.tellg();
    file_out->data = new char[file_out->size];

    // return to the beginning of the file
    file.seekg(0, std::ios::beg);

    // read everything from the current point in the file
    file.read(file_out->data, file_out->size);

    // close the file
    file.close();

    // return happily
    return true;
  }
  else
  {
    // assert with a handy debug message
    assert(false && "Unable to load file.");

    // return sadly
    return false;
  }
}


// removes old file's contents
bool WriteFile(StringRef file_name, File* file)
{
  // open the file
  std::ofstream myfile(file_name.data(), std::ios::binary | std::ios::trunc);

  // check if it's open
  if(myfile.is_open())
  {
    // clear any contents is has
    myfile.clear();

    // write the data
    myfile.write(file->data, file->size);

    // close the file
    myfile.close();

    // return happily
    return true;
  }
  else
  {
    // something went wrong
    assert(false && "Unable to open file.");

    // return sadly
    return false;
  }
}