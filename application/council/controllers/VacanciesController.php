<?php

require_once 'MathSocAction.inc';
require_once 'userDB.inc';

class Council_VacanciesController extends MathSoc_Controller_Action
{
	private $db;

	public function init($secure = false)
	{	parent::init($secure);

		$this->db = new UserDB();
	}

	public function indexAction()
	{	$positions = $this->db->getAvailablePositions();
		$this->view->assign($positions);
	}
}
