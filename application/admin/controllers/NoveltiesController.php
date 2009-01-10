<?php

require_once 'MathSocAction.inc';

require_once 'noveltiesDB.inc';

class Admin_NoveltiesController extends MathSoc_Controller_Action
{
	private $db;

	public function init($secure = true)
	{	parent::init($secure);

		$this->db = new NoveltiesDB();
	}

	/** /admin/novelties - used to review existing novelties and submissions
	 *
	 */
	public function indexAction()
	{	$this->view->novelties = $this->db->getNovelties();
	}

}
