use mathsoc;

-- DEFINE UNDERLYING STRUCTURE FOR POLICY STORAGE

-- id is an identifier for a policy
-- approved is the date this policy was approved
-- operation is the operation that created this policy and parameters (create, replace, merge, split)

CREATE TABLE policies (
	id		 SMALLINT		NOT NULL AUTO_INCREMENT,
	approved DATE			NOT NULL,
	operation VARCHAR(128),
	name	 VARCHAR(128)	NOT NULL,
	content	 TEXT			NOT NULL,

	PRIMARY KEY( id, approved )
) type=MyISAM;

CREATE TABLE policies_index (
	id		 SMALLINT		NOT NULL AUTO_INCREMENT,
	policyId SMALLINT		NOT NULL,

	PRIMARY KEY( id, policyId )
) type=MyISAM;

-- CREATE VIEW FOR EASY ACCESS TO CURRENT POLICIES

CREATE VIEW policies_current as
SELECT
	policies_index.id as num,
	policies.name as name,
	policies.approved as approved,
	policies.content as content
FROM policies, policies_index
WHERE policies_index.policyId = policies.id
	AND policies.approved IN (
	SELECT MAX(p.approved) FROM policies p WHERE p.id = policies_index.policyId GROUP BY policies_index.id)
ORDER BY policies_index.id ASC;

-- INSERT BASE POLICIES
