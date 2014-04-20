/*
    SQLiter - fileio.cpp
    Author:- Rohit Rangan

    This file contains the definitions of basic File and Folder I/O
    operations.
*/

#include "../include/fileio.h"

namespace fileio
{

bool mkdir (string dir_name)
{
    if (dir_name.length () == 0)
        return false;

    int rval = ::mkdir (dir_name.c_str (), S_IRWXU | S_IRWXG | S_IRWXO);
    return (rval == 0)?true:false;
}

bool rmdir (string dir_name)
{
    DIR* dp;
    struct dirent* entry;
    string path;
    dp = opendir (dir_name.c_str ());
    if (dp == NULL)
        return false;

    while ((entry = readdir (dp)))
    {
        path = dir_name + "/";
        if (entry->d_type == DT_DIR)
            continue;
        path += entry->d_name;
        if (remove (path.c_str ()) != 0)
            return false;
    }
    closedir (dp);

    int rval = remove (dir_name.c_str ());
    return (rval == 0)?true:false;
}

bool rmfile (string path)
{
    return (remove (path.c_str ()) == 0);
}

bool chdir (string path)
{
    int rval = ::chdir (path.c_str ());
    return (rval == 0)?true:false;
}

string getwd ()
{
    char wd_name[MAX_PATH_LEN];
    ::getcwd (wd_name, sizeof (wd_name));
    return string (wd_name);
}

template <class T>
void write_t (ostream& out_t, T& t, int pos)
{
    out_t.seekp (pos, ios_base::beg);
    t.write (out_t);
}

template <class T>
void read_t (istream& in_t, T& t, int pos)
{
    in_t.seekg (pos, ios_base::beg);
    t.read (in_t);
}

}
