/*
    SQLiter - test_fileio.cpp

    This file contains the tests for the fileio functions.
*/

#include "../include/fileio.h"

#include <iostream>

using namespace std;

int main ()
{
    string dname ("tmp_dir");
    bool rval = fileio::mkdir (dname);
    if (rval)
        cout << "Directory with name - " << dname << " created.\n\n";
    else
        cout << "Directory with name - " << dname << " not created.\n\n";

    rval = fileio::chdir (dname);
    if (rval)
        cout << "Directory changed to - " << fileio::getwd () << "\n\n";
    else
        cout << "Directory not changed\n\n";

    fileio::chdir ("..");

    rval = fileio::rmdir (dname);
    if (rval)
        cout << "Directory with name - " << dname << " deleted.\n\n";
    else
        cout << "Directory with name - " << dname << " not deleted.\n\n";

    return 0;
}
