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
    int d_len;
    int tmp_t;
    bool tmp_b;
    vector<bool> is_del;
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
            case INT_VAL:
                f_types.push_back (INT_VAL);
                break;
            case DOUBLE_VAL:
                f_types.push_back (DOUBLE_VAL);
                break;
            case CHAR_ARR:
                f_types.push_back (CHAR_ARR);
                break;
        }
    }
    ip >> d_len;
    for (int i = 0; i < d_len; i++)
    {
        ip >> tmp_b;
        is_del.push_back (tmp_b);
    }
    Table t (t_name, db_name, f_names, f_types);
    t.setDeletedField (is_del);
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

    if (!fileio::mkdir (name))
        return false;

    if (!fileio::chdir (name))
        return false;

    ofstream op ("._is_database", ios::out);
    op.close ();

    if (!fileio::chdir (MASTER_WD))
        return false;

    return true;
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

bool Database::isDatabase (string fname)
{
    string currwd = fileio::getwd ();
    fileio::chdir (MASTER_WD);
    fileio::chdir (fname);
    string to_check ("._is_database");
    if (fileio::fileExists (to_check))
        return true;
    else
        return false;
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

vector<Table> Database::getAllTables ()
{
    return tables;
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

bool Database::removeTable (string table_name)
{
    int i;
    for (i = 0; i < tables.size (); i++)
    {
        if (tables[i].getTableName () == table_name)
        {
            break;
        }
    }
    if (i > tables.size ())
        return false;
    tables.erase (tables.begin () + i);
    return true;
}

bool Database::tableExists (string table_name)
{
    for (int i = 0; i < tables.size (); i++)
    {
        if (tables[i].getTableName () == table_name)
            return true;
    }
    return false;
}

void Database::modifyTable (Table t, string table_name)
{
    for (int i = 0; i < tables.size (); i++)
    {
        if (tables[i].getTableName () == table_name)
        {
            tables[i] = t;
            return;
        }
    }
}

bool Database::useDatabase (Database& db)
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

bool Database::deleteDatabase (Database& db)
{
    bool rval = fileio::chdir (MASTER_WD);
    if (!rval)
        return false;

    return fileio::rmdir (db.getDatabaseName ());
}

bool Database::deleteTable (Database& db, string table_name)
{
    if (!fileio::rmfile (table_name))
        return false;

    if (!fileio::rmfile (table_name + ".attr"))
        return false;

    return db.removeTable (table_name);
}

void Database::showTables (Database& db)
{
    vector<Table> all_t = db.getAllTables ();
    cout << "Database " << db.getDatabaseName () << "\n\n";
    for (int i = 0; i < all_t.size (); i++)
        cout << all_t[i].getTableName () << "\n";
    /*string db_name = db.getDatabaseName ();
    string cwdir = fileio::getwd ();
    fileio::chdir (MASTER_WD);
    fileio::chdir (db_name);
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir (".")) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            if (ent->d_type != DT_REG)
                continue;
            string fname (ent->d_name);
            size_t place = fname.find (".");
            if (place != string::npos)
                continue;
            if (fname.substr (place, string::npos) == ".attr")
                continue;
            cout << fname << endl;
        }
        closedir (dir);
        fileio::chdir (cwdir);
    }
    else
    {
        fileio::chdir (cwdir);
        return;
    }*/
}

void Database::showDatabases (Database& db)
{
    cout << "Databases\n\n";
    string cwdir = fileio::getwd ();
    fileio::chdir (MASTER_WD);
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir (".")) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            if (ent->d_type != DT_DIR)
                continue;
            string fname (ent->d_name);
            size_t place = fname.find (".");
            if (place != string::npos)
                continue;
            if (db.isDatabase (fname))
                cout << fname << endl;
        }
        closedir (dir);
        fileio::chdir (cwdir);
    }
    else
    {
        fileio::chdir (cwdir);
        return;
    }
}

vector<Row> Database::selectFromTable (Database& db, char* table_name,
                                       char* col_name, const char* oper,
                                       const char* val)
{
    Table t = db.getTableFromName (string (table_name));
    vector<Row> select_reply;
    Row r1 = t.getNextRow ();
    string oper_s;
    if (oper)
        oper_s = oper;
    while (r1.isGood ())
    {
        Field f;
        if (col_name)
            f = r1.getFieldByName (string (col_name));
        if (oper_s == "<")
        {
            switch (f.getFieldType ())
            {
                case INT_VAL:
                  {
                    int tmp_val = atoi (val);
                    if (f.getIntFieldVal () < tmp_val)
                        select_reply.push_back (r1);
                    break;
                  }
                case DOUBLE_VAL:
                  {
                    double tmp_d = atof (val);
                    if (f.getDoubleFieldVal () < tmp_d)
                        select_reply.push_back (r1);
                    break;
                  }
                case CHAR_ARR:
                  {
                    string tmp_c (val);
                    if (tmp_c < string (f.getCharFieldVal ()))
                        select_reply.push_back (r1);
                    break;
                  }
            }
        }
        else if (oper_s == "<=")
        {
            switch (f.getFieldType ())
            {
                case INT_VAL:
                  {
                    int tmp_val = atoi (val);
                    if (f.getIntFieldVal () <= tmp_val)
                        select_reply.push_back (r1);
                    break;
                  }
                case DOUBLE_VAL:
                  {
                    double tmp_d = atof (val);
                    if (f.getDoubleFieldVal () <= tmp_d)
                        select_reply.push_back (r1);
                    break;
                  }
                case CHAR_ARR:
                  {
                    string tmp_c (val);
                    if (tmp_c <= string (f.getCharFieldVal ()))
                        select_reply.push_back (r1);
                    break;
                  }
            }
        }
        else if (oper_s == "==")
        {
            switch (f.getFieldType ())
            {
                case INT_VAL:
                  {
                    int tmp_val = atoi (val);
                    if (f.getIntFieldVal () == tmp_val)
                        select_reply.push_back (r1);
                    break;
                  }
                case DOUBLE_VAL:
                  {
                    double tmp_d = atof (val);
                    if (f.getDoubleFieldVal () == tmp_d)
                        select_reply.push_back (r1);
                    break;
                  }
                case CHAR_ARR:
                  {
                    string tmp_c (val);
                    if (tmp_c == string (f.getCharFieldVal ()))
                        select_reply.push_back (r1);
                    break;
                  }
            }
        }
        else if (oper_s == ">")
        {
            switch (f.getFieldType ())
            {
                case INT_VAL:
                  {
                    int tmp_val = atoi (val);
                    if (f.getIntFieldVal () > tmp_val)
                        select_reply.push_back (r1);
                    break;
                  }
                case DOUBLE_VAL:
                  {
                    double tmp_d = atof (val);
                    if (f.getDoubleFieldVal () > tmp_d)
                        select_reply.push_back (r1);
                    break;
                  }
                case CHAR_ARR:
                  {
                    string tmp_c (val);
                    if (tmp_c > string (f.getCharFieldVal ()))
                        select_reply.push_back (r1);
                    break;
                  }
            }
        }
        else if (oper_s == ">=")
        {
            switch (f.getFieldType ())
            {
                case INT_VAL:
                  {
                    int tmp_val = atoi (val);
                    if (f.getIntFieldVal () >= tmp_val)
                        select_reply.push_back (r1);
                    break;
                  }
                case DOUBLE_VAL:
                  {
                    double tmp_d = atof (val);
                    if (f.getDoubleFieldVal () >= tmp_d)
                        select_reply.push_back (r1);
                    break;
                  }
                case CHAR_ARR:
                  {
                    string tmp_c (val);
                    if (tmp_c >= string (f.getCharFieldVal ()))
                        select_reply.push_back (r1);
                    break;
                  }
            }
        }
        else if (oper_s == "!=" || oper_s == "<>")
        {
            switch (f.getFieldType ())
            {
                case INT_VAL:
                  {
                    int tmp_val = atoi (val);
                    if (f.getIntFieldVal () != tmp_val)
                        select_reply.push_back (r1);
                    break;
                  }
                case DOUBLE_VAL:
                  {
                    double tmp_d = atof (val);
                    if (f.getDoubleFieldVal () != tmp_d)
                        select_reply.push_back (r1);
                    break;
                  }
                case CHAR_ARR:
                  {
                    string tmp_c (val);
                    if (tmp_c != string (f.getCharFieldVal ()))
                        select_reply.push_back (r1);
                    break;
                  }
            }
        }
        else if (oper == NULL)
        {
            select_reply.push_back (r1);
        }
        else
        {
            cout << "ERROR! Invalid Query.\n";
        }
        r1 = t.getNextRow ();
    }
    return select_reply;
}

void Database::printSelectResult (Database& db, char* table_name,
                                  vector<Row> res)
{
    Table t = db.getTableFromName (string(table_name));
    vector<string> col_name = t.getFieldNames ();
    for (int i = 0; i < col_name.size (); i++)
        cout << col_name[i] << "\t";
    cout << "\n\n";

    for (int i = 0; i < res.size (); i++)
        cout << res[i];
}

void Database::updateTable (Database& db, char* table_name, char* col_name1,
                            const char* val1, char* col_name2,
                            const char* oper, const char* val2)
{
    Table t = db.getTableFromName (string (table_name));
    Row r1 = t.getNextRow ();
    string oper_s;
    if (oper)
        oper_s = oper;
    while (r1.isGood ())
    {
        Field f, f1;
        if (col_name2)
            f = r1.getFieldByName (string (col_name2));
        if (oper_s == "<")
        {
            switch (f.getFieldType ())
            {
                case INT_VAL:
                  {
                    int tmp_val = atoi (val2);
                    if (f.getIntFieldVal () < tmp_val)
                    {
                        f1 = r1.getFieldByName (string (col_name1));
                        switch (f1.getFieldType ())
                        {
                            case INT_VAL:
                                f1.setIntFieldVal (atoi (val1));
                                break;
                            case DOUBLE_VAL:
                                f1.setDoubleFieldVal (atof (val1));
                                break;
                            case CHAR_ARR:
                                f1.setCharFieldVal (val1);
                                break;
                        }
                    }
                    else
                        f1 = r1.getFieldByName (string (col_name1));
                    break;
                  }
                case DOUBLE_VAL:
                  {
                    double tmp_d = atof (val2);
                    if (f.getDoubleFieldVal () < tmp_d)
                    {
                        f1 = r1.getFieldByName (string (col_name1));
                        switch (f1.getFieldType ())
                        {
                            case INT_VAL:
                                f1.setIntFieldVal (atoi (val1));
                                break;
                            case DOUBLE_VAL:
                                f1.setDoubleFieldVal (atof (val1));
                                break;
                            case CHAR_ARR:
                                f1.setCharFieldVal (val1);
                                break;
                        }
                    }
                    else
                        f1 = r1.getFieldByName (string (col_name1));
                    break;
                  }
                case CHAR_ARR:
                  {
                    string tmp_c (val2);
                    if (tmp_c < string (f.getCharFieldVal ()))
                    {
                        f1 = r1.getFieldByName (string (col_name1));
                        switch (f1.getFieldType ())
                        {
                            case INT_VAL:
                                f1.setIntFieldVal (atoi (val1));
                                break;
                            case DOUBLE_VAL:
                                f1.setDoubleFieldVal (atof (val1));
                                break;
                            case CHAR_ARR:
                                f1.setCharFieldVal (val1);
                                break;
                        }
                    }
                    else
                        f1 = r1.getFieldByName (string (col_name1));
                    break;
                  }
            }
        }
        else if (oper_s == "<=")
        {
            switch (f.getFieldType ())
            {
                case INT_VAL:
                  {
                    int tmp_val = atoi (val2);
                    if (f.getIntFieldVal () <= tmp_val)
                    {
                        f1 = r1.getFieldByName (string (col_name1));
                        switch (f1.getFieldType ())
                        {
                            case INT_VAL:
                                f1.setIntFieldVal (atoi (val1));
                                break;
                            case DOUBLE_VAL:
                                f1.setDoubleFieldVal (atof (val1));
                                break;
                            case CHAR_ARR:
                                f1.setCharFieldVal (val1);
                                break;
                        }
                    }
                    else
                        f1 = r1.getFieldByName (string (col_name1));
                    break;
                  }
                case DOUBLE_VAL:
                  {
                    double tmp_d = atof (val2);
                    if (f.getDoubleFieldVal () <= tmp_d)
                    {
                        f1 = r1.getFieldByName (string (col_name1));
                        switch (f1.getFieldType ())
                        {
                            case INT_VAL:
                                f1.setIntFieldVal (atoi (val1));
                                break;
                            case DOUBLE_VAL:
                                f1.setDoubleFieldVal (atof (val1));
                                break;
                            case CHAR_ARR:
                                f1.setCharFieldVal (val1);
                                break;
                        }
                    }
                    else
                        f1 = r1.getFieldByName (string (col_name1));
                    break;
                  }
                case CHAR_ARR:
                  {
                    string tmp_c (val2);
                    if (tmp_c <= string (f.getCharFieldVal ()))
                    {
                        f1 = r1.getFieldByName (string (col_name1));
                        switch (f1.getFieldType ())
                        {
                            case INT_VAL:
                                f1.setIntFieldVal (atoi (val1));
                                break;
                            case DOUBLE_VAL:
                                f1.setDoubleFieldVal (atof (val1));
                                break;
                            case CHAR_ARR:
                                f1.setCharFieldVal (val1);
                                break;
                        }
                    }
                    else
                        f1 = r1.getFieldByName (string (col_name1));
                    break;
                  }
            }
        }
        else if (oper_s == "==")
        {
            switch (f.getFieldType ())
            {
                case INT_VAL:
                  {
                    int tmp_val = atoi (val2);
                    if (f.getIntFieldVal () == tmp_val)
                    {
                        f1 = r1.getFieldByName (string (col_name1));
                        switch (f1.getFieldType ())
                        {
                            case INT_VAL:
                                f1.setIntFieldVal (atoi (val1));
                                break;
                            case DOUBLE_VAL:
                                f1.setDoubleFieldVal (atof (val1));
                                break;
                            case CHAR_ARR:
                                f1.setCharFieldVal (val1);
                                break;
                        }
                    }
                    else
                        f1 = r1.getFieldByName (string (col_name1));
                    break;
                  }
                case DOUBLE_VAL:
                  {
                    double tmp_d = atof (val2);
                    if (f.getDoubleFieldVal () == tmp_d)
                    {
                        f1 = r1.getFieldByName (string (col_name1));
                        switch (f1.getFieldType ())
                        {
                            case INT_VAL:
                                f1.setIntFieldVal (atoi (val1));
                                break;
                            case DOUBLE_VAL:
                                f1.setDoubleFieldVal (atof (val1));
                                break;
                            case CHAR_ARR:
                                f1.setCharFieldVal (val1);
                                break;
                        }
                    }
                    else
                        f1 = r1.getFieldByName (string (col_name1));
                    break;
                  }
                case CHAR_ARR:
                  {
                    string tmp_c (val2);
                    if (tmp_c == string (f.getCharFieldVal ()))
                    {
                        f1 = r1.getFieldByName (string (col_name1));
                        switch (f1.getFieldType ())
                        {
                            case INT_VAL:
                                f1.setIntFieldVal (atoi (val1));
                                break;
                            case DOUBLE_VAL:
                                f1.setDoubleFieldVal (atof (val1));
                                break;
                            case CHAR_ARR:
                                f1.setCharFieldVal (val1);
                                break;
                        }
                    }
                    else
                        f1 = r1.getFieldByName (string (col_name1));
                    break;
                  }
            }
        }
        else if (oper_s == ">")
        {
            switch (f.getFieldType ())
            {
                case INT_VAL:
                  {
                    int tmp_val = atoi (val2);
                    if (f.getIntFieldVal () > tmp_val)
                    {
                        f1 = r1.getFieldByName (string (col_name1));
                        switch (f1.getFieldType ())
                        {
                            case INT_VAL:
                                f1.setIntFieldVal (atoi (val1));
                                break;
                            case DOUBLE_VAL:
                                f1.setDoubleFieldVal (atof (val1));
                                break;
                            case CHAR_ARR:
                                f1.setCharFieldVal (val1);
                                break;
                        }
                    }
                    else
                        f1 = r1.getFieldByName (string (col_name1));
                    break;
                  }
                case DOUBLE_VAL:
                  {
                    double tmp_d = atof (val2);
                    if (f.getDoubleFieldVal () > tmp_d)
                    {
                        f1 = r1.getFieldByName (string (col_name1));
                        switch (f1.getFieldType ())
                        {
                            case INT_VAL:
                                f1.setIntFieldVal (atoi (val1));
                                break;
                            case DOUBLE_VAL:
                                f1.setDoubleFieldVal (atof (val1));
                                break;
                            case CHAR_ARR:
                                f1.setCharFieldVal (val1);
                                break;
                        }
                    }
                    else
                        f1 = r1.getFieldByName (string (col_name1));
                    break;
                  }
                case CHAR_ARR:
                  {
                    string tmp_c (val2);
                    if (tmp_c > string (f.getCharFieldVal ()))
                    {
                        f1 = r1.getFieldByName (string (col_name1));
                        switch (f1.getFieldType ())
                        {
                            case INT_VAL:
                                f1.setIntFieldVal (atoi (val1));
                                break;
                            case DOUBLE_VAL:
                                f1.setDoubleFieldVal (atof (val1));
                                break;
                            case CHAR_ARR:
                                f1.setCharFieldVal (val1);
                                break;
                        }
                    }
                    else
                        f1 = r1.getFieldByName (string (col_name1));
                    break;
                  }
            }
        }
        else if (oper_s == ">=")
        {
            switch (f.getFieldType ())
            {
                case INT_VAL:
                  {
                    int tmp_val = atoi (val2);
                    if (f.getIntFieldVal () >= tmp_val)
                    {
                        f1 = r1.getFieldByName (string (col_name1));
                        switch (f1.getFieldType ())
                        {
                            case INT_VAL:
                                f1.setIntFieldVal (atoi (val1));
                                break;
                            case DOUBLE_VAL:
                                f1.setDoubleFieldVal (atof (val1));
                                break;
                            case CHAR_ARR:
                                f1.setCharFieldVal (val1);
                                break;
                        }
                    }
                    else
                        f1 = r1.getFieldByName (string (col_name1));
                    break;
                  }
                case DOUBLE_VAL:
                  {
                    double tmp_d = atof (val2);
                    if (f.getDoubleFieldVal () >= tmp_d)
                    {
                        f1 = r1.getFieldByName (string (col_name1));
                        switch (f1.getFieldType ())
                        {
                            case INT_VAL:
                                f1.setIntFieldVal (atoi (val1));
                                break;
                            case DOUBLE_VAL:
                                f1.setDoubleFieldVal (atof (val1));
                                break;
                            case CHAR_ARR:
                                f1.setCharFieldVal (val1);
                                break;
                        }
                    }
                    else
                        f1 = r1.getFieldByName (string (col_name1));
                    break;
                  }
                case CHAR_ARR:
                  {
                    string tmp_c (val2);
                    if (tmp_c >= string (f.getCharFieldVal ()))
                    {
                        f1 = r1.getFieldByName (string (col_name1));
                        switch (f1.getFieldType ())
                        {
                            case INT_VAL:
                                f1.setIntFieldVal (atoi (val1));
                                break;
                            case DOUBLE_VAL:
                                f1.setDoubleFieldVal (atof (val1));
                                break;
                            case CHAR_ARR:
                                f1.setCharFieldVal (val1);
                                break;
                        }
                    }
                    else
                        f1 = r1.getFieldByName (string (col_name1));
                    break;
                  }
            }
        }
        else if (oper_s == "!=" || oper_s == "<>")
        {
            switch (f.getFieldType ())
            {
                case INT_VAL:
                  {
                    int tmp_val = atoi (val2);
                    if (f.getIntFieldVal () != tmp_val)
                    {
                        f1 = r1.getFieldByName (string (col_name1));
                        switch (f1.getFieldType ())
                        {
                            case INT_VAL:
                                f1.setIntFieldVal (atoi (val1));
                                break;
                            case DOUBLE_VAL:
                                f1.setDoubleFieldVal (atof (val1));
                                break;
                            case CHAR_ARR:
                                f1.setCharFieldVal (val1);
                                break;
                        }
                    }
                    else
                        f1 = r1.getFieldByName (string (col_name1));
                    break;
                  }
                case DOUBLE_VAL:
                  {
                    double tmp_d = atof (val2);
                    if (f.getDoubleFieldVal () != tmp_d)
                    {
                        f1 = r1.getFieldByName (string (col_name1));
                        switch (f1.getFieldType ())
                        {
                            case INT_VAL:
                                f1.setIntFieldVal (atoi (val1));
                                break;
                            case DOUBLE_VAL:
                                f1.setDoubleFieldVal (atof (val1));
                                break;
                            case CHAR_ARR:
                                f1.setCharFieldVal (val1);
                                break;
                        }
                    }
                    else
                        f1 = r1.getFieldByName (string (col_name1));
                    break;
                  }
                case CHAR_ARR:
                  {
                    string tmp_c (val2);
                    if (tmp_c != string (f.getCharFieldVal ()))
                    {
                        f1 = r1.getFieldByName (string (col_name1));
                        switch (f1.getFieldType ())
                        {
                            case INT_VAL:
                                f1.setIntFieldVal (atoi (val1));
                                break;
                            case DOUBLE_VAL:
                                f1.setDoubleFieldVal (atof (val1));
                                break;
                            case CHAR_ARR:
                                f1.setCharFieldVal (val1);
                                break;
                        }
                    }
                    else
                        f1 = r1.getFieldByName (string (col_name1));
                    break;
                  }
            }
        }
        else if (oper == NULL)
        {
            f1 = r1.getFieldByName (string (col_name1));
            switch (f1.getFieldType ())
            {
                case INT_VAL:
                    f1.setIntFieldVal (atoi (val1));
                    break;
                case DOUBLE_VAL:
                    f1.setDoubleFieldVal (atof (val1));
                    break;
                case CHAR_ARR:
                    f1.setCharFieldVal (val1);
                    break;
            }
        }
        else
        {
            cout << "ERROR! Invalid Query.\n";
            return;
        }
        r1.replaceField (f1, string (col_name1));
        ofstream op (t.getTableName ().c_str (), ios::out | ios::in |
                                                 ios::binary);
        int curr_pos = t.getCurrentPosition ();
        int seek_size = t.getSeekSize ();
        op.seekp ((curr_pos - seek_size));
        r1.write (op);
        op.close ();
        r1 = t.getNextRow ();
    }
}

void Database::deleteFromTable (Database& db, char* table_name,
                                char* col_name, const char* oper,
                                const char* val)
{
    Table t = db.getTableFromName (string (table_name));
    vector<bool> to_ch = t.getDeletedField ();
    if (col_name == NULL)
    {
        for (int i = 0; i < to_ch.size (); i++)
            to_ch[i] = true;
        t.setDeletedField (to_ch);
        db.modifyTable (t, string (table_name));
        return;
    }
    Row r1 = t.getNextRow ();
    string oper_s;
    if (oper)
        oper_s = oper;
    while (r1.isGood ())
    {
        int i_tmp = t.getCurrRecordNum ();
        Field f;
        if (col_name)
            f = r1.getFieldByName (string (col_name));
        if (oper_s == "<")
        {
            switch (f.getFieldType ())
            {
                case INT_VAL:
                  {
                    int tmp_val = atoi (val);
                    if (f.getIntFieldVal () < tmp_val)
                        to_ch[i_tmp] = true;
                    break;
                  }
                case DOUBLE_VAL:
                  {
                    double tmp_d = atof (val);
                    if (f.getDoubleFieldVal () < tmp_d)
                        to_ch[i_tmp] = true;
                    break;
                  }
                case CHAR_ARR:
                  {
                    string tmp_c (val);
                    if (tmp_c < string (f.getCharFieldVal ()))
                        to_ch[i_tmp] = true;
                    break;
                  }
            }
        }
        else if (oper_s == "<=")
        {
            switch (f.getFieldType ())
            {
                case INT_VAL:
                  {
                    int tmp_val = atoi (val);
                    if (f.getIntFieldVal () <= tmp_val)
                        to_ch[i_tmp] = true;
                    break;
                  }
                case DOUBLE_VAL:
                  {
                    double tmp_d = atof (val);
                    if (f.getDoubleFieldVal () <= tmp_d)
                        to_ch[i_tmp] = true;
                    break;
                  }
                case CHAR_ARR:
                  {
                    string tmp_c (val);
                    if (tmp_c <= string (f.getCharFieldVal ()))
                        to_ch[i_tmp] = true;
                    break;
                  }
            }
        }
        else if (oper_s == "==")
        {
            switch (f.getFieldType ())
            {
                case INT_VAL:
                  {
                    int tmp_val = atoi (val);
                    if (f.getIntFieldVal () == tmp_val)
                        to_ch[i_tmp] = true;
                    break;
                  }
                case DOUBLE_VAL:
                  {
                    double tmp_d = atof (val);
                    if (f.getDoubleFieldVal () == tmp_d)
                        to_ch[i_tmp] = true;
                    break;
                  }
                case CHAR_ARR:
                  {
                    string tmp_c (val);
                    if (tmp_c == string (f.getCharFieldVal ()))
                        to_ch[i_tmp] = true;
                    break;
                  }
            }
        }
        else if (oper_s == ">")
        {
            switch (f.getFieldType ())
            {
                case INT_VAL:
                  {
                    int tmp_val = atoi (val);
                    if (f.getIntFieldVal () > tmp_val)
                        to_ch[i_tmp] = true;
                    break;
                  }
                case DOUBLE_VAL:
                  {
                    double tmp_d = atof (val);
                    if (f.getDoubleFieldVal () > tmp_d)
                        to_ch[i_tmp] = true;
                    break;
                  }
                case CHAR_ARR:
                  {
                    string tmp_c (val);
                    if (tmp_c > string (f.getCharFieldVal ()))
                        to_ch[i_tmp] = true;
                    break;
                  }
            }
        }
        else if (oper_s == ">=")
        {
            switch (f.getFieldType ())
            {
                case INT_VAL:
                  {
                    int tmp_val = atoi (val);
                    if (f.getIntFieldVal () >= tmp_val)
                        to_ch[i_tmp] = true;
                    break;
                  }
                case DOUBLE_VAL:
                  {
                    double tmp_d = atof (val);
                    if (f.getDoubleFieldVal () >= tmp_d)
                        to_ch[i_tmp] = true;
                    break;
                  }
                case CHAR_ARR:
                  {
                    string tmp_c (val);
                    if (tmp_c >= string (f.getCharFieldVal ()))
                        to_ch[i_tmp] = true;
                    break;
                  }
            }
        }
        else if (oper_s == "!=" || oper_s == "<>")
        {
            switch (f.getFieldType ())
            {
                case INT_VAL:
                  {
                    int tmp_val = atoi (val);
                    if (f.getIntFieldVal () != tmp_val)
                        to_ch[i_tmp] = true;
                    break;
                  }
                case DOUBLE_VAL:
                  {
                    double tmp_d = atof (val);
                    if (f.getDoubleFieldVal () != tmp_d)
                        to_ch[i_tmp] = true;
                    break;
                  }
                case CHAR_ARR:
                  {
                    string tmp_c (val);
                    if (tmp_c != string (f.getCharFieldVal ()))
                        to_ch[i_tmp] = true;
                    break;
                  }
            }
        }
        else
        {
            cout << "ERROR! Invalid Query.\n";
        }
        r1 = t.getNextRow ();
    }
    t.setDeletedField (to_ch);
    db.modifyTable (t, string (table_name));
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

Field Row::getFieldByName (string f_name)
{
    for (int i = 0; i < field_names.size (); i++)
    {
        if (field_names[i] == f_name)
            return field_val[i];
    }
    Field tmp;
    return tmp;
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
            case INT_VAL:
                tmp = field_val[i].getIntFieldVal ();
                out_t.write (reinterpret_cast<const char *>(&tmp),
                             sizeof (tmp));
                break;
            case DOUBLE_VAL:
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

void Row::replaceField (Field f, string f_name)
{
    for (int i = 0; i < field_names.size (); i++)
    {
        if (field_names[i] == f_name)
        {
            field_val[i] = f;
            return;
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
            case INT_VAL:
                tmp = field_val[i].getIntFieldVal ();
                out_t << tmp << "\t";
                break;
            case DOUBLE_VAL:
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
    ft = INT_VAL;
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

void Field::setIntFieldVal (int field_val)
{
    field_int = field_val;
}

void Field::setDoubleFieldVal (double field_val)
{
    field_double = field_val;
}

void Field::setCharFieldVal (const char* field_val)
{
    strcpy (field_char, field_val);
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
    op << isDeleted.size () << endl;
    for (int i = 0; i < isDeleted.size (); i++)
        op << isDeleted[i] << endl;
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
            case INT_VAL:
              {
                in_t.read ((char*)&tmp, sizeof (tmp));
                Field fi (field_types[i], tmp);
                seek_size += sizeof (fi.getIntFieldVal ());
                break;
              }
            case DOUBLE_VAL:
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
    ifstream ip (name.c_str (), ios::in | ios::binary | ios::ate);
    //cout << "ip.tellg () = " << ip.tellg () << endl;
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

Table::~Table ()
{
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

vector<bool> Table::getDeletedField ()
{
    return isDeleted;
}

void Table::setDeletedField (vector<bool> del)
{
    isDeleted = del;
    writeTableToAttrFile ();
}

Row Table::getNextRow ()
{
    setSeekSize ();
    setFileSize ();
    int i_tmp = curr_pos / seek_size;
    while (isDeleted[i_tmp] && i_tmp < isDeleted.size ())
    {
        //cout << "i_tmp = " << i_tmp << endl;
        curr_pos += seek_size;
        //cout << curr_pos << endl;
        //cout << isDeleted[i_tmp] << endl;
        ++i_tmp;
        //cout << isDeleted[i_tmp] << endl;
    }
    if (curr_pos >= file_size)
    {
        //cout << "curr_pos = " << curr_pos << endl;
        curr_pos = 0;
        //cout << i_tmp << endl;
        //cout << "Am I here??\n";
        Row r;
        return r;
    }
    //cout << "How did I get here? " << i_tmp << endl;
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
            case INT_VAL:
              {
                in_t.read ((char*)&tmp, sizeof (tmp));
                Field fi (field_types[i], tmp);
                r.addField (fi, field_names[i]);
                break;
              }
            case DOUBLE_VAL:
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

int Table::getCurrentPosition ()
{
    return curr_pos;
}

int Table::getSeekSize ()
{
    return seek_size;
}

int Table::getCurrRecordNum ()
{
    return ((curr_pos - seek_size) / seek_size);
}

bool Table::isGood ()
{
    return (field_names.size () != 0);
}

void Table::write (string tb_name, Row r)
{
    if (isDeleted.size () == 0)
    {
        ofstream out_t (tb_name.c_str (), ios::out | ios::binary);
        r.write (out_t);
        isDeleted.push_back (false);
        out_t.close ();
        writeTableToAttrFile ();
        return;
    }
    /*for (int i = 0; i < isDeleted.size (); i++)
    {
        if (isDeleted[i])
        {
            ofstream out_t (tb_name.c_str (), ios::out | ios::app |
                                              ios::binary);
            out_t.seekp (i * seek_size);
            cout << "tellp = " << out_t.tellp () << endl;
            cout << "i = " << i << endl;
            r.write (out_t);
            cout << "Row = " << r << endl;
            isDeleted[i] = false;
            out_t.close ();
            writeTableToAttrFile ();
            //cout << "In here???????\n";
            return;
        }
    }*/
    ofstream out_t (tb_name.c_str (), ios::app | ios::binary);
    r.write (out_t);
    isDeleted.push_back (false);
    out_t.close ();
    setFileSize ();
    writeTableToAttrFile ();
    //cout << "Or Maybe Here??\n";
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
            case INT_VAL:
              {
                in_t.read ((char*)&tmp, sizeof (tmp));
                Field fi (field_types[i], tmp);
                r.addField (fi, field_names[i]);
                break;
              }
            case DOUBLE_VAL:
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
