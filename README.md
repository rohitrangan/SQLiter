SQLiter
=======

A Basic RDBMS built in C++.

Basic Queries Accepted
----------------------

1.  create database db_name;
2.  USE db_name;
3.  create table tbl_name (var_name1 char, var_name2 integer,var_name3 double,......, .....,......);
4.  create table tbl_name (var_name var_type,..................);
5.  insert into tbl_name values (AAA, 12, 8.1, ...........);
6.  insert into tbl_name values (any_value,...............);
7.  select * from tbl_name;
8.  update tbl_name set c3 = 15 where c4 == 9.1;
9.  update tbl_name set col_name1 equal some_value1 where col_name2 operator some_value2
10. delete from tb1 where c3 == 12;
11. select * from tb1 where col_name1 operator some_value1
12. show databases;
13. show tables;
14. drop table tbl_name;
15. drop database db_name;

Requirements
------------

1. gcc 4.8 / clang-425.0.28 or greater.
2. flex 2.5 or greater.
3. bison 2.3 or greater.
4. GNU make 3.81 or greater.

Compiling and Executing the Code
--------------------------------

1. Execute 'make' to compile the code. This will create an executable called
   'sqliter' in the current directory.
2. Execute './sqliter' to run the code.

Note
----

1. Our database commands are case insensitive but our names are not case sensitive.
   For example Insert/INSERT/insert/or any other permutation of small and capital
   letter works in our case;

2. Sometimes the spaces are not mandatory. For example in where clause you can either
   use spaces after a left paranthesis or not. we have taken care of that situation
   and accepts the queries accordingly.

3. In case of faulty inputs syntax errors are shown.
