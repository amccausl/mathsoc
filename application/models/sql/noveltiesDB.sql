CREATE TABLE novelties (
    id          MEDIUMINT UNSIGNED NOT NULL AUTO_INCREMENT,
    submitter   CHAR(8),
    added       TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    style       ENUM('T-Shirt','Other'),
    name        VARCHAR(31)     NOT NULL,
    description TEXT,
    notes       TEXT,
    price       FLOAT(4,2),
    
    PRIMARY KEY (id)
)ENGINE=INNODB;

CREATE TABLE novelties_images (
    id      MEDIUMINT UNSIGNED NOT NULL,
    name    VARCHAR(31) NOT NULL,
    type    VARCHAR(24) NOT NULL,
    image   BLOB        NOT NULL,

    PRIMARY KEY(id, name),
	FOREIGN KEY(id) REFERENCES novelties(id) ON UPDATE CASCADE
)ENGINE=INNODB;
