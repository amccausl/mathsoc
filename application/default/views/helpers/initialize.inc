<?php

/**
 * Initialize the view with some defaults from the configuration
 */

$registry = Zend_Registry::getInstance();
$config = $registry->get('config', $config);

// load Smarty templating system
require_once('smarty/Smarty.class.php');
require_once('SmartyView.php');

$view = new SmartyView( $config->smarty );
$view->title = 'The Mathematics Society of the University of Waterloo';
$view->stylesheets = array(Zend_Controller_Front::getInstance()->getBaseUrl() . '/css/main.css');

// Add the menu to the layout
//require_once( '../application/views/helpers/menu.inc' );
include_once( 'menu.inc' );
$view->menu = $menu;

//Create a new ViewRenderer helper and assign our newly
//created SmartyView object as the view instance
$viewHelper = new Zend_Controller_Action_Helper_ViewRenderer($view);
$viewHelper->setViewSuffix($config->smarty->suffix);

//Save the helper to the HelperBroker
Zend_Controller_Action_HelperBroker::addHelper($viewHelper);
