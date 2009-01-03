use mathsoc;

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";

-- DEFINE UNDERLYING STRUCTURE FOR POLICY STORAGE

-- id is an identifier for a bylaw
-- approved is the date this bylaw was approved
-- operation is the operation that created this bylaw and parameters (create, replace, merge, split)

CREATE TABLE IF NOT EXISTS bylaws (
	id		 SMALLINT		NOT NULL AUTO_INCREMENT,
	approved DATE			NOT NULL,
	operation VARCHAR(128),
	name	 VARCHAR(128)	NOT NULL,
	content	 TEXT			NOT NULL,

	PRIMARY KEY( id, approved )
) type=MyISAM AUTO_INCREMENT=37;

CREATE TABLE bylaws_index (
	id		 SMALLINT		NOT NULL AUTO_INCREMENT,
	bylawId SMALLINT		NOT NULL,

	PRIMARY KEY( id )
) type=MyISAM AUTO_INCREMENT=33;

-- CREATE VIEW FOR EASY ACCESS TO CURRENT POLICIES

CREATE VIEW bylaws_current as
SELECT
	bylaws_index.id as num,
	bylaws.name as name,
	bylaws.approved as approved,
	bylaws.content as content
FROM bylaws, bylaws_index
WHERE bylaws_index.bylawId = bylaws.id
	AND bylaws.approved IN (
	SELECT MAX(p.approved) FROM bylaws p WHERE p.id = bylaws_index.bylawId GROUP BY bylaws_index.id)
ORDER BY bylaws_index.id ASC;

