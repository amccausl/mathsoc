<?php
require_once 'Zend/Acl.php';
require_once 'Zend/Acl/Role.php';

// Create roles for everyone attending the site

$acl = new Zend_Acl();

$acl->addRole(new Zend_Acl_Role('student')
	->addRole(new Zend_Acl_Role('math student')
	->addRole(new Zend_Acl_Role('volunteer', 'student')
	->addTole(new Zend_Acl_Role('director', 'volunteer')
	->addRole(new Zend_Acl_Role('exec', 'director');

// Administrator does not inherit access controls
$acl->addRole(new Zend_Acl_Role('administrator'));

