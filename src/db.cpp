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

/* Row class definitions.
 */
Row::Row ()
{
}

vector<Field> Row::getFieldValues ()
{
    return field_val;
}

void Row::addField (Field f, string f_name)
{
    field_val.push_back (f);
    field_names.push_back (f_name);
}

void Row::write (ostream& out_t)
{
    int tmp;
    double tmp_d;
    char* tmp_c;
    for (int i = 0; i < field_val.size (); i++)
    {
        switch (field_val[i].getFieldType ())
        {
            case INTEGER:
                tmp = field_val[i].getIntFieldVal ();
                out_t.write (reinterpret_cast<const char *>(&tmp),
                             sizeof (tmp));
                break;
            case DECIMAL:
                tmp_d = field_val[i].getDoubleFieldVal ();
                out_t.write (reinterpret_cast<const char *>(&tmp_d),
                             sizeof (tmp_d));
                break;
            case CHAR_ARR:
                tmp_c = field_val[i].getCharFieldVal ();
                out_t.write (tmp_c, sizeof (tmp_c));
                break;
        }
    }
}

ostream& operator<< (ostream& out_t, Row& r)
{
    vector<Field> field_val = r.getFieldValues ();
    int tmp;
    double tmp_d;
    char* tmp_c;
    for (int i = 0; i < field_val.size (); i++)
    {
        switch (field_val[i].getFieldType ())
        {
            case INTEGER:
                tmp = field_val[i].getIntFieldVal ();
                out_t << tmp << "\t";
                break;
            case DECIMAL:
                tmp_d = field_val[i].getDoubleFieldVal ();
                out_t << tmp_d << "\t";
                break;
            case CHAR_ARR:
                tmp_c = field_val[i].getCharFieldVal ();
                out_t << tmp_c << "\t";
                break;
        }
    }
    out_t << "\n";
    return out_t;
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

Field::Field (FieldType f, int field_val)
{
    ft = f;
    field_int = field_val;
}

Field::Field (FieldType f, double field_val)
{
    ft = f;
    field_double = field_val;
}

Field::Field (FieldType f, char field_val[MAX_CHAR_SIZE])
{
    ft = f;
    strcpy (field_char, field_val);
}

FieldType Field::getFieldType ()
{
    return ft;
}

int Field::getIntFieldVal ()
{
    return field_int;
}

double Field::getDoubleFieldVal ()
{
    return field_double;
}

char* Field::getCharFieldVal ()
{
    return field_char;
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

vector<FieldType> Table::getFieldTypes ()
{
    return field_types;
}

vector<string> Table::getFieldNames ()
{
    return field_names;
}

Row Table::read (istream& in_t, Table t)
{
    int tmp;
    double tmp_d;
    char tmp_c[MAX_CHAR_SIZE];
    Row r;
    vector<FieldType> field_types = t.getFieldTypes ();
    vector<string> field_names = t.getFieldNames ();
    for (int i = 0; i < field_types.size (); i++)
    {
        switch (field_types[i])
        {
            case INTEGER:
              {
                in_t.read ((char*)&tmp, sizeof (tmp));
                Field fi (field_types[i], tmp);
                r.addField (fi, field_names[i]);
                break;
              }
            case DECIMAL:
              {
                in_t.read ((char*)&tmp_d, sizeof (tmp_d));
                Field fd (field_types[i], tmp_d);
                r.addField (fd, field_names[i]);
                break;
              }
            case CHAR_ARR:
              {
                in_t.read (tmp_c, sizeof (tmp_c));
                Field fc (field_types[i], tmp_c);
                r.addField (fc, field_names[i]);
                break;
              }
        }
    }
    return r;
}
