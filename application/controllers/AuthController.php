<?php

require_once 'Zend/Controller/Action.php';

class AuthController extends Zend_Controller_Action
{
    public function indexAction()
    {	print( "This is the index" );
    }

	public function loginAction()
	{
	}

	public function logoutAction()
	{
	}
}

