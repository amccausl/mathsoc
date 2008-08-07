<?php

// Todo: load below from config
// Todo: move load config and load template to separate files
error_reporting(E_ALL|E_STRICT);
ini_set('display_errors', 1);
date_default_timezone_set('America/Toronto');

// directory setup and class loading
set_include_path('.' . PATH_SEPARATOR . '../libs/'
     . PATH_SEPARATOR . '../application/models'
     . PATH_SEPARATOR . get_include_path());
include "Zend/Loader.php";
Zend_Loader::registerAutoload();

// load configuration
$config = new Zend_Config_Ini('../config/main.ini', 'general');
$registry = Zend_Registry::getInstance();
$registry->set('config', $config);

// Todo: load access control

// load Smarty templating system
include 'smarty/Smarty.class.php';
require_once('local/SmartyView.php');

//Create the view and set the compile dir to template_c
$view = new SmartyView(array(
                'compileDir' => '../data/compiled'
                ));
$view->title = 'The Mathematics Society of the University of Waterloo';
$view->stylesheets = array('/css/main.css');

// Todo: add menu settings
require_once( '../application/views/helpers/menu.inc' );
$view->menu = $menu;

//Create a new ViewRenderer helper and assign our newly
//created SmartyView object as the view instance
$viewHelper = new Zend_Controller_Action_Helper_ViewRenderer($view);
$viewHelper->setViewSuffix('tpl');

//Save the helper to the HelperBroker
Zend_Controller_Action_HelperBroker::addHelper($viewHelper);

// Todo: add logging information to track users for use-case analysis

// setup controller
$frontController = Zend_Controller_Front::getInstance();
$frontController->throwExceptions(true);
$frontController->setControllerDirectory( array(
		"default" => '../application/controllers',
		"exambank" => '../application/controllers/exambank') );
//$frontController->setParam('noViewRenderer', true);
// run!
$frontController->dispatch();

