use mathsoc;

-- DEFINE UNDERLYING STRUCTURE FOR REFUNDS STORAGE (to be used by userDB)

-- userId is the 8 character uwid
-- approved is the date this policy was approved
-- operation is the operation that created this policy and parameters (create, replace, merge, split)

CREATE TABLE `refunds` (
  userId	CHAR(8)		NOT NULL,
  term		INT(8)		NOT NULL,

  status	ENUM('REQUESTED', 'RECEIVED', 'REGULAR', 'WITHDRAWN', 'REJECTED')	DEFAULT 'RECEIVED',
  reason	TEXT,

  PRIMARY KEY (term, userId)
)ENGINE=INNODB;

CREATE TABLE `access_logging` (
  time		DATETIME	NOT NULL,
  userId	CHAR(8)		NOT NULL,
  system	VARCHAR(256) NOT NULL,

  PRIMARY KEY (userId,time,system)
)ENGINE=INNODB;

CREATE VIEW `refunds_users` AS
SELECT userId, term
FROM refunds, terms
WHERE
  (term = terms.current_term AND (status = 'REQUESTED' OR status = 'RECEIVED'))
OR
  (term = terms.last_term AND status = 'RECEIVED');

CREATE VIEW `refunds_newusers` AS
SELECT term, userId
FROM refunds r
WHERE userId NOT IN (SELECT DISTINCT userId FROM refunds r2 WHERE r2.term < r.term);

CREATE VIEW `refunds_trends` AS
SELECT DISTINCT
  r.term,
  (SELECT COUNT( userId ) FROM refunds r2 WHERE r.term = r2.term GROUP BY r2.term ORDER BY r2.term DESC) AS requested,
  (SELECT COUNT( userId ) FROM refunds_newusers rn WHERE r.term = rn.term GROUP BY rn.term ORDER BY rn.term DESC) AS new,
  (SELECT COUNT( userId ) FROM refunds r2 WHERE r.term = r2.term AND status = 'WITHDRAWN' GROUP BY r2.term ORDER BY r2.term DESC) AS withdrawn,
  (SELECT COUNT( userId ) FROM refunds r2 WHERE r.term = r2.term AND status = 'REJECTED' GROUP BY r2.term ORDER BY r2.term DESC) AS rejected
FROM refunds r
ORDER BY term DESC;

CREATE TRIGGER access_logging_ai
AFTER INSERT ON access_logging
FOR EACH ROW
  UPDATE LOW_PRIORITY refunds
  SET status = 'REJECTED', reason = NEW.system
  WHERE refunds.userId = NEW.userId
	AND refunds.term = CONCAT((YEAR(NEW.time) - 1900), FLOOR((MONTH(NEW.time) - 1) / 4) * 4 + 1)
	AND refunds.status = 'REQUESTED';

