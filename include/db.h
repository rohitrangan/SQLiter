/*
    SQLiter - db.h
    Author:- Rohit Rangan

    This file contains the declarations of the database class.
*/

#ifndef INCLUDE_DB_H_
#define INCLUDE_DB_H_

#include "../include/wd.h"
#include "../include/fileio.h"

#include <dirent.h>

#include <string>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <iostream>

#define MAX_CHAR_SIZE   101

using namespace std;

enum FieldType
{
    INT_VAL = 0,
    DOUBLE_VAL,
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
    Field (FieldType f, const char* field_val);
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
    Field getFieldByName (string f_name);
    void addField (Field f, string f_name);
    void write (ostream& out_t);
    bool isGood ();
    friend ostream& operator<< (ostream& out_t, Row& r);
};

class Table
{
  private:
    string name;
    string db_name;
    vector<string> field_names;
    vector<FieldType> field_types;
    int curr_pos;
    int seek_size;
    int file_size;

    bool checkAttrFilePresent ();
    void writeTableToAttrFile ();
    void setSeekSize ();
    void setFileSize ();

  public:
    Table ();
    Table (string t_name, string dbname, vector<string>& f_names,
           vector<FieldType>& f_types);
    string getTableName ();
    string getDatabaseName ();
    vector<FieldType> getFieldTypes ();
    vector<string> getFieldNames ();
    Row getNextRow ();
    bool isGood ();
    static Row read (istream& in_t, Table t);
    static bool deleteTable (string table_name);
};

class Database
{
  private:
    string name;
    vector<Table> tables;

    bool loadTableFromAttrFile (string fname);

  public:
    Database ();
    Database (string db_name);
    bool createDatabase ();
    bool loadTables ();
    Table getTableFromName (string t_name);
    string getDatabaseName ();
    void addTable (Table t);
    static bool useDatabase (Database db);
    static bool deleteDatabase (Database db);
    static void showTables (Database db);
    static void showDatabases ();
    static vector<Row> selectFromTable (Database db, char* table_name,
                                        char* col_name, char* oper, char* val);
};

#endif  /* INCLUDE_DB_H_ */
