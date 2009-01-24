<?php

require_once 'MathSocAction.inc';
require_once 'userDB.inc';

class VolunteersController extends MathSoc_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();
		$this->db = new UserDB();
	}

	public function indexAction()
	{	$volunteers = $this->db->getUsers();
		$this->view->volunteers = $volunteers;
	}

	public function getinvolvedAction()
	{
	}
}

