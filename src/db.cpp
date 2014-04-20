/*
    SQLiter - db.cpp
    Author:- Rohit Rangan

    This file contains the definitions of the database class.
*/

#include "../include/db.h"

/* Database class definitions.
 */
bool Database::loadTableFromAttrFile (string fname)
{
    ifstream ip (fname.c_str (), ios::in);
    if (!ip.is_open ())
        return false;

    string t_name, db_name;
    string tmp;
    int f_len;
    int tmp_t;
    vector<string> f_names;
    vector<FieldType> f_types;
    ip >> t_name;
    ip >> db_name;
    ip >> f_len;
    for (int i = 0; i < f_len; i++)
    {
        ip >> tmp;
        f_names.push_back (tmp);
    }
    for (int i = 0; i < f_len; i++)
    {
        ip >> tmp_t;
        switch (tmp_t)
        {
            case INTEGER:
                f_types.push_back (INTEGER);
                break;
            case DECIMAL:
                f_types.push_back (DECIMAL);
                break;
            case CHAR_ARR:
                f_types.push_back (CHAR_ARR);
                break;
        }
    }
    Table t (t_name, db_name, f_names, f_types);
    tables.push_back (t);
    return true;
}

Database::Database ()
{
    name = "";
}

Database::Database (string db_name) : name (db_name)
{
}

bool Database::createDatabase ()
{
    bool rval = fileio::chdir (MASTER_WD);
    if (!rval)
        return rval;

    return fileio::mkdir (name);
}

bool Database::loadTables ()
{
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir (".")) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            string fname (ent->d_name);
            //cout << fname << "\n";
            size_t place = fname.find (".");
            if (place == string::npos)
                continue;
            if (fname.substr (place, string::npos) == ".attr")
            {
                bool rval = loadTableFromAttrFile (fname);
                if (!rval)
                    return rval;
            }
        }
        closedir (dir);
    }
    else
        return false;

    return true;
}

Table Database::getTableFromName (string t_name)
{
    for (int i = 0; i < tables.size (); i++)
    {
        if (tables[i].getTableName () == t_name)
            return tables[i];
    }
    return Table ();
}

string Database::getDatabaseName ()
{
    return name;
}

void Database::addTable (Table t)
{
    Table tmp = getTableFromName (t.getTableName ());
    if (tmp.isGood ())
        return;

    tables.push_back (t);
}

bool Database::useDatabase (Database db)
{
    string db_name = db.getDatabaseName ();
    bool rval = fileio::chdir (MASTER_WD);
    if (!rval)
        return rval;

    rval = fileio::chdir (db_name);
    if (!rval)
        return rval;

    return db.loadTables ();
}

bool Database::deleteDatabase (Database db)
{
    bool rval = fileio::chdir (MASTER_WD);
    if (!rval)
        return false;

    return fileio::rmdir (db.getDatabaseName ());
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
                //out_t.write (tmp_c, sizeof (tmp_c));
                out_t.write (tmp_c, MAX_CHAR_SIZE);
                break;
        }
    }
}

bool Row::isGood ()
{
    return (field_val.size () != 0);
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

Field::Field (FieldType f, const char* field_val)
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
bool Table::checkAttrFilePresent ()
{
    string attr_name = name + ".attr";
    return fileio::fileExists (attr_name);
}

void Table::writeTableToAttrFile ()
{
    string attr_name = name + ".attr";
    ofstream op (attr_name.c_str (), ios::out);
    op << name << endl;
    op << db_name << endl;
    op << field_names.size () << endl;
    for (int i = 0; i < field_names.size (); i++)
        op << field_names[i] << endl;
    for (int i = 0; i < field_types.size (); i++)
        op << field_types[i] << endl;
}

void Table::setSeekSize ()
{
    if (seek_size != -1)
        return;

    ifstream in_t (name.c_str (), ios::in | ios::binary);
    int tmp;
    double tmp_d;
    char tmp_c[MAX_CHAR_SIZE];
    seek_size = 0;
    for (int i = 0; i < field_types.size (); i++)
    {
        switch (field_types[i])
        {
            case INTEGER:
              {
                in_t.read ((char*)&tmp, sizeof (tmp));
                Field fi (field_types[i], tmp);
                seek_size += sizeof (fi.getIntFieldVal ());
                break;
              }
            case DECIMAL:
              {
                in_t.read ((char*)&tmp_d, sizeof (tmp_d));
                Field fd (field_types[i], tmp_d);
                seek_size += sizeof (fd.getDoubleFieldVal ());
                break;
              }
            case CHAR_ARR:
              {
                in_t.read (tmp_c, sizeof (tmp_c));
                Field fc (field_types[i], tmp_c);
                seek_size += MAX_CHAR_SIZE;
                //seek_size += sizeof (fc.getCharFieldVal ());
                break;
              }
        }
    }
    in_t.close ();
}

void Table::setFileSize ()
{
    if (file_size != -1)
        return;

    ifstream ip (name.c_str (), ios::in | ios::binary | ios::ate);
    file_size = (int) ip.tellg ();
    ip.close ();
}

Table::Table ()
{
    curr_pos = 0;
    seek_size = -1;
    file_size = -1;
}

Table::Table (string t_name, string dbname, vector<string>& f_names,
              vector<FieldType>& f_types) : name (t_name), db_name (dbname)
{
    curr_pos = 0;
    seek_size = -1;
    file_size = -1;
    field_names = f_names;
    field_types = f_types;
    if (!checkAttrFilePresent ())
        writeTableToAttrFile ();
}

string Table::getTableName ()
{
    return name;
}

string Table::getDatabaseName ()
{
    return db_name;
}

vector<FieldType> Table::getFieldTypes ()
{
    return field_types;
}

vector<string> Table::getFieldNames ()
{
    return field_names;
}

Row Table::getNextRow ()
{
    setSeekSize ();
    setFileSize ();
    if (curr_pos >= file_size)
    {
        curr_pos = 0;
        Row r;
        return r;
    }
    ifstream in_t (name.c_str (), ios::in | ios::binary);
    in_t.seekg (curr_pos);

    Row r;
    int tmp;
    double tmp_d;
    char tmp_c[MAX_CHAR_SIZE];
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
                //in_t.read (tmp_c, sizeof (tmp_c));
                in_t.read (tmp_c, MAX_CHAR_SIZE);
                Field fc (field_types[i], tmp_c);
                r.addField (fc, field_names[i]);
                break;
              }
        }
    }
    in_t.close ();
    curr_pos += seek_size;
    return r;
}

bool Table::isGood ()
{
    return (field_names.size () != 0);
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
                //in_t.read (tmp_c, sizeof (tmp_c));
                in_t.read (tmp_c, MAX_CHAR_SIZE);
                Field fc (field_types[i], tmp_c);
                r.addField (fc, field_names[i]);
                break;
              }
        }
    }
    return r;
}

bool Table::deleteTable (string table_name)
{
    bool rval = fileio::rmfile (table_name);
    if (!rval)
        return rval;

    return fileio::rmfile (table_name + ".attr");
}
