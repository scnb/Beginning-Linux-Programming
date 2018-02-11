
-- SQL的注释符号有意思
-- Create the table children
--

CREATE TABLE children
(
	childno INTEGER AUTO_INCREMENT NOT NULL PRIMARY KEY,
	fname VARCHAR(30),
	age INTEGER
);

--
-- Populate the table 'children'
--

INSERT INTO children(childno, fname, age) VALUES(2,'Andrew', 17);
INSERT INTO children(childno, fname, age) VALUES(3,'Gavin', 9);
INSERT INTO children(childno, fname, age) VALUES(4,'Duncan', 6);
INSERT INTO children(childno, fname, age) VALUES(5,'Emma', 4);
INSERT INTO children(childno, fname, age) VALUES(6,'Alex', 15);
INSERT INTO children(childno, fname, age) VALUES(7,'Adrian', 9);
