<?php

require_once 'MathSocAction.inc';
require_once '../application/models/announceDB.inc';

class EventsController extends MathSoc_Controller_Action
{
	private $db;

	public function init()
	{	parent::init();
		$this->db = $this->db = new AnnounceDB();
	}

	public function indexAction()
	{	$this->view->posts = $this->db->getAnnouncements();
	}
}

