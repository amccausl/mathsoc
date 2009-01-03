<?php

require_once 'MathSocAction.inc';

class Council_IndexController extends MathSoc_Controller_Action
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
		$this->db = new UserDB();
		$this->initView();
	}

	public function indexAction()
	{
		$positions = $this->db->getPositionsBycategory();
		$this->view->assign($positions);
	}

	public function minutesAction()
	{
		// TODO: extract params, call wikiPage
	}

	public function wikiAction()
	{	print( 'hello world' );
	}
}
