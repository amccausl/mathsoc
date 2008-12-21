CREATE TABLE announcement (
  id          int(11)         NOT NULL,
  title       varchar(255)    NOT NULL,
  event_date  datetime,
  content     text,

  PRIMARY KEY (id)
) type=MyISAM;
