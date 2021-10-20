/*
Lab 1 report <Student_names and liu_id>
*/

/* All non code should be within SQL-comments like this */


/*
Drop all user created tables that have been created when solving the lab
*/

DROP TABLE IF EXISTS custom_table CASCADE;


/* Have the source scripts in the file so it is easy to recreate!*/

SOURCE company_schema.sql;
SOURCE company_data.sql;

/*
Question 0: Print a message that says "hello world"
*/
/*
SELECT 'hello world!' AS 'message';
*/
/* Show the output for every question.
+--------------+
| message      |
+--------------+
| hello world! |
+--------------+
1 row in set (0.00 sec)
*/


/*
Question 1: List all employees, i.e. all tuples in the jbemployee relation
*/

/* select * from jbemployee; */

/*
+------+--------------------+--------+---------+-----------+-----------+
| id   | name               | salary | manager | birthyear | startyear |
+------+--------------------+--------+---------+-----------+-----------+
|   10 | Ross, Stanley      |  15908 |     199 |      1927 |      1945 |
|   11 | Ross, Stuart       |  12067 |    NULL |      1931 |      1932 |
|   13 | Edwards, Peter     |   9000 |     199 |      1928 |      1958 |
|   26 | Thompson, Bob      |  13000 |     199 |      1930 |      1970 |
|   32 | Smythe, Carol      |   9050 |     199 |      1929 |      1967 |
|   33 | Hayes, Evelyn      |  10100 |     199 |      1931 |      1963 |
|   35 | Evans, Michael     |   5000 |      32 |      1952 |      1974 |
|   37 | Raveen, Lemont     |  11985 |      26 |      1950 |      1974 |
|   55 | James, Mary        |  12000 |     199 |      1920 |      1969 |
|   98 | Williams, Judy     |   9000 |     199 |      1935 |      1969 |
|  129 | Thomas, Tom        |  10000 |     199 |      1941 |      1962 |
|  157 | Jones, Tim         |  12000 |     199 |      1940 |      1960 |
|  199 | Bullock, J.D.      |  27000 |    NULL |      1920 |      1920 |
|  215 | Collins, Joanne    |   7000 |      10 |      1950 |      1971 |
|  430 | Brunet, Paul C.    |  17674 |     129 |      1938 |      1959 |
|  843 | Schmidt, Herman    |  11204 |      26 |      1936 |      1956 |
|  994 | Iwano, Masahiro    |  15641 |     129 |      1944 |      1970 |
| 1110 | Smith, Paul        |   6000 |      33 |      1952 |      1973 |
| 1330 | Onstad, Richard    |   8779 |      13 |      1952 |      1971 |
| 1523 | Zugnoni, Arthur A. |  19868 |     129 |      1928 |      1949 |
| 1639 | Choy, Wanda        |  11160 |      55 |      1947 |      1970 |
| 2398 | Wallace, Maggie J. |   7880 |      26 |      1940 |      1959 |
| 4901 | Bailey, Chas M.    |   8377 |      32 |      1956 |      1975 |
| 5119 | Bono, Sonny        |  13621 |      55 |      1939 |      1963 |
| 5219 | Schwarz, Jason B.  |  13374 |      33 |      1944 |      1959 |
+------+--------------------+--------+---------+-----------+-----------+
25 rows in set (0.00 sec)
*/

/*
Question 2: List the name of all departments in alphabetical order. Note: by ?name? we mean
the name attribute for all tuples in the jbdept relation.
*/

/*
select name from jbdept order by name asc;
*/

/*
+------------------+
| Bargain          |
| Book             |
| Candy            |
| Children's       |
| Children's       |
| Furniture        |
| Giftwrap         |
| Jewelry          |
| Junior Miss      |
| Junior's         |
| Linens           |
| Major Appliances |
| Men's            |
| Sportswear       |
| Stationary       |
| Toys             |
| Women's          |
| Women's          |
| Women's          |
+------------------+
19 rows in set (0.00 sec)
*/

/*
Question 3: What parts are not in store, i.e. qoh = 0? (qoh = Quantity On Hand)
*/

/*
select * from jbparts where qoh = 0;
*/

/*
+----+-------------------+-------+--------+------+
| id | name              | color | weight | qoh  |
+----+-------------------+-------+--------+------+
| 11 | card reader       | gray  |    327 |    0 |
| 12 | card punch        | gray  |    427 |    0 |
| 13 | paper tape reader | black |    107 |    0 |
| 14 | paper tape punch  | black |    147 |    0 |
+----+-------------------+-------+--------+------+
4 rows in set (0.00 sec)

*/


/*
Question 4: Which employees have a salary between 9000 (included) and 10000 (included)?
*/

/*
select * from jbemployee where salary >= 9000 and salary <= 10000;
*/

/*
+-----+----------------+--------+---------+-----------+-----------+
| id  | name           | salary | manager | birthyear | startyear |
+-----+----------------+--------+---------+-----------+-----------+
|  13 | Edwards, Peter |   9000 |     199 |      1928 |      1958 |
|  32 | Smythe, Carol  |   9050 |     199 |      1929 |      1967 |
|  98 | Williams, Judy |   9000 |     199 |      1935 |      1969 |
| 129 | Thomas, Tom    |  10000 |     199 |      1941 |      1962 |
+-----+----------------+--------+---------+-----------+-----------+
4 rows in set (0.01 sec)

*/


/*
Question 5: What was the age of each employee when they started working (startyear)?
*/
/*
select name ,startyear-birthyear as startage from jbemployee;
*/
/*
+--------------------+----------+
| name               | startage |
+--------------------+----------+
| Ross, Stanley      |       18 |
| Ross, Stuart       |        1 |
| Edwards, Peter     |       30 |
| Thompson, Bob      |       40 |
| Smythe, Carol      |       38 |
| Hayes, Evelyn      |       32 |
| Evans, Michael     |       22 |
| Raveen, Lemont     |       24 |
| James, Mary        |       49 |
| Williams, Judy     |       34 |
| Thomas, Tom        |       21 |
| Jones, Tim         |       20 |
| Bullock, J.D.      |        0 |
| Collins, Joanne    |       21 |
| Brunet, Paul C.    |       21 |
| Schmidt, Herman    |       20 |
| Iwano, Masahiro    |       26 |
| Smith, Paul        |       21 |
| Onstad, Richard    |       19 |
| Zugnoni, Arthur A. |       21 |
| Choy, Wanda        |       23 |
| Wallace, Maggie J. |       19 |
| Bailey, Chas M.    |       19 |
| Bono, Sonny        |       24 |
| Schwarz, Jason B.  |       15 |
+--------------------+----------+
25 rows in set (0.01 sec)

*/


/*
Question 6: Which employees have a last name ending with "son"?
*/
/*
select * from jbemployee where name like '%son,%'
*/

/*
+----+---------------+--------+---------+-----------+-----------+
| id | name          | salary | manager | birthyear | startyear |
+----+---------------+--------+---------+-----------+-----------+
| 26 | Thompson, Bob |  13000 |     199 |      1930 |      1970 |
+----+---------------+--------+---------+-----------+-----------+
1 row in set (0.00 sec)

*/



/*
Question 7: Which items (note items, not parts) have been delivered by a supplier called
Fisher-Price? Formulate this query using a subquery in the where-clause.
*/

/*
select * from jbitem where supplier = (select id from jbsupplier where name like 'Fisher-Price');
*/

/*
+-----+-----------------+------+-------+------+----------+
| id  | name            | dept | price | qoh  | supplier |
+-----+-----------------+------+-------+------+----------+
|  43 | Maze            |   49 |   325 |  200 |       89 |
| 107 | The 'Feel' Book |   35 |   225 |  225 |       89 |
| 119 | Squeeze Ball    |   49 |   250 |  400 |       89 |
+-----+-----------------+------+-------+------+----------+
3 rows in set (0.00 sec)

*/

/*
Question 8: Formulate the same query as above, but without a subquery.
*/

/*
select * from jbitem inner join jbsupplier as j on supplier = j.id where j.name like 'Fisher-Price';
*/

/*
+-----+-----------------+------+-------+------+----------+----+--------------+------+
| id  | name            | dept | price | qoh  | supplier | id | name         | city |
+-----+-----------------+------+-------+------+----------+----+--------------+------+
|  43 | Maze            |   49 |   325 |  200 |       89 | 89 | Fisher-Price |   21 |
| 107 | The 'Feel' Book |   35 |   225 |  225 |       89 | 89 | Fisher-Price |   21 |
| 119 | Squeeze Ball    |   49 |   250 |  400 |       89 | 89 | Fisher-Price |   21 |
+-----+-----------------+------+-------+------+----------+----+--------------+------+
3 rows in set (0.00 sec)


*/


/*
Question 9: Show all cities that have suppliers located in them. Formulate this query using a
subquery in the where-clause.

*/
/*
select * from jbcity where id in (select city from jbsupplier);
*/

/*
+-----+----------------+-------+
| id  | name           | state |
+-----+----------------+-------+
|  10 | Amherst        | Mass  |
|  21 | Boston         | Mass  |
| 100 | New York       | NY    |
| 106 | White Plains   | Neb   |
| 118 | Hickville      | Okla  |
| 303 | Atlanta        | Ga    |
| 537 | Madison        | Wisc  |
| 609 | Paxton         | Ill   |
| 752 | Dallas         | Tex   |
| 802 | Denver         | Colo  |
| 841 | Salt Lake City | Utah  |
| 900 | Los Angeles    | Calif |
| 921 | San Diego      | Calif |
| 941 | San Francisco  | Calif |
| 981 | Seattle        | Wash  |
+-----+----------------+-------+
15 rows in set (0.00 sec)
*/


/*
Question 10: What is the name and color of the parts that are heavier than a card reader?
Formulate this query using a subquery in the where-clause.
(The SQL query must not contain the weightas a constant.)
*/

/*
select name, color from jbparts where weight > (select weight from jbparts where name like 'card reader');
*/

/*
+--------------+--------+
| name         | color  |
+--------------+--------+
| disk drive   | black  |
| tape drive   | black  |
| line printer | yellow |
| card punch   | gray   |
+--------------+--------+
4 rows in set (0.01 sec)


*/


/*
Question 11: Formulate the same query as above, but without a subquery.
(The query must not contain the weight as a constant).
*/

/*
select a.name, a.color from jbparts as a inner join jbparts as b on 'card reader' = b.name where b.weight-a.weight < 0;
*/
/*
+--------------+--------+
| name         | color  |
+--------------+--------+
| disk drive   | black  |
| tape drive   | black  |
| line printer | yellow |
| card punch   | gray   |
+--------------+--------+
4 rows in set (0.01 sec)

*/


/*
Question 12: What is the average weight of black parts?
*/
/*
select avg(weight) from jbparts where color like 'black'
*/
/*
+-------------+
| avg(weight) |
+-------------+
|    347.2500 |
+-------------+
1 row in set (0.00 sec)


*/

/*
Question 13: What is the total weight of all parts that each supplier in Massachusetts ("Mass")
has delivered? Retrieve the name and the total weight for each of these suppliers.
Do not forget to take the quantity of delivered parts into account. Note that one
row should be returned for each supplier
*/

/*
select supply.name, sum(quan*weight) as totalweight  from
(select id, name, part, quan from jbsupply inner join
(select id, name from jbsupplier where city in
(select id from jbcity where state like 'Mass')) as a on supplier = id) as supply
inner join jbparts on part = jbparts.id group by supply.id;
*/

/*
+--------------+-------------+
| name         | totalweight |
+--------------+-------------+
| Fisher-Price |     1135000 |
| DEC          |        3120 |
+--------------+-------------+
2 rows in set (0.00 sec)

*/

/*
Question 14: Create a new relation (a table), with the same attributes as the table items using
the CREATE TABLE syntax where you define every attribute explicitly (i.e. not
as a copy of another table). Then fill the table with all items that cost less than the
average price for items. Remember to define primary and foreign keys in your
table!
*/
/*
#select * from jbitem;

drop table newitem;
create table newitem(
  id integer,
  name varchar(16),
  dept integer,
  price integer,
  qoh integer,
  supplier integer,

  constraint pk_newitem primary key (id),

  constraint fk_dept foreign key (dept) references jbdept(id),

  constraint fk_supplier foreign key (supplier) references jbsupplier(id)
);

insert into newitem (select * from jbitem where price < (select avg(price) from jbitem));

select * from newitem;
*/

/*
+-----+-----------------+------+-------+------+----------+
| id  | name            | dept | price | qoh  | supplier |
+-----+-----------------+------+-------+------+----------+
|  11 | Wash Cloth      |    1 |    75 |  575 |      213 |
|  19 | Bellbottoms     |   43 |   450 |  600 |       33 |
|  21 | ABC Blocks      |    1 |   198 |  405 |      125 |
|  23 | 1 lb Box        |   10 |   215 |  100 |       42 |
|  25 | 2 lb Box, Mix   |   10 |   450 |   75 |       42 |
|  26 | Earrings        |   14 |  1000 |   20 |      199 |
|  43 | Maze            |   49 |   325 |  200 |       89 |
| 106 | Clock Book      |   49 |   198 |  150 |      125 |
| 107 | The 'Feel' Book |   35 |   225 |  225 |       89 |
| 118 | Towels, Bath    |   26 |   250 | 1000 |      213 |
| 119 | Squeeze Ball    |   49 |   250 |  400 |       89 |
| 120 | Twin Sheet      |   26 |   800 |  750 |      213 |
| 165 | Jean            |   65 |   825 |  500 |       33 |
| 258 | Shirt           |   58 |   650 | 1200 |       33 |
+-----+-----------------+------+-------+------+----------+
14 rows in set (0.00 sec)
*/

/*
Question 15: Create a view that contains the items that cost less than the average price for
items.
*/

/*
drop view item_view;
create view item_view as select * from jbitem where price < (select avg(price) from jbitem);

select * from item_view;
*/
/*
+-----+-----------------+------+-------+------+----------+
| id  | name            | dept | price | qoh  | supplier |
+-----+-----------------+------+-------+------+----------+
|  11 | Wash Cloth      |    1 |    75 |  575 |      213 |
|  19 | Bellbottoms     |   43 |   450 |  600 |       33 |
|  21 | ABC Blocks      |    1 |   198 |  405 |      125 |
|  23 | 1 lb Box        |   10 |   215 |  100 |       42 |
|  25 | 2 lb Box, Mix   |   10 |   450 |   75 |       42 |
|  26 | Earrings        |   14 |  1000 |   20 |      199 |
|  43 | Maze            |   49 |   325 |  200 |       89 |
| 106 | Clock Book      |   49 |   198 |  150 |      125 |
| 107 | The 'Feel' Book |   35 |   225 |  225 |       89 |
| 118 | Towels, Bath    |   26 |   250 | 1000 |      213 |
| 119 | Squeeze Ball    |   49 |   250 |  400 |       89 |
| 120 | Twin Sheet      |   26 |   800 |  750 |      213 |
| 165 | Jean            |   65 |   825 |  500 |       33 |
| 258 | Shirt           |   58 |   650 | 1200 |       33 |
+-----+-----------------+------+-------+------+----------+
14 rows in set (0.00 sec)

*/

/*
Question 16: What is the difference between a table and a view? One is static and the other is
dynamic. Which is which and what do we mean by static respectively dynamic?
*/

/*
The difference between a table and a view is that the a view is a
one to many pointer, pointing at different tables.
Compared to a table, a view is not part of the schema, it just points to diffrents tables.
A view may also be used to add security to data by selective display of the data stored in the table.

A table is static while a view is dynamic, this is because when you ask for data
from the view it then looks up that data in the tables defined in the view.
So changes in the underlying tables will be reflected in the view.
*/

/*
Question 17: Create a view that calculates the total cost of each debit,
by considering price and quantity of each bought item.
(To be used for charging customer accounts).
The view should contain the sale identifier (debit) and total cost. Use only the implicit join notation,
i.e. only use a where clause but not the keywords inner join, right join or left join,:
*/

/*
drop view debit_view;
create view debit_view as
select debit, sum(price*quantity) as total_cost from jbsale, jbitem where id = item
group by debit;

select * from debit_view;
*/

/*
+--------+------------+
| debit  | total_cost |
+--------+------------+
| 100581 |       2050 |
| 100582 |       1000 |
| 100586 |      13446 |
| 100592 |        650 |
| 100593 |        430 |
| 100594 |       3295 |
+--------+------------+
6 rows in set (0.00 sec)


*/

/*
Question 18:
Do the same as in (17), using only the explicit join notation,
i.e. using only left, right or inner joins but no join condition in a where clause.
Motivate why you use the join you do (left, right or inner),
and why this is the correct one (unlike the others).
*/

/*
drop view debit_view;
create view debit_view as
select debit, sum(price*quantity) as total_cost from
jbsale left outer join jbitem on item = id group by debit;

select * from debit_view;
*/

/*
We use left outer join due to that we want to guarantee that every sale will be in the created view.
We could also have used an inner join but that would remove sales of an unknown item which
we feel would be wrong in this case.

*/

/*
+--------+------------+
| debit  | total_cost |
+--------+------------+
| 100581 |       2050 |
| 100582 |       1000 |
| 100586 |      13446 |
| 100592 |        650 |
| 100593 |        430 |
| 100594 |       3295 |
+--------+------------+
6 rows in set (0.00 sec)

*/



/*
Question 19:
Oh no! An earthquake!
a) Remove all suppliers in Los Angeles from the table jbsupplier.
This will not work right away (you will receive error code 23000)
which you will have to solve by deleting some other related tuples.
However, do not delete more tuples from other tables than necessary and do not change the structure of the tables,
i.e. do not remove foreign keys.
Also, remember that you are only allowed to use “Los Angeles” as a constant in your queries, not “199” or “900”.

 b)Explain what you did and why
*/

/*
drop view city_id;
create view city_id as
  (select id from jbcity where name like "Los Angeles");

drop view supplier_id;
create view supplier_id as
  (select id from jbsupplier where city = (select * from city_id));

delete from jbsupply where supplier = (select * from supplier_id);
delete from jbsale where item in (select id from jbitem where supplier = (select * from supplier_id));
delete from jbitem where supplier = (select * from supplier_id);
delete from jbsupplier where city = (select * from city_id);

select * from jbsupplier;
*/

/*
+-----+--------------+------+
| id  | name         | city |
+-----+--------------+------+
|   5 | Amdahl       |  921 |
|  15 | White Stag   |  106 |
|  20 | Wormley      |  118 |
|  33 | Levi-Strauss |  941 |
|  42 | Whitman's    |  802 |
|  62 | Data General |  303 |
|  67 | Edger        |  841 |
|  89 | Fisher-Price |   21 |
| 122 | White Paper  |  981 |
| 125 | Playskool    |  752 |
| 213 | Cannon       |  303 |
| 241 | IBM          |  100 |
| 440 | Spooley      |  609 |
| 475 | DEC          |   10 |
| 999 | A E Neumann  |  537 |
+-----+--------------+------+
15 rows in set (0.00 sec)
*/


/*

Because supplier has a foreign key with supply we need to remove rows from supply containing that supplier.
Supplier also contains a foreign key with item but as item contains a foreign key with
sale we can't remove an item before we remove from sale. This feels wrong because
sale should remain even if an item is no longer supplied, as a sale has still happened.
So we first remove from sale and then remove from item. And after this we can now remove from supplier as
there are no foreign keys that reference this supplier anymore.

*/




/*
Question 20:
An employee has tried to find out which suppliers that have delivered items that have been sold.
He has created a view and a query that shows the number of items sold from a supplier.

+--------------+------+
| supplier     | sum  |
+--------------+------+
| Cannon       |    6 |
| Levi-Strauss |    1 |
| Playskool    |    2 |
| White Stag   |    4 |
| Whitman's    |    2 |
+--------------+------+


The employee would also like include the suppliers which has delivered some items,
although for whom no items have been sold so far. In other words he wants to list all suppliers,
which has supplied any item, as well as the number of these items that have been sold.
Help him!

Drop and redefine jbsale_supplyto consider suppliers that have delivered items
that have never been sold as well.
Hint:The above definition of jbsale_supplyuses an (implicit) inner join that
removes suppliers that have not had any of their delivered item
*/

/*
drop view supplied_suppliers;
create view supplied_suppliers as
select id, name from jbsupplier where id in (select supplier from jbitem);

drop view items_supplied;
create view items_supplied as
select
  jbitem.id as id,
  jbitem.name as name,
  supplier,
  supplied_suppliers.name as supplier_name
from
jbitem right outer join supplied_suppliers
on supplied_suppliers.id = supplier;

drop view jbsale_supply;
create view jbsale_supply(supplier, item, quantity) as
select supplier_name, name, quantity from
jbsale right outer join items_supplied
on items_supplied.id = jbsale.item;

SELECT supplier, sum(quantity) AS sum FROM jbsale_supply
GROUP BY supplier;
*/

/*
+--------------+------+
| supplier     | sum  |
+--------------+------+
| Cannon       |    6 |
| Fisher-Price | NULL |
| Koret        |    1 |
| Levi-Strauss |    1 |
| Playskool    |    2 |
| White Stag   |    4 |
| Whitman's    |    2 |
+--------------+------+
7 rows in set (0.00 sec)

*/
