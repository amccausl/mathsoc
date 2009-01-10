-- club is a type of unit
CREATE TABLE `clubs` (
  id        INT(8)      NOT NULL,
  phone     INT,
  room      CHAR(6),
  membership    FLOAT(4,2)  DEFAULT '0',
  description   TEXT,

  PRIMARY KEY(id),
  FOREIGN KEY (id) REFERENCES units(id) ON DELETE CASCADE
)ENGINE=INNODB;

