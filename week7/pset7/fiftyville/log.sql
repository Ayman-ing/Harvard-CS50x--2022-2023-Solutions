-- Keep a log of any SQL queries you execute as you solve the mystery.
--reading the description of the crime
select description from crime_scene_reports where month=7 and day=28 and street='Humphrey Street';
--reading the transcript of the interviews
select name,transcript from interviews where year=2021 and month=7 and day=28;
--relying on the transcript of ruth wi will check the bakery_security_logs after ten minutes of the theft
select activity,license_plate from bakery_security_logs where year=2021 and month=7 and day=28 and hour=10 and minute between 20 and 30;
--relying on the transcript of Eugene finding the account of the transaction
select account_number,transaction_type from atm_transactions where year=2021 and month=7 and day=28 and atm_location='Leggett Street';
--relying on raymond transcript we will check the persons that take a flight on 29 july 2021
select passport_number from passengers join flights on passengers.flight_id=flights.id where year=2021 and month=7 and day=29;

--matching all the queries




 select name  from people where passport_number in (select passport_number from passengers join flights on passengers.flight_id=flights.id where year=2021 and month=7 and day=29)and license_plate in(select  license_plate from bakery_security_logs where year=2021 and month=7 and day=28 and hour=10 and minute between 15 and 25) and phone_number in(select caller from phone_calls where year=2021 and month=7 and day=28)and id in(select person_id from bank_accounts where account_number in(select account_number from atm_transactions where year=2021 and month=7 and day=28 and atm_location='Leggett Street'));
 --after knowing the tief (Bruce) checking the receiver of the call to know who helped him
 select name from people where phone_number=(select receiver  from phone_calls where year=2021 and month=7 and day=28 and caller in(select phone_number from people where name ='Bruce'));
 --and then looking for the name of the city where Bruce escaped
 select destination_airport_id from flights where id =(select flight_id from passengers where passport_number=(select passport_number from people where name='Bruce'));
 select * from airports where id=4;
