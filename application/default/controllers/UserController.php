<?php

require_once 'MathSocAction.inc';

class UserController extends MathSocAuth_Controller_Action
{
    public function indexAction()
	{	// Display user information

		// Access to email, clubs, sections for positions held
    }

	public function logoutAction()
	{	// Remove the authentication session information
		Zend_Auth::getInstance()->clearIdentity();
		$this->_redirect('https://strobe.uwaterloo.ca/cpadev/kiwi/user/out?__kiwi_referer__='
								. $_SERVER["HTTP_REFERER"]);
	}

	public function profileAction()
	{
		require_once( "userDB.inc" );
		$db = new UserDB();
		$user = $db->getProfile( $this->_getParam('username') );

		$this->view->user = $user;
	}
}

