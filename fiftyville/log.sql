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
-- 1. Within 10 min of the theft, Ruth saw the thieft get into a car in the bakery parking lot and drive away.
--                 We do not have a time of the theft yet. Still we can search the bakery_security_logs.
-- 2. According to Eugene the theif had widrawn money from Leggett Street
--                 We can search for transactions in tm_transactions.
-- 3. Raymond stated that the theif talked to someone fot less than a minute, The were planning to take the earliest flight tomorrow.
--    The theif asked the person at the other end to purchase the ticket.
--                  We can search for phone_calls during that day that was less than a minute long. Also we can look at the earliest
--                  flight from flights on the day after and search the passengers.

-- Using CLUE 1
        SELECT * FROM bakery_security_logs
        WHERE year = 2021 AND month  = 7 AND day = 28 AND activity = "exit";
-- this gives us hours and license_plate numbers of cars entered into the bakery.

-- Using CLUE 2 and searching for transactions done on that day in Leggett Street.
        SELECT * FROM atm_transactions
        WHERE year = 2021 AND month  = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw";
-- We 8 account_numbers one of which is of the theif's

-- Using CLUE 3 and searching for calls under 1 minute
        SELECT * FROM phone_calls WHERE year = 2021 AND month  = 7 AND day = 28 AND duration < 60;
-- We get 9 results, one of the row contains the thieft and accomplice.

-- Using CLUE 3 about the ticket booking part we can narrow down the flights on the 28 (the next day of the theft).
        SELECT * FROM (SELECT * FROM flights
        JOIN airports ON flights.origin_airport_id = airports.id
        WHERE flights.origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville"))
        WHERE day = 29

-- Clubbing these clues together and narrowing search.
-- Clue 1,2 and first part of 3 talks about the theif.
-- Lets see if we can find a theft with these criterias and worry about accomplice later.


SELECT name
FROM bakery_security_logs

JOIN people
ON people.license_plate = bakery_security_logs.license_plate

JOIN bank_accounts
ON people.id = bank_accounts.person_id

JOIN atm_transactions
ON bank_accounts.account_number = atm_transactions.account_number

JOIN phone_calls
ON people.phone_number = phone_calls.caller

WHERE bakery_security_logs.year = 2021 AND bakery_security_logs.month  = 7 AND bakery_security_logs.day = 28 AND activity = "exit"
AND atm_transactions.year = 2021 AND atm_transactions.month  = 7 AND atm_transactions.day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"
AND phone_calls.year = 2021 AND phone_calls.month  = 7 AND phone_calls.day = 28 AND duration < 60

GROUP BY name;

--| Bruce  |
--| Diana  |
--| Taylor |

-- This gives us three potential people who could be the theif
-- To get the acomplice we have to take search for the name of the receivers

SELECT people.name
FROM(SELECT name, receiver
FROM bakery_security_logs
JOIN people
ON people.license_plate = bakery_security_logs.license_plate
JOIN bank_accounts
ON people.id = bank_accounts.person_id
JOIN atm_transactions
ON bank_accounts.account_number = atm_transactions.account_number
JOIN phone_calls
ON people.phone_number = phone_calls.caller
WHERE bakery_security_logs.year = 2021 AND bakery_security_logs.month  = 7 AND bakery_security_logs.day = 28 AND activity = "exit"
AND atm_transactions.year = 2021 AND atm_transactions.month  = 7 AND atm_transactions.day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"
AND phone_calls.year = 2021 AND phone_calls.month  = 7 AND phone_calls.day = 28 AND duration < 60
GROUP BY name) AS subquery
JOIN people ON subquery.receiver = people.phone_number;


-- We got three people who could be our accomplice.

--| James  |
--| Philip |
--| Robin  |

-- Now we have to check all the passengers who travelled in flights from fiftyvill on the day after

SELECT *

FROM (SELECT * FROM flights
JOIN airports ON flights.origin_airport_id = airports.id
WHERE flights.origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville"))
AS flights

JOIN passengers ON flights.id = passengers.flight_id
JOIN people ON passengers.passport_number = people.passport_number

WHERE day = 29
AND name = "James" OR name = "Philip" OR name = "Robin" OR name = "Bruce" OR name = "Diana" OR name = "Taylor"
ORDER BY hour;

--+----+-------------------+------------------------+------+-------+-----+------+--------+------+--------------+-----------------------------+------------+-----------+-----------------+------+--------+--------+----------------+-----------------+---------------+
--| id | origin_airport_id | destination_airport_id | year | month | day | hour | minute | id:1 | abbreviation |          full_name          |    city    | flight_id | passport_number | seat |   id   |  name  |  phone_number  | passport_number | license_plate |
--+----+-------------------+------------------------+------+-------+-----+------+--------+------+--------------+-----------------------------+------------+-----------+-----------------+------+--------+--------+----------------+-----------------+---------------+
--| 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     | 8    | CSF          | Fiftyville Regional Airport | Fiftyville | 36        | 5773159633      | 4A   | 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |
--| 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     | 8    | CSF          | Fiftyville Regional Airport | Fiftyville | 36        | 1988161715      | 6D   | 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       |
--| 54 | 8                 | 5                      | 2021 | 7     | 30  | 10   | 19     | 8    | CSF          | Fiftyville Regional Airport | Fiftyville | 54        | 3592750733      | 6C   | 514354 | Diana  | (770) 555-1861 | 3592750733      | 322W7JE       |
--| 10 | 8                 | 4                      | 2021 | 7     | 30  | 13   | 55     | 8    | CSF          | Fiftyville Regional Airport | Fiftyville | 10        | 3391710505      | 2A   | 847116 | Philip | (725) 555-3243 | 3391710505      | GW362R6       |
--| 18 | 8                 | 6                      | 2021 | 7     | 29  | 16   | 0      | 8    | CSF          | Fiftyville Regional Airport | Fiftyville | 18        | 3592750733      | 4C   | 514354 | Diana  | (770) 555-1861 | 3592750733      | 322W7JE       |
--+----+-------------------+------------------------+------+-------+-----+------+--------+------+--------------+-----------------------------+------------+-----------+-----------------+------+--------+--------+----------------+-----------------+---------------+


-- 5 out of the six people have travelled in flighst from fiftyville
-- But 3 people had boarded different flights while 2 had boarded same flight and also the flight is the earliest flight of that day.

-- So we can conform that its "Bruce" and "Taylor" and they had flown to destination_aiport_id = 4.
SELECT city
FROM airports
WHERE id = 4;
-- Which is "New York City"


-- We can conform who is the thieft by checking both of thier license in the bakery_security_log
SELECT *
FROM bakery_security_logs

JOIN people
ON people.license_plate = bakery_security_logs.license_plate

WHERE bakery_security_logs.year = 2021 AND bakery_security_logs.month  = 7 AND bakery_security_logs.day = 28
AND bakery_security_logs.license_plate = "94KL13X" OR  bakery_security_logs.license_plate = "1106N58";


-- While cheking at the bakery_security_log we find that both Taylor and Bruce had entered the premise and excited at different times

--+-----+------+-------+-----+------+--------+----------+---------------+--------+--------+----------------+-----------------+---------------+
--| id  | year | month | day | hour | minute | activity | license_plate |   id   |  name  |  phone_number  | passport_number | license_plate |
--+-----+------+-------+-----+------+--------+----------+---------------+--------+--------+----------------+-----------------+---------------+
--| 232 | 2021 | 7     | 28  | 8    | 23     | entrance | 94KL13X       | 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |
--| 237 | 2021 | 7     | 28  | 8    | 34     | entrance | 1106N58       | 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       |
--| 261 | 2021 | 7     | 28  | 10   | 18     | exit     | 94KL13X       | 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |
--| 268 | 2021 | 7     | 28  | 10   | 35     | exit     | 1106N58       | 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       |
--+-----+------+-------+-----+------+--------+----------+---------------+--------+--------+----------------+-----------------+---------------+


-- We know that after this the theif widraw money from the atm and also called the accomplce so we can join those two informations and find who is who.

-- Or we can check the call logs of both Bruce and Taylor

SELECT *
fROM (SELECT caller, receiver, year, month, day, duration
        FROM phone_calls
        JOIN people ON phone_calls.caller = people.phone_number)
AS subquery
JOIN people ON subquery.receiver = people.phone_number

WHERE subquery.year = 2021 AND subquery.month  = 7 AND subquery.day = 28 AND subquery.duration < 60;

--+----------------+----------------+------+-------+-----+----------+--------+------------+----------------+-----------------+---------------+
--|     caller     |    receiver    | year | month | day | duration |   id   |    name    |  phone_number  | passport_number | license_plate |
--+----------------+----------------+------+-------+-----+----------+--------+------------+----------------+-----------------+---------------+
--| (130) 555-0289 | (996) 555-8899 | 2021 | 7     | 28  | 51       | 567218 | Jack       | (996) 555-8899 | 9029462229      | 52R0Y8U       |
--| (499) 555-9472 | (892) 555-8872 | 2021 | 7     | 28  | 36       | 251693 | Larry      | (892) 555-8872 | 2312901747      | O268ZZ0       |
--| (367) 555-5533 | (375) 555-8161 | 2021 | 7     | 28  | 45       | 864400 | Robin      | (375) 555-8161 | NULL            | 4V16VO0       |  <---------
--| (499) 555-9472 | (717) 555-1342 | 2021 | 7     | 28  | 50       | 626361 | Melissa    | (717) 555-1342 | 7834357192      | NULL          |
--| (286) 555-6063 | (676) 555-6554 | 2021 | 7     | 28  | 43       | 250277 | James      | (676) 555-6554 | 2438825627      | Q13SVG6       |  <---------
--| (770) 555-1861 | (725) 555-3243 | 2021 | 7     | 28  | 49       | 847116 | Philip     | (725) 555-3243 | 3391710505      | GW362R6       |
--| (031) 555-6622 | (910) 555-3251 | 2021 | 7     | 28  | 38       | 712712 | Jacqueline | (910) 555-3251 | NULL            | 43V0R5D       |
--| (826) 555-1652 | (066) 555-9701 | 2021 | 7     | 28  | 55       | 953679 | Doris      | (066) 555-9701 | 7214083635      | M51FA04       |
--| (338) 555-6650 | (704) 555-2131 | 2021 | 7     | 28  | 54       | 484375 | Anna       | (704) 555-2131 | NULL            | NULL          |
--+----------------+----------------+------+-------+-----+----------+--------+------------+----------------+-----------------+---------------+

-- Here we see that "Taylor" and "Bruce" had not called eacother on 29th.
-- But Taylor had called James
-- And Bruce had called Robin

SELECT *
FROM bakery_security_logs

JOIN people
ON people.license_plate = bakery_security_logs.license_plate

WHERE name = "Taylor" OR name = "Bruce";

--+-----+------+-------+-----+------+--------+----------+---------------+--------+--------+----------------+-----------------+---------------+
--| id  | year | month | day | hour | minute | activity | license_plate |   id   |  name  |  phone_number  | passport_number | license_plate |
--+-----+------+-------+-----+------+--------+----------+---------------+--------+--------+----------------+-----------------+---------------+
--| 237 | 2021 | 7     | 28  | 8    | 34     | entrance | 1106N58       | 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       |
--| 268 | 2021 | 7     | 28  | 10   | 35     | exit     | 1106N58       | 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       |
--| 232 | 2021 | 7     | 28  | 8    | 23     | entrance | 94KL13X       | 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |
--| 261 | 2021 | 7     | 28  | 10   | 18     | exit     | 94KL13X       | 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |
--+-----+------+-------+-----+------+--------+----------+---------------+--------+--------+----------------+-----------------+---------------+

-- Eventhough both have entered and exited the bakery on that day on similar times. "Taylor" exited at 10:35 in the morning while "Bruce" exited at 10:18.
-- The crime took place on 10:15.
-- According to the interview given by Ruth, who said that they had seen the thieft getting out of the bakery within 10 minutes of the crime.
-- Thus, we can confirm that our thieft is "Bruce"
-- and the person who he called for less than a minute on that day, "Robin" is our accomplice.
