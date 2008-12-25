CREATE TABLE announcement (
  id          int(11)         NOT NULL,
  title       varchar(255)    NOT NULL,
  event_date  datetime,
  content     text,

  PRIMARY KEY (id)
)ENGINE=INNODB;

CREATE TABLE `events` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `UniqueId` enum('userid','watcard') default 'userid',
  `UserInfo` text,
  `Name` varchar(64) NOT NULL default '',
  `Description` text,
  PRIMARY KEY  (`id`)
)ENGINE=INNODB AUTO_INCREMENT=3 ;

INSERT INTO `events` (`id`, `UniqueId`, `UserInfo`, `Name`, `Description`) VALUES 
(1, 'watcard', 'Name:=text|Student Id:=id|Faculty:=select(Applied Health Sciences,Arts,Engineering,Environmnental Studies,[Mathematics],Science,Multiple faculties)', 'Retro Dance Party', '<p>Retro Dance Party will be at Fed Hall from 9PM to 1AM on March 25th (Tuesday).</p>\r\n<p>Since you''re signing up early, it''s free!  If you had waited to get your tickets at the door, it would have cost you $2.  Tell your friends!  Plus, you can <a href="http://www.phatalbert.ca/retrodanceparty.html">request songs to be played</a>.</p>\r\n<p>Be sure to bring your Watcard.</p>');

CREATE TABLE `events_signup` (
  `Event` int(10) unsigned NOT NULL default '0',
  `UniqueId` varchar(8) NOT NULL default '',
  `UserInfo` varchar(255) default NULL,
  PRIMARY KEY  (`Event`,`UniqueId`),
  FOREIGN KEY (Event) REFERENCES events(id)
)ENGINE=INNODB;

INSERT INTO `events_signup` (`Event`, `UniqueId`, `UserInfo`) VALUES 
(1, '12345678', 'name:Test User;id:12345678;faculty:Engineering'),
(1, '66666662', 'name:Master Tarfal, Underlord of Pain;id:66666662;faculty:Mathematics'),
(1, '20138973', 'name:Natalie Owen;id:20138973;faculty:Mathematics'),
(1, '20216513', 'name:Nav Chandrani;id:20216513;faculty:Arts'),
(1, '20200010', 'name:Sageevshantha;id:20200010;faculty:Mathematics'),
(1, '20279441', 'name:Katherine Wong;id:20279441;faculty:Environmnental Studies'),
(1, '20274686', 'name:Meaghan Mendonca;id:20274686;faculty:Environmnental Studies'),
(1, '20276188', 'name:Donavyn Mendonca;id:20276188;faculty:Science'),
(1, '20283098', 'name:Karyn Menezes;id:20283098;faculty:Arts'),
(1, '20170523', 'name:Alastair Lahtinen;id:20170523;faculty:Mathematics'),
(1, '20268263', 'name:Elaine Tran;id:20268263;faculty:Environmnental Studies'),
(1, '20209461', 'name:Alison Lang;id:20209461;faculty:Arts'),
(1, '20224569', 'name:Gagandeep Pabla;id:20224569;faculty:Mathematics'),
(1, '20127004', 'name:Hussein Hirjee;id:20127004;faculty:Mathematics'),
(1, '20164053', 'name:abid;id:20164053;faculty:Arts'),
(1, '20266511', 'name:Emily Fitzpatrick;faculty:Mathematics'),
(1, '20268230', 'name:Laura Stewart;faculty:Arts'),
(1, '20142432', 'name:Aly Sivji;faculty:Mathematics'),
(1, '20259671', 'name:Peter Barfuss;faculty:Mathematics'),
(1, '20128898', 'name:Laura Bradbury;faculty:Mathematics'),
(1, '20235792', 'name:Ashley Dean;faculty:Arts'),
(1, '20146451', 'name:Andrew Fransen;faculty:Mathematics'),
(1, '20288154', 'name:Abhishek;faculty:Mathematics'),
(1, '20287269', 'name:Shu Lee;faculty:Mathematics'),
(1, '20126742', 'name:Marcus Shea;faculty:Mathematics'),
(1, '20243492', 'name:Simon Cai;faculty:Arts'),
(1, '20195054', 'name:paul Nogas;faculty:Engineering'),
(1, '20165080', 'name:Michael Klein;faculty:Arts'),
(1, '20228459', 'name:aporzucz;faculty:Arts'),
(1, '20178317', 'name:Valerie Fraser;faculty:Mathematics'),
(1, '20268373', 'name:Janet Su;faculty:Arts'),
(1, '20267797', 'name:Annie Tang;faculty:Science'),
(1, '20144121', 'name:Kuo Wei Lee;faculty:Mathematics'),
(1, '20183797', 'name:Yu-Ling Chang;faculty:Mathematics'),
(1, '20272503', 'name:Natasha Moes;faculty:Science'),
(1, '20265795', 'name:Kelly Wills;faculty:Science'),
(1, '20265949', 'name:Rebecca Jenish;faculty:Science'),
(1, '20282435', 'name:Rebecca Steiner;faculty:Arts'),
(1, '20228565', 'name:Hannah Michielsen;faculty:Arts'),
(1, '20280848', 'name:Sean Errey;faculty:Arts'),
(1, '20228137', 'name:Aadan Rigby;faculty:Science'),
(1, '20265585', 'name:AJ Phinney;faculty:Arts'),
(1, '20272038', 'name:Terry Reid;faculty:Arts'),
(1, '20278095', 'name:Michelle Van Rassel;faculty:Arts'),
(1, '20239350', 'name:Kirk;faculty:Engineering'),
(1, '20223183', 'name:Dave M;faculty:Arts'),
(1, '20195997', 'name:Kate Wilson;faculty:Arts');

