create table test (Company char(10), OrderNumber int);
insert into test values('IBM',3532);
insert into test values('Microsoft',2356);
insert into test values('Apple',4698);
insert into test values('Microsoft',6953);
SELECT Company, OrderNumber FROM test ORDER BY Company;
SELECT Company, OrderNumber FROM test ORDER BY Company, OrderNumber;
SELECT Company, OrderNumber FROM test ORDER BY Company DESC, OrderNumber ASC;
SELECT Company, OrderNumber FROM test ORDER BY OrderNumber ASC LIMIT 2;
drop table test;
#