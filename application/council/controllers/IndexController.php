<?php

require_once 'Zend/Controller/Action.php';

class Council_IndexController extends Zend_Controller_Action
{
	private $db;

	public function wikiPage($section = null, $page = null)
	{
		// TODO: set WikiDir

		ob_start();
		
		// Render content from wiki
		include( 'pmwiki-latest/pmwiki.php' );

		$this->view->content_data = ob_get_clean();
	}

	public function init()
	{	parent::init();

		// User must be authenticated to see any of these pages
		$this->view->baseUrl = $this->_request->getBaseUrl();
		$this->view->user = Zend_Auth::getInstance()->getIdentity();
		$this->initView();
	}

/*
    function preDispatch()
	{
		$auth = Zend_Auth::getInstance();
		print( "auth = " . $auth->hasIdentity() );

		if (!$auth->hasIdentity())
		{	$this->_redirect('auth/login');
		}
	}
*/

	public function indexAction()
	{
	}

	public function minutesAction()
	{
		// TODO: extract params, call wikiPage
	}

	public function wikiAction()
	{	print( 'hello world' );
	}
}
