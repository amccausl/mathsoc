CREATE TABLE exams (
    id              MEDIUMINT UNSIGNED	NOT NULL	AUTO_INCREMENT,
    courseId        SMALLINT UNSIGNED	NOT NULL,
    term            SMALLINT UNSIGNED	NOT NULL,
    uploader        char(8),
    type            ENUM('Assignment', 'Final', 'Midterm', 'Practice', 'Quiz', 'Test') NOT NULL,
    number          TINYINT UNSIGNED,
    practice        TINYINT(1),
    exam_path       varchar(128),
    exam_type       varchar(24),
    solutions_path  varchar(128),
    solutions_type  varchar(24),
    on_campus       bool		DEFAULT 0,
    in_math         bool		DEFAULT 0,
    visible         bool		DEFAULT 1,
    approved        bool		DEFAULT 0,
    added           TIMESTAMP   DEFAULT CURRENT_TIMESTAMP,
    
    PRIMARY KEY (id),
    FOREIGN KEY (courseId) REFERENCES courses(id)
)ENGINE=INNODB;

CREATE TABLE courses (
    id		SMALLINT UNSIGNED	NOT NULL	AUTO_INCREMENT,
    prefix	char(6)				NOT NULL,
    code	char(4)				NOT NULL,
    title	varchar(255)		NOT NULL,

    PRIMARY KEY (id,prefix,code)
)ENGINE=INNODB;

CREATE VIEW exams_view as
SELECT
	e.id,
	CONCAT(c.prefix, ' ', c.code) as course,
	c.prefix,
	c.code,
	e.term,
	e.type,
	e.exam_path,
	e.solutions_path,
	e.approved
FROM exams e, courses c
WHERE c.id = e.courseId
	AND e.visible = 1
ORDER BY e.term DESC;
