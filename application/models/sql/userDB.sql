CREATE DATABASE user_management;

GRANT SELECT ON user_management.*
TO mailuser@localhost
IDENTIFIED BY 'mailpassword';

-- Allow VPF to alter the refunds
--GRANT SELECT, INSERT, UPDATE ON user_management.refunds
--TO vpf@localhost
--IDENTIFIED BY 'vpfpassword';

USE user_management;

-- Everything we care about is a unit.
CREATE TABLE `units` (
  id		INT(8)		NOT NULL	AUTO_INCREMENT,
  alias		VARCHAR(16)	NOT NULL,
  name		VARCHAR(128)	NOT NULL,
  url		VARCHAR(255),

  PRIMARY KEY (id)
)ENGINE=INNODB;

INSERT INTO `units` (`alias`,`name`,`url`) VALUES
('MathSoc','Mathematics Society','http://mathsoc.uwaterloo.ca');

-- Our virtual domains are the domains we're accepting email for.
CREATE TABLE `virtual_domains` (
  id	INT(8)		NOT NULL	AUTO_INCREMENT,
  name	VARCHAR(50)	NOT NULL,

  PRIMARY KEY (id)
)ENGINE=INNODB;

INSERT INTO `virtual_domains` (name) VALUES
('mathsoc.uwaterloo.ca');

-- There is a one to many correlation of units to domains
CREATE TABLE `domain_map` (
  unitId	INT(8)		NOT NULL,
  domainId	INT(8)		NOT NULL,

  PRIMARY KEY (unitId,domainId),
  FOREIGN KEY (unitId) REFERENCES units(id) ON DELETE CASCADE,
  FOREIGN KEY (domainId) REFERENCES virtual_domains(id) ON DELETE CASCADE
)ENGINE=INNODB;

INSERT INTO `domain_map` (`unitId`, `domainId`) VALUES
('1','1');

CREATE TABLE `users` (
  userId	CHAR(8)		NOT NULL,
  password	VARCHAR(40),
  email		VARCHAR(255),
  added		DATETIME	NOT NULL	DEFAULT CURRENT_TIMESTAMP,

  PRIMARY KEY( userId )
)ENGINE=INNODB;

-- This is the users table.  It stores information about users.
CREATE TABLE `volunteers` (
  userId	CHAR(8)		NOT NULL,
  name		VARCHAR(127)	NOT NULL,
  description	TEXT,
  photo		VARCHAR(255),
  photo_data	BLOB,
  photo_type	ENUM('png','jpeg','gif'),
  office_trained BOOL		NOT NULL	DEFAULT '0',

  PRIMARY KEY( userId ),
  FOREIGN KEY (userId) REFERENCES users(userId) ON UPDATE CASCADE
)ENGINE=INNODB;

INSERT INTO `users` (`userId`,`password`,`email`) VALUES
('amccausl',MD5('password'),'alex.mccausland@gmail.com'),
('cneal',MD5('password'),'nowen@uwaterloo.ca'),
('nowen',MD5('password'),'natalielowen@gmail.com'),
('nleblanc',MD5('password'),NULL),
('h2perry',MD5('password'),NULL);

INSERT INTO `volunteers` (`name`, `userid`,`description`) VALUES 
('Chris Neal', 'cneal', 'Hello!\r\n\r\nMy name is Chris Neal but I also go by Bruce.'),
('Alex McCausland', 'amccausl', ''),
('Nicole LeBlanc', 'nleblanc', 'Program: Accounting\r\n<br>\r\nTerm: 3B\r\n<br>\r\nPlease feel free to ask me any questions and have a great term!\r\n'),
('Natalie Owen', 'nowen', NULL),
('Heather Perry', 'h2perry', 'Hi, I am in 3B actuarial science.\r\n\r\nI am always open to hearing new ideas.');

-- To determine who is holding a position in a given term
-- INSERT INTO holders VALUES ((select current_term from terms), )
CREATE TABLE `holders` (
  term		INT		NOT NULL,
  unitId	INT(8)		NOT NULL,
  position	VARCHAR(31)	NOT NULL,
  userId	CHAR(8)		NOT NULL,
  email		BOOL		NOT NULL	DEFAULT '0',

  PRIMARY KEY(unitId,term,userId,position),
  FOREIGN KEY (unitId) REFERENCES positions(unitId) ON UPDATE CASCADE,
  FOREIGN KEY (userId) REFERENCES users(userId) ON UPDATE CASCADE
)ENGINE=INNODB;

INSERT INTO holders (`term`,`unitId`,userId,position,email) VALUES
(1081,1,'amccausl','website',1);

-- Mail aliases are set outside of the user system to do forwarding and such
CREATE TABLE mail_aliases (
  domainId	INT(8)		NOT NULL	AUTO_INCREMENT,
  alias		VARCHAR(31)	NOT NULL,
  destination	VARCHAR(127)	NOT NULL,

  PRIMARY KEY( domainId,alias ),
  FOREIGN KEY (domainId) REFERENCES virtual_domains(id) ON UPDATE CASCADE
)ENGINE=INNODB;

INSERT INTO mail_aliases (`domainId`,`alias`,`destination`) VALUES
('1','president','prez@mathsoc.uwaterloo.ca');

-- Create some views to make the querying easier

-- Create an easy way to get the current term.
CREATE VIEW terms AS
SELECT CONCAT((YEAR(CURDATE()) - 1900), FLOOR((MONTH(CURDATE()) - 1) / 4) * 4 + 1) as current_term,
  CONCAT((YEAR(CURDATE() - INTERVAL 4 MONTH) - 1900), FLOOR((MONTH(CURDATE() - INTERVAL 4 MONTH) - 1) / 4) * 4 + 1) as last_term;

CREATE VIEW user_emails AS
  SELECT users.userId, users.email
  FROM users
  WHERE users.email IS NOT NULL
UNION
  SELECT users.userId, CONCAT(users.userId,'@','uwaterloo.ca')
  FROM users
  WHERE users.email IS NULL;

CREATE VIEW virtual_aliases_helper AS
-- Forward email to those who want it
  SELECT CONCAT(positions.alias,'@',virtual_domains.name) AS email, `users`.email AS destination
  FROM virtual_domains, domain_map, units, positions, users, holders, terms
  WHERE virtual_domains.id = domain_map.domainId
    AND domain_map.unitId = units.id
    AND units.id = positions.unitId
    AND units.id = holders.unitId
    AND (holders.term = terms.current_term OR holders.term IS NULL)
    AND holders.userId = users.userId
    AND holders.position = positions.alias
    AND holders.email = 1
    AND positions.email = 1
UNION
-- Make sure IMAP accounts get a copy of emails
  SELECT CONCAT(positions.alias,'@',virtual_domains.name) AS email, CONCAT(positions.alias,'@',virtual_domains.name) AS destination
  FROM virtual_domains, domain_map, units, positions
  WHERE virtual_domains.id = domain_map.domainId
    AND domain_map.unitId = units.id
    AND units.id = positions.unitId
    AND positions.imap = 1
UNION
-- Add forced aliases
  SELECT CONCAT(mail_aliases.alias,'@',virtual_domains.name) AS email, mail_aliases.destination AS destination
  FROM mail_aliases, virtual_domains
  WHERE virtual_domains.id = mail_aliases.domainId;

CREATE VIEW virtual_aliases AS
  SELECT * FROM virtual_aliases_helper
UNION
-- Add defaulting email addresses for unfilled positions
  SELECT CONCAT(positions.alias,'@',virtual_domains.name) AS email, virtual_aliases_helper.destination
  FROM positions, virtual_domains, domain_map, units, virtual_aliases_helper
  WHERE CONCAT(positions.unitId,'-',positions.alias) NOT IN (
    SELECT CONCAT(holders.unitId,'-',holders.position)
    FROM holders, terms
    WHERE holders.term = terms.current_term
      OR holders.term IS NULL)
    AND positions.email = 1
    AND positions.imap = 0
    AND virtual_domains.id = domain_map.domainId
    AND domain_map.unitId = units.id
    AND units.id = positions.unitId
    AND virtual_aliases_helper.email = CONCAT(positions.alias,'@',virtual_domains.name);

-- Store the membership of varius units on a term by term basis
CREATE VIEW members AS
  SELECT holders.term, holders.unitId, 'full' AS status, holders.userId
  FROM holders, terms
  WHERE holders.term = terms.current_term
    AND holders.position = 'full-members'
UNION
  SELECT holders.term, holders.unitId, 'partial' AS status, holders.userId
  FROM holders, terms
  WHERE holders.term = terms.current_term
    AND holders.position = 'partial-members'
UNION
  SELECT holders.term, holders.unitId, 'mail-only' AS status, holders.userId
  FROM holders, terms
  WHERE holders.term = terms.current_term
    AND holders.position = 'mail-members';

CREATE VIEW volunteers_current AS
  SELECT volunteers.userId,
	volunteers.name,
	(	SELECT GROUP_CONCAT(CONCAT_WS(':',positions.alias,positions.name) SEPARATOR ';')
		FROM positions, holders
		WHERE holders.term = terms.current_term
			AND positions.alias = holders.position
			AND holders.userId = volunteers.userId
			GROUP BY volunteers.userId
	) AS positions,
	(	SELECT DISTINCT 1
		FROM office_workers
		WHERE term = terms.current_term
			AND volunteers.userId = office_workers.userId
	) AS office_worker
  FROM volunteers, terms;
