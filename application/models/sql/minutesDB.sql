
CREATE TABLE `minutes` (
  `id` int(11) NOT NULL auto_increment,
  `meeting_date` datetime NOT NULL default '0000-00-00 00:00:00',
  `meeting_number` int(11) NOT NULL default '0',
  `format` enum('text','html') NOT NULL default 'text',
  `term` int(11) NOT NULL default '0',
  `minutes_text` mediumtext,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM;
