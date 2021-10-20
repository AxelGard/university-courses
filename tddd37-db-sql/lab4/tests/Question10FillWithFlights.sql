/******************************************************************************************
 Question 10, concurrency
 This is the first of two scripts that tests that the BryanAir database can handle concurrency.
 This script simply fills the database with a relevant flight used by the other script. Note 
 that this script only should be run in one of the terminals.
**********************************************************************************************/
SELECT "Testing answer for 10" as "Message";
SELECT "Filling database with flights, should only be run in one of the terminals" as "Message";
/*Fill the database with data */
CALL addYear(2010, 2.3);
CALL addDay(2010,"Monday",1);
CALL addDestination("MIT","Minas Tirith","Mordor");
CALL addDestination("HOB","Hobbiton","The Shire");
CALL addRoute("MIT","HOB",2010,2000);
CALL addFlight("MIT","HOB", 2010, "Monday", "09:00:00");

