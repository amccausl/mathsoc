<?php

// Todo: load below from config
// Todo: move load config and load template to separate files

error_reporting(E_ALL|E_STRICT);
ini_set('display_errors', 0);
ini_set('display_errors', 1);
date_default_timezone_set('America/Toronto');

// directory setup and class loading
set_include_path('.' . PATH_SEPARATOR . '../libs/'
     . PATH_SEPARATOR . '../application/models/'
     . PATH_SEPARATOR . '../application/default/views/helpers/'
     . PATH_SEPARATOR . get_include_path());

include "Zend/Loader.php";
Zend_Loader::registerAutoload();

// load configuration
$config = new Zend_Config_Ini('../config/main.ini', 'general');
Zend_Registry::set('config', $config);

// Add database connection
$db = Zend_Db::factory($config->db);
Zend_Db_Table::setDefaultAdapter($db);

// Todo: load access control
Zend_Loader::loadClass('Zend_Auth');

$logger = new Zend_Log();
$writer = new Zend_Log_Writer_Firebug();
$logger->addWriter($writer);
Zend_Registry::set('logger',$logger);

// setup controller
$frontController = Zend_Controller_Front::getInstance();
//$frontController->setBaseUrl("/zend/html");
$frontController->throwExceptions(true);
$frontController->addModuleDirectory('../application/');

// Create the view and load in the ViewRenderer
include_once( '../application/default/views/helpers/initialize.inc' );

// Add required routers
$router = $frontController->getRouter();

// Add routers for the ajax calls in the exambank
$router->addRoute('courses1',
	new Zend_Controller_Router_Route('exambank/courses/:prefix',
		array('module' => 'default', 'controller' => 'exambank', 'action' => 'courses')));
$router->addRoute('courses2',
	new Zend_Controller_Router_Route('exambank/courses/:prefix/:number',
		array('module' => 'default', 'controller' => 'exambank', 'action' => 'courses')));
$router->addRoute('exams',
	new Zend_Controller_Router_Route('exambank/exams/:prefix/:number',
		array('module' => 'default', 'controller' => 'exambank', 'action' => 'exams')));
$router->addRoute('exams1',
	new Zend_Controller_Router_Route('exambank/exams/:prefix/:number/:term/:id/:type/',
		array('module' => 'default', 'controller' => 'exambank', 'action' => 'exams')));
// Add routers for the ajax calls in the exambank
$router->addRoute('council',
	new Zend_Controller_Router_Route('council/minutes/:id',
		array('module' => 'council', 'controller' => 'minutes', 'action' => 'display')));
$router->addRoute('council-policies',
	new Zend_Controller_Router_Route('council/policies/:page',
		array('module' => 'council', 'controller' => 'policies', 'action' => 'display')));
$router->addRoute('council-policies1',
	new Zend_Controller_Router_Route('council/policies/diff/:page/:version1/:version2',
		array('module' => 'council', 'controller' => 'policies', 'action' => 'diff')));
$router->addRoute('council-bylaws',
	new Zend_Controller_Router_Route('council/bylaws/:page',
		array('module' => 'council', 'controller' => 'bylaws', 'action' => 'display')));

// run!
$frontController->dispatch();

