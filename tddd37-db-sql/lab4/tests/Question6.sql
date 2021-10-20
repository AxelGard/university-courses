/******************************************************************************************
 Question 6, Reservation interfaces test script.
 This is a test script that tests that the interface of the BryanAir back-end works
 correctly. More specifically it tests that the interfaces handling the reservation 
 procedures works correctly and give the correct output. 
 The test-cases are:
	Test1:  Adding a correct reservation.
	Test2:  Adding a reservation with incorrect flight details. Should print the
		message "There exist no flight for the given route, date and time".
	Test3:  Adding a reservation when there are not enough seats. Should print the
		message "There are not enough seats available on the chosen flight".
	Test4:  Adding a passenger in a correct manner.
	Test5:  Adding a passenger with incorrect reservation number. Should print the
 		message: "The given reservation number does not exist".
	Test6:  Adding a contact in a correct manner.
	Test7:  Adding a contact with incorrect reservation number. Should print the 
		message "The given reservation number does not exist".
	Test8:  Adding a contact that is not a passenger on the reservation. Should print
		the message "The person is not a passenger of the reservation".
	Test9:  Making a payment in a correct manner. 
	Test10: Making a payment to a reservation with incorrect reservation number. 
		Should print the message "The given reservation number does not exist". 
	Test11: Making a payment to a reservation with no contact. Should print the message 
		"The reservation has no contact yet".
	Test12: Adding a passenger to an already payed reservation. Should print the message 
		"The booking has already been payed and no futher passengers can be added".
**********************************************************************************************/
SELECT "Testing answer for 6, handling reservations and bookings" as "Message";
SELECT "Filling database with flights" as "Message";
/*Fill the database with data */
CALL addYear(2010, 2.3);
CALL addDay(2010,"Monday",1);
CALL addDestination("MIT","Minas Tirith","Mordor");
CALL addDestination("HOB","Hobbiton","The Shire");
CALL addRoute("MIT","HOB",2010,2000);
CALL addFlight("MIT","HOB", 2010, "Monday", "09:00:00");
CALL addFlight("MIT","HOB", 2010, "Monday", "21:00:00");

SELECT "Test 1: Adding a reservation, expected OK result" as "Message";
CALL addReservation("MIT","HOB",2010,1,"Monday","09:00:00",3,@a);
SELECT "Check that the reservation number is returned properly (any number will do):" AS "Message",@a AS "Res. number returned"; 

SELECT "Test 2: Adding a reservation with incorrect flightdetails. Expected answer: There exist no flight for the given route, date and time" as "Message";
CALL addReservation("MIT","HOB",2010,1,"Tuesday","21:00:00",3,@b); 

SELECT "Test 3: Adding a reservation when there are not enough seats. Expected answer: There are not enough seats available on the chosen flight" as "Message";
CALL addReservation("MIT","HOB",2010,1,"Monday","09:00:00",61,@c); 

SELECT "Test 4.1: Adding a passenger. Expected OK result" as "Message";
CALL addPassenger(@a,00000001,"Frodo Baggins");

SELECT "Test 4.2: Test whether the same passenger can be added to other reservations. For this test, first add another reservation" as "Message";
CALL addReservation("MIT","HOB",2010,1,"Monday","21:00:00",4,@e);
SELECT @e AS "Reservation number";

SELECT "Now testing. Expected OK result" as "Message";
CALL addPassenger(@e,00000001,"Frodo Baggins"); 

SELECT "Test 5: Adding a passenger with incorrect reservation number. Expected result: The given reservation number does not exist" as "Message";
CALL addPassenger(9999999,00000001,"Frodo Baggins"); 

SELECT "Test 6: Adding a contact. Expected OK result" as "Message";
CALL addContact(@a,00000001,"frodo@magic.mail",080667989); 

SELECT "Test 7: Adding a contact with incorrect reservation number. Expected result: The given reservation number does not exist" as "Message";
CALL addContact(99999,00000001,"frodo@magic.mail",080667989); 

SELECT "Test 8: Adding a contact that is not a passenger on the reservation. Expected result: The person is not a passenger of the reservation" as "Message";
CALL addContact(@a,00000099,"frodo@magic.mail",080667989); 

SELECT "Test 9: Making a payment. Expected OK result" as "Message";
CALL addPayment (@a, "Gandalf", 6767676767676767); 

SELECT "Test 10: Making a payment to a reservation with incorrect reservation number. Expected result: The given reservation number does not exist" as "Message";
CALL addPayment (999999, "Gandalf", 6767676767676767);

SELECT "Test 11: Making a payment to a reservation with no contact. First setting up reservation" as "Message";
CALL addReservation("MIT","HOB",2010,1,"Monday","09:00:00",1,@d); 
CALL addPassenger(@d,00000002,"Sam Gamgee"); 

SELECT "Now testing. Expected result: The reservation has no contact yet" as "Message";
CALL addPayment (@d, "Gandalf", 6767676767676767); 

SELECT "Test 12: Adding a passenger to an already payed reservation. Expected result: The booking has already been payed and no futher passengers can be added" as "Message";
CALL addPassenger(@a,00000003,"Merry Pippins"); 


/******************************************************************************************
 Question 6, Final test to test for check against overbooking, should print the message
	"There are not enough seats available on the flight anymore, deleting reservation".
**********************************************************************************************/


SELECT "Test 13: Testing if an overbooking occurs" as "Message";
SELECT "Preparing the reservation:" as "Message";
/*Fill the database with data */
CALL addReservation("MIT","HOB",2010,1,"Monday","21:00:00",3,@a); 
CALL addPassenger(@a,13000001,"Saruman"); 
CALL addPassenger(@a,13000002,"Orch1"); 
CALL addPassenger(@a,13000003,"Orch2"); 
CALL addPassenger(@a,13000004,"Orch3"); 
CALL addPassenger(@a,13000005,"Orch4"); 
CALL addPassenger(@a,13000006,"Orch5"); 
CALL addPassenger(@a,13000007,"Orch6"); 
CALL addPassenger(@a,13000008,"Orch7"); 
CALL addPassenger(@a,13000009,"Orch8"); 
CALL addPassenger(@a,13000010,"Orch9"); 
CALL addPassenger(@a,13000011,"Orch10"); 
CALL addPassenger(@a,13000012,"Orch11"); 
CALL addPassenger(@a,13000013,"Orch12"); 
CALL addPassenger(@a,13000014,"Orch13"); 
CALL addPassenger(@a,13000015,"Orch14"); 
CALL addPassenger(@a,13000016,"Orch15"); 
CALL addPassenger(@a,13000017,"Orch16"); 
CALL addPassenger(@a,13000018,"Orch17"); 
CALL addPassenger(@a,13000019,"Orch18"); 
CALL addPassenger(@a,13000020,"Orch19"); 
CALL addPassenger(@a,13000021,"Orch20");
CALL addPassenger(@a,13000022,"Orch21"); 
CALL addPassenger(@a,13000023,"Orch22"); 
CALL addPassenger(@a,13000024,"Orch23"); 
CALL addPassenger(@a,13000025,"Orch24"); 
CALL addPassenger(@a,13000026,"Orch25"); 
CALL addPassenger(@a,13000027,"Orch26"); 
CALL addPassenger(@a,13000028,"Orch27"); 
CALL addPassenger(@a,13000029,"Orch28"); 
CALL addPassenger(@a,13000030,"Orch29"); 
CALL addPassenger(@a,13000031,"Orch30");
CALL addPassenger(@a,13000032,"Orch31"); 
CALL addPassenger(@a,13000033,"Orch32"); 
CALL addPassenger(@a,13000034,"Orch33"); 
CALL addPassenger(@a,13000035,"Orch34"); 
CALL addPassenger(@a,13000036,"Orch35"); 
CALL addPassenger(@a,13000037,"Orch36"); 
CALL addPassenger(@a,13000038,"Orch37"); 
CALL addPassenger(@a,13000039,"Orch38"); 
CALL addPassenger(@a,13000040,"Orch39"); 
CALL addPassenger(@a,13000041,"Orch40");
CALL addContact(@a,13000001,"saruman@magic.mail",080667989); 
SELECT "Now testing. Expected result: There are not enough seats available on the flight anymore, deleting reservation" as "Message";
CALL addPayment (@a, "Sauron",7878787878); 
