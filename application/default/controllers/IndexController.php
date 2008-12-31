<?php

require_once 'MathSocAction.inc';
require_once 'announceDB.inc';

class IndexController extends MathSoc_Controller_Action
{
	// TODO: initialize announcement database
	private $db;

	public function init()
	{	parent::init();
		$this->db = new AnnounceDB();
	}

	public function indexAction()
	{	$posts = $this->db->getAnnouncements();
		$this->view->posts = $posts;
	}
}

