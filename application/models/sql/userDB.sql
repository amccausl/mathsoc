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

-- Clubs will have a few special positions to list members
-- default_email is the address that emails to this account should go to if no one currently holds the position
-- admin is 1 if this position has admin access for it's unit
-- email is 1 if this position should have an email address. 
-- imap is 1 if this position should have an IMAP email account.
-- shell is 1 if this position should have shell access to pink-tie.
CREATE TABLE `positions` (
  unitId	INT(8)		NOT NULL,
  alias		VARCHAR(31)	NOT NULL,
  name		VARCHAR(255)	NOT NULL,
  category	enum('EXC','OEX','DIR','APP','REP','AFF','LIST'),
  description	text,
  default_email	VARCHAR(127),
  desired	INT(8),
  admin		BOOL		NOT NULL	DEFAULT '0',
  email		BOOL		NOT NULL	DEFAULT '0',
  imap		BOOL		NOT NULL	DEFAULT '0',
  shell		BOOL		NOT NULL	DEFAULT '0',
  chosen	ENUM('selected','elected','appointed'),
  duration	ENUM('term', 'year')	DEFAULT 'term',

  PRIMARY KEY (`unitId`,`alias`),
  FOREIGN KEY (unitId) REFERENCES units(id) ON DELETE CASCADE
)ENGINE=INNODB;

INSERT INTO `positions` (`unitId`,`name`,`alias`,`category`,`description`) VALUES
(1, 'President', 'prez', 'EXC', '<p>The President of the Math Society is the CEO of the Society and an <i>ex-officio</i> member of all committees and Boards of the Society.  The Presidents duties include:</p><ul><li>to preside over General Meetings of the Society</li><li>to provide for Society representation at official functions and on public occasions</li><li>to be responsible for Society public relations</li><li>to know and interpret the bylaws of the Society</li><li>to appoint members to act as representatives of the Society on committees external thereto</li></ul>'),
(1, 'Vice President, Activities and Services', 'vpas', 'EXC', 'The VPAS oversees the day-to-day activities of the Society.  They are the elected representative responsible for the activies and services provided by the Society.  The directors under the VPAS are the people who actually make it happen.  The directors the VPAS is responsible for are:<ul><li>Charity Ball</li><li>Computing</li><li>Mathletics</li><li>Movies</li><li>Novelites</li><li>Office</li><li>Postings</li><li>Publicity</li><li>Social</li><li>Website</li></ul>'),
(1, 'Computing Director', 'computing', 'DIR', 'The Computing Director is tasked with providing a useful computing environment for MathSoc council and members (read: everyone!).  Whether it''s maintaining servers or beating up fried processors, it''s Computing''s job.'),
(1, 'Resources Director', 'resources', 'DIR', '<P>The resources director is responsible for MathSoc''s exambank.  This includes obtaining exams from professors and students, maintaining and updating the hardcopy booklets and typesetting exams for the <a href="http://www.mathsoc.uwaterloo.ca/exambank">Online Exambank</a>.\r\n\r\n<P>Volunteers are always needed to submit old exams and type exams.  If you are interested in helping, please e-mail <a href="mailto:resources@mathsoc.uwaterloo.ca">resources@mathsoc.uwaterloo.ca</a>.\r\n</p>\r\n<p>\r\nIf you are interested in this position, please email vpa@mathsoc.uwaterloo.ca. The exam bank is one of the most important services MathSoc provides, so consider applying!\r\n</p>'),
(1, 'Vice President, Finances', 'vpf', 'EXC', 'The vice-president, financial, is responsible for keeping records, presenting a budget, preparing financial reports and reports to the president, among other things.'),
(1, 'Movies Director', 'movies', 'DIR', '<p>Two movies every Thursday night at 7pm for FREE!  Usually in MC 2066 (look for posters and on the whiteboard to confirm).  We show blockbuster after blockbuster!</p>\r\n<p>If you feel our movies aren''t up to scratch, or have any movies you want to see, send us an email (movies@mathsoc.uwaterloo.ca)!</p>'),
(1, 'Office Manager', 'office', 'DIR', 'The duties of the Office Manager are pretty self explanatory, they are to maintain the MathSoc Office (MC 3038).\r\n<br><br>\r\nIf you have any suggestions on things that would improve the MathSoc office, write an e-mail to <a href = office@mathsoc.uwaterloo.ca>office@mathsoc.uwaterloo.ca</a>.<br>'),
(1, 'Publicity Director', 'publicity', 'DIR', '<p>Well, the duties are to let people know what''s going on, advertise events, etc. So naturally, this would be the person you bug if you want posters for your event. Just email publicity@mathsoc.uwaterloo.ca for all your poster-making needs!\r\n</p>\r\n'),
(1, 'Postings Director', 'postings', 'DIR', 'The Postings Director is responsible for timely posting of approved bills and enforcement of rules against illegal bill postings.\r\n<br>\r\nThe rules for postings are:<br>\r\n-No more than 2 posters if bigger than 8.5 x 11 inches<br>\r\n-No more than 2 posters if commercial in nature (ie, you are endeavouring to sell something)<br>\r\n-No more than 6 posters otherwise<br>\r\n-Every poster must have contact information in the form of an e-mail, telephone number, or Feds/MathSoc recognised club, etc.<br>\r\n-If a poster contains a language other than English, an English translation must be present on the same poster'),
(1, 'Internal Director', 'internal', 'DIR', 'The Internal Director is responsible for many tasks:\r\n<ul>\r\n<li>Recommending amendments to the Society''s Bylaws and Policies to Council </li>\r\n<li>Reporting violations of the Bylaws to Council</li>\r\n<li>Acting as a resource person on all matters regarding the interpretation of the Bylaws</li>\r\n<li>Acting as a Liason between the Society and its clubs, charged with the following tasks:\r\n<ul>\r\n<li>Keeping a record of all minutes submitted by clubs</li>\r\n<li>receiving and reviewing constitutions and constitutional ammendments of clubs recognized or seeking to be recognized by the society.</li>\r\n</ul>\r\n</li>\r\n</ul>\r\n<p>\r\nIf you are interested in this position, please email prez@mathsoc.uwaterloo.ca. Relations with clubs are very important to MathSoc.\r\n</p>'),
(1, 'External Director', 'external', 'DIR', '<p>The External Director chairs the External Board of Mathsoc.  (Often consisting of only the External Director)</p>\r\n\r\n<p>The External Board:\r\n<br />Acts as a non-academic liaison between the Society and the Federation of Students and the other student societies<br />\r\nensures that the Society is suitably represented on any committees where such representation is available.\r\n</p>\r\n\r\n<p>The External Director is appointed by, and responsible to, the President.</p>'),
(1, 'Chief Returning Officer', 'cro', 'APP', '<p>CRO stands for Chief Returning Officer, and this is the person in charge of elections.</p>\r\n\r\n<p>The CRO works with other members of the Election committee to publicize elections, verify nominees, run polling stations and most importantly to ensure that elections are fair and problem-free.</p><br>'),
(1, 'First Year Representative', 'firstyear', 'REP', 'The Duties of the First Year Class Representative are to be a voice for all first year members on the MathSoc Council, and make sure that first year students'' interests are being met. First Year Representatives are also responsible for keeping their constituents informed as to recent events occurring in the math faculty and the university itself.'),
(1, 'Canada Day Coordinator', 'canadaday', 'DIR', 'MathSoc runs the Kids Fun Fest at UW''s Canada Day celebrations every July 1st. It''s a bunch of fun activities for small children, and we can always use more help. \r\n\r\nIf you want to help out, please sign up on the office door!!'),
(1, 'Speaker', 'speaker', 'APP', '<p>The Speaker of the Mathematics Society Council is charged with running the meetings of Council according to Robert''s Rules of Orders and the Bylaws of the Society.</p>'),
(1, 'Teaching Option Representative', 'teachreps', 'REP', 'Represent students in the Math Teaching Option on MathSoc Council.'),
(1, 'Vice President, Academic', 'vpa', 'EXC', 'The VPA is tasked to represent student views and opinions on academic issues in various faculty and school councils. The VPA is also on various committees to review and amend academic plan changes. Additionally, they can provide academic rights counselling and organize forums for the discussion of issues of interest to students.'),
(1, 'Mathletics Director', 'mathletics', 'DIR', 'Each term, MathSoc subsidizes the entry fee for teams of Math students participating in Campus Rec activities. The Director is responsible for organizing these teams, and then making sure that they follow the rules (register on time, show up to games, etc). As well, the director is expected to run various other athletic events throughout the year.'),
(1, 'Novelties Director', 'novelties', 'DIR', '<p>The novelties director is in charge of the ordering, inventory and creation of novelties for sale from MathSoc in the office.  The director is always interested in hearing ideas for new math novelties so be sure to send any ideas you have to novelties@mathsoc.\r\n</p>'),
(1, 'Social Director', 'social', 'DIR', 'The Social director is responsible for planning, organizing, and implementing social events for all math students to enjoy. <br>'),
(1, 'Computer Science Representative', 'csreps', 'REP', 'The Computer Science representatives attend MathSoc council meetings and School of Computer Science Committee meetings, and speak on behalf of Computer Science majors. They are available for advice and will be happy to address any issues that their constituents may have.'),
(1, 'Operations Research Representative', 'orreps', 'REP', NULL),
(1, 'Statistics Representative', 'statsreps', 'REP', NULL),
(1, 'First Year Affairs Director', 'fya', 'DIR', '<p>The CFYA Director coordinates, mentors, and provides frosh with focal point to express their concerns to the Math Society. The CFYA director is also responsible for planning, organizing, and implementing social and academic events for first year students.\r\n</p><p>\r\nIf you are interested in this position, please email prez@mathsoc.uwaterloo.ca. Working with the first year reps is a great opportunity.\r\n</p>'),
(1, 'Combinatorics & Optimization Representative', 'coreps', 'REP', ''),
(1, 'Pure Math Representative', 'pmreps', 'REP', ''),
(1, 'President (Off-Stream)', 'off-prez', 'EXC', '<p>The Off-Stream President is the Chief Executive Officer of the Society in the next term.</p>'),
(1, 'Secretary', 'secretary', 'APP', 'The secretary is responsible for taking minutes at council meetings, sending the minutes out to Council to approve, and for posting the minutes on the webpage, as well as being the person in charge of maintaining the Council list on the website.'),
(1, 'Software Engineering Representative', 'sereps', 'REP', 'I represent the interests of Software Engineering students on council. If you are in SE and have questions/concerns about where your Math Society fee is going, I am the one to talk to.'),
(1, 'Charity Ball Director', 'charity', 'DIR', 'The Charity Ball Director organizes one of the most fun events of the\r\nfall term.  A semi-formal event with a banquet dinner and dance, as\r\nwell as live and silent auctions with the proceeds going to a\r\ndesignated charity, it''s fun for the whole group.  \r\n'),
(1, 'Website Director', 'website', 'DIR', 'The website director is responsible for maintaining, troubleshooting, and updating the Society''s website. <br>\r\n<br>\r\nPlease direct any questions or concerns to <i>website at mathsoc dot uwaterloo.ca</i>. Also feel free to e-mail any suggestions or wishes that you have for the website.<br>\r\n'),
(1, 'Feds Math Councillor', 'fedsmath', 'AFF', '<p>Math Councillors are responsible for representing undergrads to the <a href="http://www.feds.ca/">Federation of Students</a>, and thereby to the administration of the University of Waterloo. Most responsibilites fall under the auspices of the many committees of which we can be members, but they also include attending monthly council meetings on Sundays, and in the case of co-op councillors, Co-op council meetings. Our job, in many ways, is to advise and critique the executive in the execution of their duties as servants of the full time undergraduate population. </p>\r\n<p>The motto of the Feds is "To empower, serve and represent full time undergraduates at the university of Waterloo."  It is our job to do this on behalf of Math students.</p><br>'),
(1, 'Math Senator', 'senator', 'AFF', '<p>Senate is the highest legislative academic body at the University of Waterloo. From curriculum decisions to updates on housing in the community to setting budgetary and enrollment targets, Senate does it all.  There are 9 undergraduate students on Senate: one from each faculty, the President of the FedS and two undergrads at-large.</p>\r\n\r\n<p>If you would like any more information please don''t hesitate to email me.</p>'),
(1, 'Math/Accounting Representative', 'accreps', 'REP', 'Being a class rep means that I am a voting member of the council and if there are any concerns in either program I represent, I can bring it to attention of MathSoc.  So let me know if there are any issues you want discussed at MathSoc and I''ll do my best!'),
(1, 'Applied Math Representative', 'amreps', 'REP', NULL),
(1, 'Offically Undeclared Representative', 'undeclreps', 'REP', 'The officially undeclared representaive is responsible to the students in the society who have currently not selected a major for their program.  The representaive is responsible for representing the interests of these students on council.'),
(1, 'Double Degree Business Admin & Math Representative', 'badminreps', 'REP', 'Represent All Business Admin & Math Double Degree students.'),
(1, 'Actuarial Science Representative', 'actscireps', 'REP', 'Represent students in the Actuarial Science program on MathSoc Council.'),
(1, 'Software Engineering Orientation Director', 'seorientation', 'AFF', '<p>The Software Engineering Orientation Director is responsible for everything and anything related to the Software Engineering Orientation week. Since Software Engineering is part of both the Math and Engineering Faculties, this director gets a unique experience planning the week. Selected in October and going well into September they spend 11 months planning the best week of the year with the help of the Math and Engineering Orientation directors.</p>\r\n\r\n<p>Contact the Software Engineering Orientation Director at <A HREF="mailto:se-orientation@softeng.uwaterloo.ca">se-orientation@softeng.uwaterloo.ca</A>.</p>'),
(1, 'Vice-President, Academic (Off-Stream)', 'off-vpa', 'EXC', '<p>As off-stream VPA, I hope that nothing bad happens to council, as then I have to do something. It''s like being the 20th person in line to the throne, if the throne was being on-stream VPA. </p>'),
(1, 'Vice-President, Finances (Off-Stream)', 'off-vpf', 'EXC', 'The Off-Stream Vice-President, Finances position is held by the elected person who will be the Vice-President, Finances in the next term.'),
(1, 'Vice-President, Activities and Services (Off-Stream)', 'off-vpas', 'EXC', 'The Off-Stream VPAS is the person who is currently elected VPAS in the following term.'),
(1, 'Math/Business Representative', 'busreps', 'REP', NULL),
(1, 'Mathematical Sciences Representative', 'mathscireps', 'REP', 'My goal is to vote wisely :)'),
(1, 'Mathematics General', 'genreps', 'REP', NULL),
(1, 'Computational Math Representative', 'cmreps', 'REP', 'The Computational Mathematics representative attends MathSoc council meetings and speaks on behalf of CM students.'),
(1, 'Games Director', 'games', 'DIR', NULL),
(1, 'Internal Financial Reviewer', 'reviewer', 'APP', 'Ensure the accountability of the VPF by independently reviewing the finances periodically throughout the term, and reporting to Council.');

-- Contain information about who a position defaults to
CREATE TABLE `positions_rules` (
  ruler_unit      INT(8)      NOT NULL,
  ruler_position  VARCHAR(31) NOT NULL,
  ruled_unit      INT(8)      NOT NULL,
  ruled_position  VARCHAR(31) NOT NULL,

  PRIMARY KEY (ruler_unit, ruler_position, ruled_unit, ruled_position),
  FOREIGN KEY (ruler_unit) REFERENCES units(id) ON UPDATE CASCADE,
  FOREIGN KEY (ruler_position) REFERENCES postions(alias) ON UPDATE CASCADE,
  FOREIGN KEY (ruled_unit) REFERENCES units(id) ON UPDATE CASCADE,
  FOREIGN KEY (ruled_position) REFERENCES postions(alias) ON UPDATE CASCADE
)ENGINE=INNODB;

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

-- The questions to ask a user who is applying
CREATE TABLE questions (
  `id`			INT(8)		NOT NULL	AUTO_INCREMENT,
  `key`			VARCHAR(31)	NOT NULL,
  `text`		TEXT		NOT NULL,
  `type`		ENUM('text')	NOT NULL,
  `type_params` TEXT,
  `default`		TEXT,
  PRIMARY KEY( id )
)ENGINE=INNODB;

-- map positions to the questions asked to those applying
CREATE TABLE positions_questions (
  position	VARCHAR(31)	NOT NULL,
  question	INT(8)		NOT NULL,

  PRIMARY KEY (position, question ),
  FOREIGN KEY (position) REFERENCES positions(alias) ON UPDATE CASCADE,
  FOREIGN KEY (question) REFERENCES questions(id) ON UPDATE CASCADE
)ENGINE=INNODB;

CREATE TABLE positions_applications (
  id		INT(8)		NOT NULL	AUTO_INCREMENT,
  term		INT			NOT NULL,
  unit		INT(8)		NOT NULL,
  position	VARCHAR(31)	NOT NULL,
  user		CHAR(8)		NOT NULL,
  questions	TEXT,
  applied	TIMESTAMP	DEFAULT CURRENT_TIMESTAMP,

  PRIMARY KEY( id ),
  FOREIGN KEY (unit) REFERENCES units(id) ON UPDATE CASCASE,
  FOREIGN KEY (position) REFERENCES positions(alias) ON UPDATE CASCADE,
  FOREIGN KEY (user) REFERENCES users(userId) ON UPDATE CASCADE
)ENGINE=INNODB;

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
