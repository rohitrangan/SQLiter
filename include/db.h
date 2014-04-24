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
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>


/* Maximum size of the CHAR field type.
 */
#define MAX_CHAR_SIZE   101

using namespace std;


/* We support only three field types; INTEGER, DOUBLE and CHAR. CHAR
 * here is an array of characters of maximum length 101.
 */
enum FieldType
{
    INT_VAL = 0,
    DOUBLE_VAL,
    CHAR_ARR
};

/* Definition for a field. We refer to a column (consisting of a single row)
 * as a field.
 */
class Field
{
  private:
    /* Type of field.
     */
    FieldType ft;

    /* Variable to store the field value if it is an integer.
     */
    int field_int;

    /* Variable to store the field value if it is a double.
     */
    double field_double;

    /* Variable to store the field value if it is a character array.
     */    
    char field_char[MAX_CHAR_SIZE];

  public:
    /* Default Constructor.
     */
    Field ();

    /* Constructor for a Field of type integer.
     */
    Field (FieldType f, int field_val);

    /* Constructor for a field of type double.
     */
    Field (FieldType f, double field_val);

    /* Constructor for a field of type char.
     */
    Field (FieldType f, const char* field_val);

    /* Used to get the field type, so that we can call the appropriate
     * functions to get the data type.
     */
    FieldType getFieldType ();

    /* Returns the integer field value.
     */
    int getIntFieldVal ();

    /* Returns the double field value.
     */
    double getDoubleFieldVal ();

    /* Returns the char field value.
     */
    char* getCharFieldVal ();

    /* Sets the integer field value.
     */
    void setIntFieldVal (int field_val);

    /* Sets the double field value.
     */
    void setDoubleFieldVal (double field_val);

    /* Sets the char field value.
     */
    void setCharFieldVal (const char* field_val);
};


/* Definition for a Row. A row in the table consists of an array of fields.
 */
class Row
{
  private:
    /* The array of Fields.
     */
    vector<Field> field_val;

    /* The array of field names.
     */
    vector<string> field_names;

  public:
    /* Default Constructor.
     */
    Row ();

    /* Returns the Field vector.
     */
    vector<Field> getFieldValues ();

    /* Returns a field given its name.
     */
    Field getFieldByName (string f_name);

    /* Inserts a field into the Row.
     */
    void addField (Field f, string f_name);

    /* Writes the field into the output stream passed as a parameter.
     * The output stream should preferably be a binary file.
     */
    void write (ostream& out_t);

    /* Checks whether the Row object actually contains any Fields.
     */
    bool isGood ();

    /* Replaces a field with the given name with the one passed as a
     * parameter.
     */
    void replaceField (Field f, string f_name);

    /* Overloaded operator which prints the contents of a row, with
     * tab spaces between the fields.
     */
    friend ostream& operator<< (ostream& out_t, Row& r);
};


/* Definitions for a Table. The table here does not contain a vector of rows,
 * as very large databases cannot be stored in memory. Instead, we just store
 * the table name as well as the database name. Since out table name is a key
 * to the table file, we use the table object to read rows when queries are
 * executed.
 *
 * We create two files for every table; one contains the actual data while the
 * other is an attributes file (with the suffix ".attr"). This attributes file
 * stores all the fields required to instantiate a Table object at runtime.
 */
class Table
{
  private:
    /* Table name.
     */
    string name;

    /* Database name.
     */
    string db_name;

    /* Value of the i'th element is 'true' if the i'th element it has been
     * deleted. Else its values is false.
     */
    vector<bool> isDeleted;

    /* Field names.
     */
    vector<string> field_names;

    /* Field types.
     */
    vector<FieldType> field_types;

    /* Current position in the table file. Used only when we need to search
     * for records.
     */
    int curr_pos;

    /* Size of one row, in bytes.
     */
    int seek_size;

    /* Size of the table file.
     */
    int file_size;

    /* Checks whether the attributes file for the table is present.
     */
    bool checkAttrFilePresent ();

    /* Writes all the required fields of the table to the attributes file.
     */
    void writeTableToAttrFile ();

    /* Sets the row size for the table.
     */
    void setSeekSize ();

    /* Sets the file size for the table.
     */
    void setFileSize ();

  public:
    /* Default Constructor.
     */
    Table ();

    /* Constructor which takes the table name, database name, field names
     * and types to instantiate the table object.
     */
    Table (string t_name, string dbname, vector<string>& f_names,
           vector<FieldType>& f_types);

    /* Destructor, used for testing.
     */
    ~Table ();

    /* Returns the table name.
     */
    string getTableName ();

    /* Returns the database name.
     */
    string getDatabaseName ();

    /* Returns the field types.
     */
    vector<FieldType> getFieldTypes ();

    /* Returns the field names.
     */
    vector<string> getFieldNames ();

    /* Returns the vector isDeleted.
     */
    vector<bool> getDeletedField ();

    /* Sets the vector isDeleted to del.
     */
    void setDeletedField (vector<bool> del);

    /* Gets the next row from the table file, which has not been deleted.
     */
    Row getNextRow ();

    /* Gets the current position in the table file.
     */
    int getCurrentPosition ();

    /* Gets the size of one row.
     */
    int getSeekSize ();

    /* Gets the current record's position in the file number.
     */
    int getCurrRecordNum ();

    /* Checks whether the Table object actually exists.
     */
    bool isGood ();

    /* Writes the Row to the file tb_name. We mostly append to the file.
     */
    void write (string tb_name, Row r);

    /* Reads a row from the given input stream. This function is no longer
     * used due to the more robust getNextRow () function.
     */
    static Row read (istream& in_t, Table t);
};

/* Our main class. A database consists of a vector of Table objects. Whenever
 * "USE" a database, the Table vector is populated using each table's attribute
 * file. Every database is a folder, consisting of a file "._is_database" which
 * tells us that the folder is a database. This file contains no data. It is
 * somewhat like a flag which separates the folders which are databases to ones
 * that aren't.
 *
 * The user must ensure that there are no sub-folders inside a database folder,
 * as this may lead to errors. There is no limit to the number of tables that
 * a database can have.
 *
 * NOTE: Once the program starts, all databases are created in the directory
 *       in which the program is executing, i.e. the CURRENT DIRECTORY.
 */
class Database
{
  private:
    /* Database name.
     */
    string name;

    /* A vector containing all the tables of the given database.
     */
    vector<Table> tables;

    /* Loads the table from the given table name, using its attribute file.
     */
    bool loadTableFromAttrFile (string fname);

  public:
    /* Default Constructor.
     */
    Database ();

    /* We pass the database name to this constructor. This is the usual way to
     * create a new database object.
     */
    Database (string db_name);

    /* Creates the database (creates the folder and the "._is_database" file).
     */
    bool createDatabase ();

    /* Loads all the tables from the database folder using their ".attr" files.
     */
    bool loadTables ();

    /* Tells us whether a given file/folder name is a database.
     */
    bool isDatabase (string fname);

    /* Gets a Table object given a table name.
     */
    Table getTableFromName (string t_name);

    /* Gets all the tables that make up the database.
     */
    vector<Table> getAllTables ();

    /* Gets the database name.
     */
    string getDatabaseName ();

    /* Adds a table to the existing database.
     */
    void addTable (Table t);

    /* Removes a table from the existing database, given a table name.
     */
    bool removeTable (string table_name);

    /* Checks whether a table with the given name exists.
     */
    bool tableExists (string table_name);

    /* Replaces a table object with the given table name by t. This is done
     * when a table object has some fields that get modified.
     */
    void modifyTable (Table t, string table_name);

    /* This function changes directory to the database directory and loads all
     * the table objects into memory.
     */
    static bool useDatabase (Database& db);

    /* Deletes the given database object.
     */
    static bool deleteDatabase (Database& db);

    /* Deletes the given table from the database object.
     */
    static bool deleteTable (Database& db, string table_name);

    /* Shows all the tables contained in the database object.
     */
    static void showTables (Database& db);

    /* Shows all the databases in the current directory.
     */
    static void showDatabases (Database& db);

    /* Function which evaluates the SELECT query. We give a table name,
     * a single column name, a comparison operator and the value which we
     * need to compare it with. We return a vector of Rows.
     *
     * NOTE: We only support ONE column name in the SELECT queries and we
     *       return ALL the columns of the given table.
     */
    static vector<Row> selectFromTable (Database& db, char* table_name,
                                        char* col_name, const char* oper,
                                        const char* val);

    /* Function which prints the result of a SELECT query.
     */
    static void printSelectResult (Database& db, char* table_name,
                                   vector<Row> res);

    /* Function which evaluates the UPDATE query. We give a table name, the
     * column name which need to be updated (col_name1), the new column value
     * (val1), and the parameters required for the WHERE clause.
     *
     * NOTE: We only support update of ONE column name.
     */
    static void updateTable (Database& db, char* table_name, char* col_name1,
                             const char* val1, char* col_name2,
                             const char* oper, const char* val2);

    /* Function which evaluates the DELETE query. We give a table name and the
     * parameters required for the WHERE clause.
     */
    static void deleteFromTable (Database& db, char* table_name,
                                 char* col_name, const char* oper,
                                 const char* val);
};

#endif  /* INCLUDE_DB_H_ */
