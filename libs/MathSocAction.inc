<?php

require_once 'Zend/Controller/Action.php';

/**
 * MathSoc_Controller_Action is a class to be extended by all other controllers to deal with
 * initialization of some shared resources.  This is only for functions that need to be run
 * post dispatch (ie. after the request object has been created).
 */

class MathSoc_Controller_Action extends Zend_Controller_Action
{	public function init()
	{	parent::init();

		// Set the baseUrl as a variable that can be used by the view
		$this->view->baseUrl = $this->getRequest()->getBaseUrl();

		// Initialize the context-based menu
		require_once( '../application/default/views/helpers/menu.inc' );
		$this->view->menu = getMenu($this->getRequest());
	}
}
