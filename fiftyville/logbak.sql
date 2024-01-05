-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Using the first clue we can get a discription of the crime from the crime_scene_reports.
SELECT *
FROM crime_scene_reports
WHERE year = 2021 AND month = 7 AND day = 28 AND street = "Humphrey Street";

-- The description mentions that
-- Interviews were conducted today with three witnesses,
-- each of their interview transcripts mentions the bakery."

-- As per the discription we search for interviews which mention bakery.
SELECT * FROM interviews WHERE transcript LIKE "%bakery%";

-- We got there new clues from the interviews.
-- 1. Within 10 min of the theft, Ruth saw the thieft get into a car in the bakery parking lot adn drive away.
--                 We do not have a time of the theft yet. Still we can search the bakery_security_logs.
-- 2. According to Eugene the theif had widrawn money from Leggett Street
--                 We can search for transactions in tm_transactions.
-- 3. Raymond stated that the theif talked to someone fot less than a minute, The were planning to take the earliest flight tomorrow.
--    The theif asked the person at the other end to purchase the ticket.
--                  We can search for phone_calls during that day that was less than a minute long. Also we can look at the earliest
--                  flight from flights on the day after and search the passengers.

-- Using CLUE 1
SELECT * FROM bakery_security_logs
WHERE year = 2021 AND month  = 7 AND day = 28 AND activity = "entrance";
-- this gives us hours and license_plate numbers of cars entered into the bakery.



-- Using CLUE 2 and searching for transactions done on that day in Leggett Street.
SELECT * FROM atm_transactions WHERE year = 2021 AND month  = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw";
-- We 8 account_numbers one of which is of the theif's

-- Using CLUE 3 and searching for calls under 1 minute
SELECT * FROM phone_calls WHERE year = 2021 AND month  = 7 AND day = 28 AND duration < 60;
-- We get 9 results, one of the row contains the thieft and accomplice.

-- Using CLUE 3 about the ticket booking part we can narrow down the flights on the 28 (the next day of the theft).
SELECT * FROM (SELECT * FROM flights
JOIN airports ON flights.origin_airport_id = airports.id
WHERE flights.origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville"))
WHERE day = 29
ORDER BY hour;

-- We assume for the moment that the theft and accomplice have booked tickets on the earliest flight which is flight id 6.
SELECT * FROM flights WHERE id = 6;
-- This gives us 8 potential persons one of them could be the thief

-- We can get the informations of the passengers of this flight by joining the people table to this.
SELECT passengers.passport_number, name, id, phone_number, license_plate
FROM passengers
JOIN people ON passengers.passport_number = people.passport_number
WHERE flight_id = 6;
-- This gives us  passport_number, name, id, phone_number, license_plate of the passengers of the earliest flight which we assume is what the theif took

-- We can join the earlier narrowed down tables to this new table to narrow even further

SELECT passengers.passport_number, name, people.id, phone_number, license_plate
FROM passengers
JOIN people ON passengers.passport_number = people.passport_number
JOIN phone_calls ON people.phone_number = phone_calls.caller
WHERE flight_id = 6
GROUP BY name;
-- Here we joined the phone_calls as well


---------------------------------------------------------------------------------



-- Passengers who travelled from Fiftyville on 29th with a phone
SELECT *
FROM passengers
JOIN people ON passengers.passport_number = people.passport_number
JOIN phone_calls ON people.phone_number = phone_calls.receiver
WHERE flight_id = (SELECT id FROM flights WHERE origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville")) AND phone_calls.duration < 60;
GROUP BY name;

-- HERE WE GET AN INITIAL SUSPECT FOR THE ACCOMPLICE AS THEY BOUGHT THE TICKET and so THE CALLED WILL BE OUR THIEF
--+-----------+-----------------+------+--------+------+----------------+-----------------+---------------+-----+----------------+----------------+------+-------+-----+----------+
--| flight_id | passport_number | seat |   id   | name |  phone_number  | passport_number | license_plate | id  |     caller     |    receiver    | year | month | day | duration |
--+-----------+-----------------+------+--------+------+----------------+-----------------+---------------+-----+----------------+----------------+------+-------+-----+----------+
--| 1         | 3412604728      | 5A   | 440007 | Sara | (340) 555-8872 | 3412604728      | 99A843C       | 313 | (344) 555-9601 | (340) 555-8872 | 2021 | 7     | 29  | 48       |
--+-----------+-----------------+------+--------+------+----------------+-----------------+---------------+-----+----------------+----------------+------+-------+-----+----------+

-- Lets check the person with the number (344) 555-9601.
 SELECT * FROM people WHERE phone_number = "(344) 555-9601";

--+--------+---------+----------------+-----------------+---------------+
--|   id   |  name   |  phone_number  | passport_number | license_plate |
--+--------+---------+----------------+-----------------+---------------+
--| 985497 | Deborah | (344) 555-9601 | 8714200946      | 10I5658       |
--+--------+---------+----------------+-----------------+---------------+

--Lets check if such a license plate was recorded in the parking lot

SELECT * FROM bakery_security_logs WHERE license_plate = "10I5658";

--+-----+------+-------+-----+------+--------+----------+---------------+
--| id  | year | month | day | hour | minute | activity | license_plate |
--+-----+------+-------+-----+------+--------+----------+---------------+
--| 363 | 2021 | 7     | 30  | 9    | 33     | entrance | 10I5658       |
--| 413 | 2021 | 7     | 30  | 14   | 58     | exit     | 10I5658       |
--+-----+------+-------+-----+------+--------+----------+---------------+

--We see that this person's car had entered and exited the parking but it was on 30th.



SELECT *
FROM passengers
JOIN people ON passengers.passport_number = people.passport_number
JOIN phone_calls ON people.phone_number = phone_calls.receiver
JOIN
WHERE flight_id = (SELECT id FROM flights WHERE origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville")) AND
GROUP BY name;






SELECT *
FROM passengers
JOIN flights ON passengers.flight_id = flights.id
JOIN people ON passengers.passport_number = people.passport_number
JOIN phone_calls ON people.phone_number = phone_calls.receiver
JOIN (SELECT person_id FROM bank_accounts
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE year = 2021 AND month  = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"
) AS subset
ON people.id = subset.person_id
WHERE flight_id = (SELECT id FROM flights WHERE origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville")) AND flights.day = 29 AND phone_calls.day = 29 AND phone_calls.duration < 60
GROUP BY people.name;









--STEP ONE

--PASSENGERS WHO TRAVELLED ON 29th FROM FIFTYVILLE
SELECT *
FROM passengers
JOIN flights ON passengers.flight_id = flights.id
WHERE origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville") AND flights.day = 29;


----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


SELECT people.id, people.name, people.phone_number, people.passport_number, people.license_plate, phone_calls.day
FROM passengers

JOIN flights ON passengers.flight_id = flights.id
JOIN people ON passengers.passport_number = people.passport_number
JOIN phone_calls ON people.phone_number = phone_calls.receiver
JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate


WHERE origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville")

AND flights.day = 29
AND phone_calls.day = 28 AND phone_calls.duration < 60
AND people.license_plate != "NULL"
GROUP BY people.name;


--*** POSSIBLE ACCOMPLICE ***--

--+--------+-------+----------------+-----------------+---------------+-----+
--|   id   | name  |  phone_number  | passport_number | license_plate | day |
--+--------+-------+----------------+-----------------+---------------+-----+
--| 953679 | Doris | (066) 555-9701 | 7214083635      | M51FA04       | 28  |
--+--------+-------+----------------+-----------------+---------------+-----+

SELECT * FROM phone_calls
WHERE receiver = "(066) 555-9701"
AND duration < 60;

--+-----+----------------+----------------+------+-------+-----+----------+
--| id  |     caller     |    receiver    | year | month | day | duration |
--+-----+----------------+----------------+------+-------+-----+----------+
--| 279 | (826) 555-1652 | (066) 555-9701 | 2021 | 7     | 28  | 55       |
--+-----+----------------+----------------+------+-------+-----+----------+

-- whose details can be derived from the peoples table

SELECT *
FROM people
WHERE phone_number = "(826) 555-1652";

--+--------+-------+----------------+-----------------+---------------+
--|   id   | name  |  phone_number  | passport_number | license_plate |
--+--------+-------+----------------+-----------------+---------------+
--| 395717 | Kenny | (826) 555-1652 | 9878712108      | 30G67EN       |
--+--------+-------+----------------+-----------------+---------------+

-- We have to confirm if the possible theif had been entered into the bakery security log.

SELECT *
FROM bakery_security_logs
WHERE license_plate = "30G67EN";


SELECT *
FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE phone_number = "(826) 555-1652";

--+--------+-------+----------------+-----------------+---------------+----------------+-----------+---------------+
--|   id   | name  |  phone_number  | passport_number | license_plate | account_number | person_id | creation_year |
--+--------+-------+----------------+-----------------+---------------+----------------+-----------+---------------+
--| 395717 | Kenny | (826) 555-1652 | 9878712108      | 30G67EN       | 28296815       | 395717    | 2014          |
--+--------+-------+----------------+-----------------+---------------+----------------+-----------+---------------+

SELECT *
FROM atm_transactions
WHERE account_number = "28296815" AND day =28;


--** OUR SUSPECT HAS WITHDRAWN MONEY FROM LEGGETT STREET ON 28

--+-----+----------------+------+-------+-----+----------------+------------------+--------+
--| id  | account_number | year | month | day |  atm_location  | transaction_type | amount |
--+-----+----------------+------+-------+-----+----------------+------------------+--------+
--| 264 | 28296815       | 2021 | 7     | 28  | Leggett Street | withdraw         | 20     |
--+-----+----------------+------+-------+-----+----------------+------------------+--------+


SELECT flights.destination_airport_id
FROM passengers

JOIN flights ON passengers.flight_id = flights.id
JOIN people ON passengers.passport_number = people.passport_number
JOIN phone_calls ON people.phone_number = phone_calls.receiver
JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate


WHERE origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville")

AND flights.day = 29 AND people.name = "Kenny"
GROUP BY people.name;


--+------------------------+
--| destination_airport_id |
--+------------------------+
--| 4                      |
--+------------------------+

SELECT city FROM airports WHERE id = 4;

--+---------------+
--|     city      |
--+---------------+
--| New York City |
--+---------------+
