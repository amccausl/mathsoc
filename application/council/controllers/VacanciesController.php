<?php

require_once 'MathSocAction.inc';
require_once 'userDB.inc';

class Council_VacanciesController extends MathSoc_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();

		$this->db = new UserDB();

		// User must be authenticated to see any of these pages
		$this->view->baseUrl = $this->_request->getBaseUrl();
		$this->initView();
	}

	public function indexAction()
	{	$positions = $this->db->getAvailablePositions();
		$this->view->assign($positions);
	}
}
