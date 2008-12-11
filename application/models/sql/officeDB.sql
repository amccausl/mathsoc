use user_management;

CREATE TABLE office_hours (
  hourId   SMALLINT  NOT NULL AUTO_INCREMENT,
  duration FLOAT     NOT NULL,
  day      SMALLINT  NOT NULL,
  start    TIME      NOT NULL,

  PRIMARY KEY (hourId)
) type=MyISAM;

CREATE TABLE office_workers (
  term     SMALLINT  NOT NULL,
  userId   int(11)   NOT NULL,
  hourId   SMALLINT  NOT NULL,
  added    TIMESTAMP,

  PRIMARY KEY (term, userId, hourId)
) type=MyISAM;

