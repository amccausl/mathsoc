<?php

require_once 'MathSocAction.inc';
require_once 'clubsDB.inc';

/**
 * Allows users access to common functions for a club interaction
 *
 * @category   Zend
 * @package    Zend_Controller
 * @subpackage Zend_Controller_Action
 */
class ClubsController extends MathSoc_Controller_Action
{
	protected $db;

	public function init()
	{	parent::init();

		$this->db = new ClubsDB();
	}

	/**
	 * Display information about clubs.
	 */
    public function indexAction()
    {	$this->view->clubs = $this->db->getClubs();
    }

	/** /clubs/faq/
	 * Display a few common questions about clubs and their answers.
	 */
	public function faqAction()
	{
	}

	/**
	 * Allow users to join a club.
	 */
	public function joinAction()
	{	$this->secure();
		$this->view->clubs = $this->db->getClubs();
	}
}

