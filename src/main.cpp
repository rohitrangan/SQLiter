/*
    SQLiter - main.cpp
    Author:- Rohit Rangan

    This class contains the main function.
*/

#include "../include/db.h"
#include "../include/wd.h"
#include "../include/fileio.h"

#include <string>
#include <fstream>
#include <iostream>

string MASTER_WD;

using namespace std;

int main ()
{
    MASTER_WD = fileio::getwd ();
    cout << MASTER_WD << endl;
    string db_name = "new_db";
    Database db (db_name);
    db.createDatabase ();
    Database::useDatabase (db);
    string table = "First";
    vector<string> fields;
    fields.push_back ("Integer");
    fields.push_back ("Double");
    fields.push_back ("String");
    vector<FieldType> ftype;
    ftype.push_back (INTEGER);
    ftype.push_back (DECIMAL);
    ftype.push_back (CHAR_ARR);

    Table t (table, db.getDatabaseName (), fields, ftype);
    db.addTable (t);
    Field fi (INTEGER, 10);
    Field fd (DECIMAL, 2.9);
    Field fc (CHAR_ARR, "FINALLY!! YAYAYAYAYAYAYAYAYAYAYAYA");
    Row r;
    r.addField (fi, fields[0]);
    r.addField (fd, fields[1]);
    r.addField (fc, fields[2]);
    ofstream op (t.getTableName ().c_str (), ios::app | ios::binary);
    r.write (op);
    op.close ();
    Row r1;
    r1 = t.getNextRow ();
    while (r1.isGood ())
    {
        cout << r1 << endl;
        r1 = t.getNextRow ();
    }
    Table::deleteTable (table);
    /*string table = "tmp";
    string db = "tmp_db";
    vector<string> fields;
    fields.push_back ("Integer");
    fields.push_back ("Double");
    fields.push_back ("String");
    vector<FieldType> ftype;
    ftype.push_back (INTEGER);
    ftype.push_back (DECIMAL);
    ftype.push_back (CHAR_ARR);

    Table t (table, db, fields, ftype);
    Field fi (INTEGER, 10);
    Field fd (DECIMAL, 2.9);
    Field fc (CHAR_ARR, "Come on");
    Row r;
    r.addField (fi, fields[0]);
    r.addField (fd, fields[1]);
    r.addField (fc, fields[2]);
    ofstream op (table.c_str (), ios::app | ios::binary);
    r.write (op);
    op.close ();
    cout << "Value of Row r:- \n" << r << endl;
    ifstream ip (table.c_str (), ios::in | ios::binary);
    int seek_size = sizeof (int) + sizeof (double) + sizeof (fc.getCharFieldVal ());
    cout << "Seek size = " << seek_size << endl;
    //ip.seekg ((3*seek_size), ios::beg);
    //Row r1 = Table::read (ip, t);
    //cout << "Value of Row r1:- \n" << r1 << endl;
    //ip.close ();
    //ifstream ip1 (table.c_str (), ios::in | ios::binary);
    ip.seekg ((3*seek_size));
    ip.seekg (0);
    Row r1 = Table::read (ip, t);
    cout << "Value of Row r1:- \n" << r1 << endl;
    ip.close ();
    ip.open (table.c_str (), ios::in | ios::binary);
    ip.seekg (seek_size);
    r1 = Table::read (ip, t);
    cout << "Value of Row r1:- \n" << r1 << endl;
    ip.close ();*/
    return 0;
}
