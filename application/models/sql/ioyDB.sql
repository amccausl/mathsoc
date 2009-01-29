
CREATE TABLE ioy (
  term			INT		NOT NULL,
  winner		CHAR(8)	NOT NULL,
  report		BLOB	NOT NULL,
  PRIMARY KEY (term, winner)
);

CREATE TABLE ioy_nominations (
  term			INT		NOT NULL,
  candidate		CHAR(8)	NOT NULL,
  nominator		CHAR(8)	NOT NULL,

  PRIMARY KEY (term, candidate, nominator)
);

