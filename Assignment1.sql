-- 1
select name
from Pokemon
where type = 'Grass'
order by name asc;

-- 2
select name
from Trainer
where hometown = 'Brown City' or hometown = 'Rainbow City'
order by name asc;

-- 3
select distinct type
from Pokemon
order by type asc;

-- 4
select name
from City
where name like 'B%'
order by name asc;

-- 5
select hometown 
from Trainer 
where name not in (select name from Trainer where name like 'M%')
order by hometown;

-- 6
select nickname
from CatchedPokemon
where level = (select max(level)
from CatchedPokemon)
order by nickname;

-- 7
select name
from Pokemon
where name like 'A%' or name like 'E%' or name like 'I%' or name like 'O%' or name like 'U%'
order by name;

-- 8
select avg(level)
from CatchedPokemon;

-- 9
select max(level)
from CatchedPokemon
where owner_id = (select id from Trainer where name = 'Yellow');

-- 10
select distinct hometown
from Trainer
order by hometown;

-- 11
select name, nickname
from Trainer, CatchedPokemon as C
where C.nickname like 'A%' and Trainer.id = C.owner_id
order by name;

-- 12
select name
from Trainer
where Trainer.id = (select leader_id from Gym where Gym.city = (select name from City where City.description = 'Amazon'));

-- 13
with max_select as (
select distinct owner_id, count(pid) as C
from CatchedPokemon
where pid in (select id from Pokemon where type = 'Fire')
group by owner_id)
select *
from max_select
order by C desc
limit 1;

-- 14 
select A.type 
from (select avg(id) as avg_id,
		type from Pokemon where id<10 
        group by type 
        order by avg_id desc)A;

-- 15
select count(id)
from Pokemon
where type != 'Fire';

-- 16
select name 
from Pokemon join Evolution on Evolution.before_id = Pokemon.id
where Evolution.after_id < Evolution.before_id
order by Pokemon.name;

-- 17
select avg(level)
from CatchedPokemon join Pokemon on CatchedPokemon.pid = Pokemon.id
where Pokemon.type = 'Water';

-- 18
With my_list as (
	select *
    from CatchedPokemon join Gym on owner_id = leader_id
)
select nickname
from my_list
where level = (select max(level) from my_list);

-- 19
With my_table as (
	select Trainer.name, avg(level) as avg_level
    from CatchedPokemon join Trainer on Trainer.id = CatchedPokemon.owner_id
    where Trainer.hometown = 'Blue City'
    group by Trainer.name
)
select name
from my_table
where avg_level = (select max(avg_level) from my_table);

-- 20
With my_table as (
	select hometown, count(hometown) as cnt_hometown
    from Trainer
    group by hometown
)
select Pokemon.name
from (((Trainer natural join my_table) join CatchedPokemon on Trainer.id = owner_id) join Evolution on pid = before_id) join Pokemon on pid = Pokemon.id 
where cnt_hometown = 1 AND type  = 'Electric';

-- 21
with my_table as (
	select leader_id, sum(level) as sum_level
    from Gym join CatchedPokemon on Gym.leader_id = CatchedPokemon.owner_id
    group by leader_id
)
select Trainer.name, sum_level
from my_table join Trainer on leader_id = id
order by sum_level desc;

-- 22
With my_table as (
	select hometown, count(hometown) as cnt_hometown
    from Trainer
    group by hometown
)
select hometown
from my_table
where cnt_hometown = (select max(cnt_hometown) from my_table);

-- 23
With my_table as (
	select pid
    from Trainer join CatchedPokemon on Trainer.id = CatchedPokemon.owner_id
    where Trainer.hometown = 'Sangnok City'
)
select distinct Pokemon.name
from Pokemon
where id in (select distinct my_table.pid
from Trainer join CatchedPokemon on Trainer.id = CatchedPokemon.owner_id, my_table
where hometown = 'Brown City' AND CatchedPokemon.pid = my_table.pid);

-- 24
with my_table as(
	select id
    from Pokemon
    where name like 'P%'
)
select Trainer.name
from my_table join CatchedPokemon on my_table.id = pid, Trainer
where CatchedPokemon.owner_id = Trainer.id AND Trainer.hometown = 'Sangnok City';

-- 25
select Trainer.name, Pokemon.name
from (Trainer join CatchedPokemon on Trainer.id = CatchedPokemon.owner_id) join Pokemon on Pokemon.id = CatchedPokemon.pid
order by Trainer.name asc, Pokemon.name asc;

-- 26
with my_table as(
	select A.before_id, A.after_id
    from Evolution as A, Evolution as B
    where A.after_id = B.before_id
)
select Pokemon.name
from Evolution join Pokemon on Evolution.before_id = Pokemon.id
where before_id not in (select before_id from my_table) AND before_id not in (select after_id from my_table)
order by name;

-- 27
select CatchedPokemon.nickname
from (Gym join CatchedPokemon on leader_id = owner_id) join Pokemon on CatchedPokemon.pid = Pokemon.id
where Gym.City = 'Sangnok City' AND Pokemon.type = 'Water';

-- 28
with my_table as(
	select owner_id, count(owner_id) as cnt_3
    from CatchedPokemon, Evolution
    where Evolution.after_id = pid
    group by owner_id
)
select Trainer.name
from Trainer join my_table on Trainer.id = my_table.owner_id
where cnt_3 >= 3
order by Trainer.name;

-- 29
select Pokemon.name
from Pokemon
where Pokemon.id not in (select pid from CatchedPokemon)
order by name;

-- 30
with my_table as(
	select Trainer.hometown, max(level) as max_level
    from Trainer join CatchedPokemon on Trainer.id = CatchedPokemon.owner_id
    group by Trainer.hometown
)
select max_level
from my_table
order by max_level desc;

-- 31
with my_table as (
	select A.before_id as firstID, B.before_id as secondID, C.after_id as thirdID
    from Evolution A, Evolution B, Evolution C
    where A.after_id = B.before_id AND B.after_id = C.after_id
)
select firstID as id, A.name as FirstName, B.name as SecondName, C.name as ThirdName
from my_table, Pokemon as A, Pokemon as B, Pokemon as C
where A.id = (select id from Pokemon where my_table.firstID = Pokemon.id)
AND A.id = my_table.firstID
AND B.id = my_table.secondID
AND C.id = my_table.thirdID
order by id, FirstName, SecondName, ThirdName;