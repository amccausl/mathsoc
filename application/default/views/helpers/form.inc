<?php
	require_once( "smarty/SmartyValidate.class.php" );
	$front = Zend_Controller_Front::getInstance();

	// Add the css to render forms
	$front->view->array_push( 'stylesheets', $front->getRequest()->getBaseUrl() . '/css/form.css' );
