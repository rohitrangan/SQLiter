/*
    SQLiter - db.cpp
    Author:- Rohit Rangan

    This file contains the definitions of the database class.
*/

#include "../include/db.h"

/* Database class definitions.
 */
Database::Database ()
{
    name = "";
}

Database::Database (string db_name) : name (db_name)
{
}

bool Database::createDatabase ()
{
    return fileio::mkdir (name);
}

bool Database::useDatabase (string db_name)
{
    return fileio::chdir (db_name);
}

/* Field class definitions.
 */
Field::Field ()
{
    ft = INTEGER;
    field_int = 0;
    field_double = 0.0;
    field_char[0] = '\0';
}

Field::Field (FieldType f, void* field_val)
{
    ft = f;
    switch (ft)
    {
        case INTEGER:
            field_int = *((int*) field_val);
            break;
        case DECIMAL:
            field_double = *((double*) field_val);
            break;
        case CHAR_ARR:
            char* tmp_var = (char*) field_val;
            strcpy (field_char, tmp_var);
            break;
    }
}

/* Table class definitions.
 */
Table::Table ()
{
}

Table::Table (string t_name, string dbname, vector<string>& f_names,
              vector<FieldType>& f_types) : name (t_name), db_name (dbname)
{
    field_names = f_names;
    field_types = f_types;
}
