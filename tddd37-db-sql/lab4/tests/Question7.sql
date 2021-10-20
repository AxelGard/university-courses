/******************************************************************************************
 Question 7, Correct representation in the view.
 This is a test script that tests that the interface of the BryanAir back-end works
 correctly. More specifically it tests that flights and reservations are added correctly and
 that the number of seats and price is calculated correctly. This is done by checking against a 
 previous (correct) response to the query from an external database. 
**********************************************************************************************/

SELECT "Checking that bookings and flights are added correctly by checking the view" as "Message";
/*Fill the database with flights */
SELECT "Step1, fill the database with flights" AS "Message";
CALL addYear(2010, 2.3);
CALL addYear(2011, 2.5);
CALL addDay(2010,"Monday",1);
CALL addDay(2010,"Tuesday",1.5);
CALL addDay(2011,"Saturday",2);
CALL addDay(2011,"Sunday",2.5);
CALL addDestination("MIT","Minas Tirith","Mordor");
CALL addDestination("HOB","Hobbiton","The Shire");
CALL addRoute("MIT","HOB",2010,2000);
CALL addRoute("HOB","MIT",2010,1600);
CALL addRoute("MIT","HOB",2011,2100);
CALL addRoute("HOB","MIT",2011,1500);
CALL addFlight("MIT","HOB", 2010, "Monday", "09:00:00");
CALL addFlight("HOB","MIT", 2010, "Tuesday", "10:00:00");
CALL addFlight("MIT","HOB", 2011, "Sunday", "11:00:00");
CALL addFlight("HOB","MIT", 2011, "Sunday", "12:00:00");

SELECT "Step2, add a bunch of bookings to the flights" AS "Message";
CALL addReservation("MIT","HOB",2010,1,"Monday","09:00:00",3,@a); 
CALL addPassenger(@a,00000001,"Frodo Baggins"); 
CALL addContact(@a,00000001,"frodo@magic.mail",080667989); 
CALL addPassenger(@a,00000002,"Sam Gamgee"); 
CALL addPassenger(@a,00000003,"Merry Pippins");
CALL addPayment (@a, "Gandalf", 6767); 
CALL addReservation("MIT","HOB",2010,1,"Monday","09:00:00",3,@b); 
CALL addPassenger(@b,00000011,"Nazgul1"); 
CALL addContact(@b,00000011,"Nazgul@darkness.mail",666); 
CALL addPassenger(@b,00000012,"Nazgul2"); 
CALL addPassenger(@b,00000013,"Nazgul3");
CALL addPayment (@b, "Saruman", 6868); 

SELECT "Step3, check that the results are correct. If so the next query should return the empty set. If any line is returned then this is either missing, incorrect or additional to what the database should contain" AS "Message";
SELECT departure_city_name, destination_city_name, departure_time, departure_day,departure_week, departure_year, nr_of_free_seats, current_price_per_seat
FROM (
SELECT departure_city_name, destination_city_name, departure_time, departure_day,departure_week, departure_year, nr_of_free_seats, current_price_per_seat FROM allFlights
UNION ALL
SELECT departure_city_name, destination_city_name, departure_time, departure_day,departure_week, departure_year, nr_of_free_seats, current_price_per_seat FROM TDDD37.Question7CorrectResult
) res
GROUP BY departure_city_name, destination_city_name, departure_time, departure_day,departure_week, departure_year, nr_of_free_seats, current_price_per_seat
HAVING count(*) = 1;
