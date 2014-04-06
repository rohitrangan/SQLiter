/*
    SQLiter - db.h
    Author:- Rohit Rangan

    This file contains the declarations of the database class.
*/

#ifndef INCLUDE_DB_H_
#define INCLUDE_DB_H_

#include "../include/fileio.h"

#include <string>
#include <cstdio>
#include <vector>
#include <fstream>
#include <iostream>

#define MAX_CHAR_SIZE   101

using namespace std;

class Database
{
  private:
    string name;

  public:
    Database ();
    Database (string db_name);
    bool createDatabase ();
    static bool useDatabase (string db_name);
};

enum FieldType
{
    INTEGER,
    DECIMAL,
    CHAR_ARR
};

class Field
{
  private:
    FieldType ft;
    int field_int;
    double field_double;
    char field_char[MAX_CHAR_SIZE];

  public:
    Field ();
    Field (FieldType f, void* field_val);
};

class Table
{
  private:
    string name;
    string db_name;
    vector<string> field_names;
    vector<FieldType> field_types;

  public:
    Table ();
    Table (string t_name, string dbname, vector<string>& f_names,
           vector<FieldType>& f_types);
};

#endif  /* INCLUDE_DB_H_ */
