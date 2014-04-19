/*
    SQLiter - fileio.h
    Author:- Rohit Rangan

    This file contains the declarations of basic File and Folder I/O
    operations.
*/

#ifndef INCLUDE_FILEIO_H_
#define INCLUDE_FILEIO_H_

#include <string>
#include <cstdio>
#include <fstream>
#include <iostream>

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_PATH_LEN    256

using namespace std;

namespace fileio
{

bool mkdir (string dir_name);

bool rmdir (string dir_name);

bool chdir (string path);

string getwd ();

template <class T>
void write_t (ostream& out_t, T& t, int pos = 0);

template <class T>
void read_t (istream& in_t, T& t, int pos = 0);

inline bool fileExists (string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

}

#endif  /* INCLUDE_FILEIO_H_ */
