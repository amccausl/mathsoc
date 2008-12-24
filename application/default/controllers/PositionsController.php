<?php

require_once 'MathSocAction.inc';

class PositionsController extends MathSoc_Controller_Action
{
	/** /positions/ - Display current positions and who holds them
	 *
	 */
    public function indexAction()
	{	// Display user information

		// Access to email, clubs, sections for positions held
    }

	public function logoutAction()
	{	// Remove the authentication session information
		Zend_Auth::getInstance()->clearIdentity();
		// TODO: forward to kiwi logout url
		$this->_redirect('/');
	}

	public function profileAction()
	{
		require_once( "userDB.inc" );
		$db = new UserDB();
		$user = $db->getProfile( $this->_getParam('position') );

	}
}

