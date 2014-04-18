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
    Field (FieldType f, int field_val);
    Field (FieldType f, double field_val);
    Field (FieldType f, char field_val[MAX_CHAR_SIZE]);
    FieldType getFieldType ();
    int getIntFieldVal ();
    double getDoubleFieldVal ();
    char* getCharFieldVal ();
};

class Row
{
  private:
    vector<Field> field_val;
    vector<string> field_names;

  public:
    Row ();
    vector<Field> getFieldValues ();
    void addField (Field f, string f_name);
    void write (ostream& out_t);
    friend ostream& operator<< (ostream& out_t, Row& r);
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
    vector<FieldType> getFieldTypes ();
    vector<string> getFieldNames ();
    static Row read (istream& in_t, Table t);
};

#endif  /* INCLUDE_DB_H_ */
