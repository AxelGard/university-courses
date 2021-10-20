/*************************************************************************************
 Question 3, Flight addition interfaces test script.
 This is a test script that tests that the interface of the BryanAir back-end works
 correctly. It simply tests that the interfaces exists for filling the database with
 flightinformation, without acutally checking that they do what they are supposed to. ´
 This is done later in the question 7 testscript.
*************************************************************************************/
SELECT "Testing questions for flight addition procedures, i.e. question 3" as "Message";
SELECT "Expected output for all queries are 'Query OK, 1 row affected (0.00 sec)' (where the time might differ)" as "Message";

/*Fill the database with data */
SELECT "Trying to add 2 years" AS "Message";
CALL addYear(2010, 2.3);
CALL addYear(2011, 2.5);
SELECT "Trying to add 4 days" AS "Message";
CALL addDay(2010,"Monday",1);
CALL addDay(2010,"Tuesday",1.5);
CALL addDay(2011,"Saturday",2);
CALL addDay(2011,"Sunday",2.5);
SELECT "Trying to add 2 destinations" AS "Message";
CALL addDestination("MIT","Minas Tirith","Mordor");
CALL addDestination("HOB","Hobbiton","The Shire");
SELECT "Trying to add 4 routes" AS "Message";
CALL addRoute("MIT","HOB",2010,2000);
CALL addRoute("HOB","MIT",2010,1600);
CALL addRoute("MIT","HOB",2011,2100);
CALL addRoute("HOB","MIT",2011,1500);
SELECT "Trying to add 4 weeklyschedule flights" AS "Message";
CALL addFlight("MIT","HOB", 2010, "Monday", "09:00:00");
CALL addFlight("HOB","MIT", 2010, "Tuesday", "10:00:00");
CALL addFlight("MIT","HOB", 2011, "Sunday", "11:00:00");
CALL addFlight("HOB","MIT", 2011, "Sunday", "12:00:00");

SELECT "You are now supposed to have 208 flights in your database. If so, and with reasonable data, it is probably correct and this is further tested for question 7" as "Message";