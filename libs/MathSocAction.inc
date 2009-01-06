<?php

require_once 'Zend/Controller/Action.php';
require_once 'KiwiId.php';

/**
 * MathSoc_Controller_Action is a class to be extended by all other controllers to deal with
 * initialization of some shared resources.  This is only for functions that need to be run
 * post dispatch (ie. after the request object has been created).
 */

class MathSoc_Controller_Action extends Zend_Controller_Action
{
	private $loginUrl = "https://strobe.uwaterloo.ca/cpadev/kiwi/user/login/";

	public function init()
	{	parent::init();

		// Set the baseUrl as a variable that can be used by the view
		$this->view->baseUrl = $this->getRequest()->getBaseUrl();

		// Initialize the context-based menu
		require_once( '../application/default/views/helpers/menu.inc' );
		$this->view->menu = getMenu($this->getRequest());
	}

	protected function secure()
	{
		$referer = "http://{$_SERVER['HTTP_HOST']}{$_SERVER['REQUEST_URI']}";

		// If the user is returning from Kiwi, authenticate them
		$status = "";
		$auth = Zend_Auth::getInstance();

		if( !$auth->hasIdentity() )
		{	// If user not authenticated and query not set
			if( !$this->_request->isGet() || !$this->_request->getQuery('__kiwi_id__') )
			{	// Forward to kiwi login URL
				$this->_redirect( $this->loginUrl . "?__kiwi_referer__={$referer}" );
			}else
			{	// If authentication information is set
				// Attempt to use it to authenticate
				$result = $auth->authenticate(
					new Zend_Auth_Adapter_KiwiId($this->_request->getQuery('__kiwi_id__')) );

				if( $result->isValid() )
				{	// If the authentication is valid
					$status = "You are logged-in as " . $auth->getIdentity() . "<br>\n";
					// Do database lookup for groups the user returned by kiwi check is a part of

					// TODO: log access to database
				}else
				{	// If authentication is invalid, remove user
					$auth->clearIdentity();
					$this->_redirect( $this->loginUrl . "?__kiwi_referer__={$referer}" );
				}

				// Add authentication messages to status
				foreach( $result->getMessages() as $message )
				{	$status .= "$message<br>\n";
				}
			}
		}
		// TODO: LOG TO 
		$request = $this->getRequest();
		$module = $request->getModuleName();
		if( !isset( $module ) )
		{	$module = Zend_Controller_Front::getInstance()->getDispatcher()->getDefaultModule();
		}
		$controller = $request->getControllerName();
		if( !isset( $controller ) )
		{	$controller = Zend_Controller_Front::getInstance()->getDispatcher()->getDefaultController();
		}
		$term = (date('Y') - 1900) . (1 + 4*(ceil(date('m') / 4) - 1));
		$filename = "$module-$controller-$term";
	}
}

class MathSocAuth_Controller_Action extends MathSoc_Controller_Action
{
	public function init()
	{	parent::init();
		$this->secure();
	}
}

