<?php

require_once 'MathSocAction.inc';

class VolunteersController extends MathSoc_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();
	}

	public function indexAction()
	{	$volunteers = $this->db->getUsers();
		$this->view->volunteers = $volunteers;
	}

	public function getinvolvedAction()
	{
	}
}

