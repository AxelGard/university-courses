# Lab 4 

Lab 4 is a big project consisting of four parts. 

## Lab 4 Part A
Your report must contain the following *two* files:

* `template.drawio` with the source of the diagrams
* `lab4A.pdf` file with the EER and RM diagrams, created from the drawio template **as a PDF document**

Place these files into this gitlab folder.

### Drawing diagrams
1. Use `template.drawio` file from [Lab2](https://gitlab.liu.se/olaha93/db/-/tree/master/lab2)
2. Open it using [draw.io](https://www.draw.io)
3. Follow the instructions from the template and implement the assignment
4. Save the `template.drawio` and export the final `lab4A.pdf` diagram as a PDF file
5. Place the files in this folder
5. Submit as described in the [README](https://gitlab.liu.se/olaha93/db/-/blob/master/README.md)

## Lab 4 Part B
Discuss your design at the mandatory teaching session. 
Fix the design and submit it as part B. Part C requires Part B being approved.

* Submit EER-diagram and Relational Model as a `lab4B.pdf`

## Lab 4 Part C
This is the implementational phase of the project. **Do not start coding until your EER and RM are approved!**

After you've verified that the test scripts return expected results, the following must be handed in: 

<!--* Functional dependencies for all relations as .txt or .pdf-->
* Project code as one file named `lab4C.sql`, which should be executable directly without any error
* Text answers to questions must be included as SQL-comments at the end of `lab4C.sql` file
<!--* An identified secondary index as  SQL-comments in the `lab4C.sql` file (do not implement it), place at the end of the file
* * A file named q10b.sql that is your modified version of Question10MakeBooking.sql

### Functional dependencies 
Find the Candidate keys, primary keys and whether the tables are in BCNF or not. If it is not in BCNF, motivate why! 
-->

### SQL Code
Please delete all your created tables and views in the beginning of the file!

You can do this by temporarily disabling Foreign Keys in MySQL. Do not forget to enable it again!

`
SET FOREIGN_KEY_CHECKS=0;
DROP TABLE XXX;
SET FOREIGN_KEY_CHECKS=1;`

#### The test scripts

**Question 3** 
Confirm that you have 208 flights in your database


**Question 6**
Confirm that the output is correct. 
Confirm that the change has actually been done in the database. E.g. in Test 13, confirm that the reservation was actually removed. 


**Question 7**
This test is correct if there is no output at all. If you get output it means something is wrong. See which rows are different to the result. 
Common errors are rounding errors and route errors

<!--#### Issues
You will most likely run into problems during the coding phase! Create issues, tag your lab assistant and reference the part of the code where you have a problem. -->

## Lab 4 Part D
Submit the approved `lab4C.sql` to Urkund. Rename the SQL-file from `lab4C.sql` to `lab4.txt` and send it to Urkund. Details are on course website.


