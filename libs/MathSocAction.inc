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
	private $_loginUrl = "https://strobe.uwaterloo.ca/cpadev/kiwi/user/login/";
	private $_cwd;
	protected $admins = array();

	public function init()
	{	parent::init();

		// Set the baseUrl as a variable that can be used by the view
		$this->view->baseUrl = $this->getRequest()->getBaseUrl();
		$this->_cwd = getcwd();

		// Load authentication and authorization module
		$this->authorization();
		
		// Force the user to authenticate if authentication is required for this page

		// Initialize the context-based menu
		require( '../application/default/views/helpers/menu.inc' );
		$this->view->menu = getMenu($this->getRequest());
	}

	protected function getModule()
	{
		$request = $this->getRequest();
		$module = $request->getModuleName();
		if( !isset( $module ) )
		{	$module = Zend_Controller_Front::getInstance()->getDispatcher()->getDefaultModule();
		}
		return $module;
	}

	protected function getController()
	{
		$request = $this->getRequest();
		$controller = $request->getControllerName();
		if( !isset( $controller) )
		{	$controller = Zend_Controller_Front::getInstance()->getDispatcher()->getDefaultController();
		}
		return $controller;
	}

	protected function getAction()
	{
		$request = $this->getRequest();
		$action = $request->getActionName();
		if( !isset( $action ) )
		{	$action = Zend_Controller_Front::getInstance()->getDispatcher()->getDefaultAction();
		}
		return $action;
	}

	protected function authorization()
	{	// Grab request information from the request object
		$module = $this->getModule();
		$controller = $this->getController();
		$action = $this->getAction();

		// Load the config from the registry
		$config = Zend_Registry::getInstance()->get('config');

		// Determine if the current page is authenticated and who for
		$secure = 1;

		$module = $config->auth->$module;
		if( isset($module->secure) )
		{	$secure = $module->secure;
			$controller = $module->$controller;
			if( isset( $controller->secure ) )
			{	$secure = $controller->secure;
				$action = $controller->$action;
				if( isset( $action->secure ) )
				{	$secure = $action->secure;
				}
			}
		}

		if( is_numeric( $secure ) )
		{	// $secure is a binary flag
			if( $secure )
			{	$this->secure();
			}
		}else
		{	// Secure is a ';' delimited string of positions that should have access
			// ex 'mathsoc/office:current;novelties:current'
			$this->admins = split(';', $secure);
			$this->secure();
		}
	}

	protected function secure( $groups = array() )
	{
		$referer = "http://{$_SERVER['HTTP_HOST']}{$_SERVER['REQUEST_URI']}";

		// If the user is returning from Kiwi, authenticate them
		$status = "";
		$auth = Zend_Auth::getInstance();

		if( !$auth->hasIdentity() )
		{	// If user not authenticated and query not set
			if( !$this->_request->isGet() || !$this->_request->getQuery('__kiwi_id__') )
			{	// Forward to kiwi login URL
				$this->_redirect( $this->_loginUrl . "?__kiwi_referer__={$referer}" );
			}else
			{	// If authentication information is set
				// Attempt to use it to authenticate
				$result = $auth->authenticate(
					new Zend_Auth_Adapter_KiwiId($this->_request->getQuery('__kiwi_id__')) );

				if( $result->isValid() )
				{	// If the authentication is valid
					$status = "You are logged-in as " . $auth->getIdentity() . "<br>\n";
					// Do database lookup for groups the user returned by kiwi check is a part of

					require_once( "userDB.inc" );
					$db = new UserDB();
					if( !$db->isMember($auth->getIdentity()) )
					{	$auth->clearIdentity();
						$this->_redirect( "/user/refund/" );
					}

					// Add group check to ensure that the user is able to access the desired system
					if( !empty( $groups ) )
					{	$groups = $this->admins;
					}
					if( !empty( $groups ) )
					{	$authorized = $db->isGroup( $auth->getIdentity(), $groups );
						if( !authorized )
							$this->_redirect( '/user/access_denied' . "?referer={$_SERVER ['HTTP_REFERER']}" );
					}

				}else
				{	// If authentication is invalid, remove user
					$auth->clearIdentity();
					$this->_redirect( $this->_loginUrl . "?__kiwi_referer__={$referer}" );
				}

				// Add authentication messages to status
				foreach( $result->getMessages() as $message )
				{	$status .= "$message<br>\n";
				}
			}
		}else
		{	// Add group check to ensure that the user is able to access the desired system
			if( !empty( $groups ) )
			{	$groups = $this->admins;
			}
			if( !empty( $groups ) )
			{	$authorized = $db->isGroup( $auth->getIdentity(), $groups );
				if( !authorized )
					$this->_redirect( '/user/access_denied' . "?referer={$_SERVER ['HTTP_REFERER']}" );
			}
		}
		$this->view->user = Zend_Auth::getInstance()->getIdentity();
	}

	protected function log()
	{	$auth = Zend_Auth::getInstance();
		$request = $this->getRequest();
		$module = $request->getModuleName();
		if( !isset( $module ) )
		{	$module = Zend_Controller_Front::getInstance()->getDispatcher()->getDefaultModule();
		}
		$module = strtolower($module);
		$controller = $request->getControllerName();
		if( !isset( $controller ) )
		{	$controller = Zend_Controller_Front::getInstance()->getDispatcher()->getDefaultController();
		}
		$controller = strtolower( $controller );
		$term = (date('Y') - 1900) . (1 + 4*(ceil(date('m') / 4) - 1));
		$filename = "$module-$controller-$term";

		$fh = fopen( "{$this->_cwd}/../data/hits/$filename", "a" );
		if( isset( $_SERVER['HTTP_REFERER'] ) )
			fputs( $fh, date("m/d H:i:s") . " - " . $auth->getIdentity() . " - {$_SERVER['REMOTE_ADDR']} ({$_SERVER['REQUEST_URI']}) - {$_SERVER['HTTP_USER_AGENT']} - {$_SERVER ["HTTP_REFERER"]}\n" );
		else
			fputs( $fh, date("m/d H:i:s") . " - " . $auth->getIdentity() . " - {$_SERVER['REMOTE_ADDR']} ({$_SERVER['REQUEST_URI']}) - {$_SERVER['HTTP_USER_AGENT']}\n" );
		fclose( $fh );
	}

	function __destruct()
	{	// Log the system access for refunds and usage purposes
		$this->log();
	}
}

