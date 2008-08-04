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

// load access control
//include('');

// load Smarty templating system
include 'smarty/Smarty.class.php';
$smarty = new Smarty();
$smarty->debugging = false;
$smarty->force_compile = true;
$smarty->caching = false;
$smarty->compile_check = true;
$smarty->cache_lifetime = -1;
$smarty->template_dir = '../application/views/scripts';
$smarty->compile_dir = '../data/cache';
$smarty->plugins_dir = array(
  SMARTY_DIR . 'plugins',
  '../application/views/helpers');
$registry->set('smarty', $smarty);

// setup controller
$frontController = Zend_Controller_Front::getInstance();
$frontController->throwExceptions(true);
$frontController->setControllerDirectory('../application/controllers');
$frontController->setParam('noViewRenderer', true);
// run!
$frontController->dispatch();

