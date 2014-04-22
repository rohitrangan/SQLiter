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
    void setIntFieldVal (int field_val);
    void setDoubleFieldVal (double field_val);
    void setCharFieldVal (const char* field_val);
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
    void replaceField (Field f, string f_name);
    friend ostream& operator<< (ostream& out_t, Row& r);
};

class Table
{
  private:
    string name;
    string db_name;
    vector<bool> isDeleted;
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
    ~Table ();
    string getTableName ();
    string getDatabaseName ();
    vector<FieldType> getFieldTypes ();
    vector<string> getFieldNames ();
    vector<bool> getDeletedField ();
    void setDeletedField (vector<bool> del);
    Row getNextRow ();
    int getCurrentPosition ();
    int getSeekSize ();
    int getCurrRecordNum ();
    bool isGood ();
    void write (string tb_name, Row r);
    static Row read (istream& in_t, Table t);
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
    bool isDatabase (string fname);
    Table getTableFromName (string t_name);
    vector<Table> getAllTables ();
    string getDatabaseName ();
    void addTable (Table t);
    bool removeTable (string table_name);
    bool tableExists (string table_name);
    void modifyTable (Table t, string table_name);
    static bool useDatabase (Database& db);
    static bool deleteDatabase (Database& db);
    static bool deleteTable (Database& db, string table_name);
    static void showTables (Database& db);
    static void showDatabases (Database& db);
    static vector<Row> selectFromTable (Database& db, char* table_name,
                                        char* col_name, const char* oper,
                                        const char* val);
    static void printSelectResult (Database& db, char* table_name,
                                   vector<Row> res);
    static void updateTable (Database& db, char* table_name, char* col_name1,
                             const char* val1, char* col_name2,
                             const char* oper, const char* val2);
    static void deleteFromTable (Database& db, char* table_name,
                                 char* col_name, const char* oper,
                                 const char* val);
};

#endif  /* INCLUDE_DB_H_ */
