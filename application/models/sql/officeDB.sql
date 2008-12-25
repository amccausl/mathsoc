
CREATE TABLE office_hours (
  hourId   SMALLINT  NOT NULL AUTO_INCREMENT,
  duration FLOAT     NOT NULL,
  day      SMALLINT  NOT NULL,
  start    TIME      NOT NULL,

  PRIMARY KEY (hourId)
) ENGINE=INNODB;

CREATE TABLE office_workers (
  term     SMALLINT  NOT NULL,
  userId   CHAR(8)   NOT NULL,
  hourId   SMALLINT  NOT NULL,
  added    TIMESTAMP,

  PRIMARY KEY (term, userId, hourId),
  FOREIGN KEY (hourId) REFERENCES office_hours(hourId)
    ON UPDATE CASCADE,
  FOREIGN KEY (userId) REFERENCES users(userId)
    ON DELETE CASCADE
) ENGINE=INNODB;

