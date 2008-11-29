
CREATE TABLE `lockers` (
    `lockerId`       INT(10) NOT NULL,
    `current_userId` CHAR(8),
    `current_phone`  CHAR(12),
    `last_userId`    CHAR(8),
    `last_phone`     CHAR(12),
    `starts`         DATETIME,
    `expires`        DATETIME,
    `combo`          VARCHAR(10),
    PRIMARY KEY (`lockerId`)
);

