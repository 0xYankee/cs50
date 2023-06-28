-- Keep a log of any SQL queries you execute as you solve the mystery.

-- To understand the tables in the database.
.schema fiftyville.db



-- All you know is that the theft took place on July 28, 2021 and that it took place on Humphrey Street.
-- Query to learn about what was reported during day of crime.
-- Theft of the CS50 duck took place at 10:15am at Humphrey Street Bakery
-- Interviews were conducted with 3 witnesses
SELECT *
FROM crime_scene_reports
WHERE year = 2021
AND month = 7
AND day = 28;



-- Query to learn about what the witnesses provided during the interview
-- Ruth mentioned within 10 mins of theft, theif went into a car in the parking lot and drove away.
-- Eugene mentioned he recognized the theft, at the ATM on Leggett Street, he saw the thief withdrawing money.
-- Raymond mentioned when the theif was leaving the bakery, suspect was calling someone which suspect mentioned they are taking the earliest flight out of Fiftyville tomorrow (29 July 2021) and told the other party of the call to purchase a flight ticket.
SELECT *
FROM interviews
WHERE year = 2021
AND month = 7
AND day = 28;



-- Tracing Eugene's remarks, query the ATM transactions table to learn about the theif's identity.
-- To note, the theft withdrew money on Leggett Street.
-- Join the bank accounts and people table to identify suspects.
-- Suspect information who withdrew money:
-- |  name   |  phone_number  | passport_number | license_plate |
-- | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
-- | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
-- | Brooke  | (122) 555-4581 | 4408372428      | QX4YZN3       |
-- | Kenny   | (826) 555-1652 | 9878712108      | 30G67EN       |
-- | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |
-- | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
-- | Taylor  | (286) 555-6063 | 1988161715      | 1106N58       |
-- | Benista | (338) 555-6650 | 9586786673      | 8X428L0       |
SELECT *
FROM atm_transactions AS a
JOIN bank_accounts AS b, people AS p
ON a.account_number = b.account_number
AND b.person_id = p.id
WHERE year = 2021
AND month = 7
AND day = 28
AND atm_location = 'Leggett Street'
AND transaction_type = 'withdraw';



-- Tracing Ruth's remarks, join the previous query to match the car the theif used to identify suspect.
-- To note that the crime took place on July 28, 10:15am and car left within 10 minutes.
-- | 94KL13X | Bruce   | (367) 555-5533 |
-- | 4328GD8 | Luca    | (389) 555-5198 |
-- | L93JTIZ | Iman    | (829) 555-5269 |
-- | 322W7JE | Diana   | (770) 555-1861 |
-- | 8X428L0 | Taylor  | (286) 555-6063 |
SELECT name, phone_number
FROM people AS p
JOIN bakery_security_logs AS b
ON b.license_plate = p.license_plate
WHERE year = 2021
AND month = 7
AND day = 28
AND hour = 10
AND minute BETWEEN 15 AND 25
AND activity = 'exit'
AND b.license_plate
IN (
    SELECT license_plate
    FROM atm_transactions AS a
    JOIN bank_accounts AS b, people AS p
    ON a.account_number = b.account_number
    AND b.person_id = p.id
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND atm_location = 'Leggett Street'
    AND transaction_type = 'withdraw'
);



-- Tracing Raymond's remarks, query the phone calls table to further narrow who called during the period of theft.
-- To note that suspect called the accomplice for less than a minute.
-- | (367) 555-5533 | Robin  | (375) 555-8161 |
-- | (770) 555-1861 | Philip | (725) 555-3243 |
SELECT caller, name, receiver
FROM phone_calls
JOIN people
ON people.phone_number = phone_calls.receiver
WHERE year = 2021
AND month = 7
AND day = 28
AND duration < 60
AND phone_calls.caller
IN (
    SELECT phone_number
    FROM people AS p
    JOIN bakery_security_logs AS b
    ON b.license_plate = p.license_plate
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND hour = 10
    AND activity = 'exit'
    AND minute BETWEEN 15 AND 25
    AND b.license_plate
    IN (
        SELECT license_plate
        FROM atm_transactions AS a
        JOIN bank_accounts AS b, people AS p
        ON a.account_number = b.account_number
        AND b.person_id = p.id
        WHERE year = 2021
        AND month = 7
        AND day = 28
        AND atm_location = 'Leggett Street'
        AND transaction_type = 'withdraw'
    )
);


-- Tracing Raymond's remarks, query the flights table to narrow down which flight the suspects are taking.
-- To note, the suspect mentioned to take the earliest flight out of Fiftyville the day after the theft (29 July 2021)
-- | 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     | 8  | CSF          | Fiftyville Regional Airport | Fiftyville |
-- | 43 | 8                 | 1                      | 2021 | 7     | 29  | 9    | 30     | 8  | CSF          | Fiftyville Regional Airport | Fiftyville |
-- | 23 | 8                 | 11                     | 2021 | 7     | 29  | 12   | 15     | 8  | CSF          | Fiftyville Regional Airport | Fiftyville |
-- | 53 | 8                 | 9                      | 2021 | 7     | 29  | 15   | 20     | 8  | CSF          | Fiftyville Regional Airport | Fiftyville |
-- | 18 | 8                 | 6                      | 2021 | 7     | 29  | 16   | 0      | 8  | CSF          | Fiftyville Regional Airport | Fiftyville |
SELECT *
FROM flights
JOIN airports
ON airports.id = flights.origin_airport_id
WHERE year = 2021
AND month = 7
AND day = 29
AND airports.city = 'Fiftyville'
ORDER BY hour, minute;



-- The earliest flight on 29 July 2021 going out of Fiftyville is 8:20am with the id of 36.
-- Using the previous query, we can check if the passengers in the flight match our suspects.
-- | 36        | 7214083635      | 2A   | 953679 | Doris  | (066) 555-9701 | 7214083635      | M51FA04       |
-- | 36        | 1695452385      | 3B   | 398010 | Sofia  | (130) 555-0289 | 1695452385      | G412CB7       |
-- | 36        | 5773159633      | 4A   | 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       |
-- | 36        | 1540955065      | 5C   | 651714 | Edward | (328) 555-1152 | 1540955065      | 130LD9Z       |
-- | 36        | 8294398571      | 6C   | 560886 | Kelsey | (499) 555-9472 | 8294398571      | 0NTHK55       |
-- | 36        | 1988161715      | 6D   | 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       |
-- | 36        | 9878712108      | 7A   | 395717 | Kenny  | (826) 555-1652 | 9878712108      | 30G67EN       |
-- | 36        | 8496433585      | 7B   | 467400 | Luca   | (389) 555-5198 | 8496433585      | 4328GD8       |
SELECT *
FROM passengers AS p
JOIN people AS ppl
ON ppl.passport_number = p.passport_number
WHERE p.flight_id
IN (
    SELECT flights.id
    FROM flights
    JOIN airports
    ON airports.id = flights.origin_airport_id
    WHERE year = 2021
    AND month = 7
    AND day = 29
    AND airports.city = 'Fiftyville'
    ORDER BY hour, minute
    LIMIT 1
);



-- Bruce is the prime suspect for the CS50 case.
-- As he appeared in both the suspect list and on the intended flight to New York City.
-- This makes Robin his accomplice.