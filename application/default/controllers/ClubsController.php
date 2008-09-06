<?php

require_once 'Zend/Controller/Action.php';

/**
 * Allows users access to common functions for a club interaction
 *
 * @category   Zend
 * @package    Zend_Controller
 * @subpackage Zend_Controller_Action
 */
class ClubsController extends Zend_Controller_Action
{
	/**
	 * Display information about clubs.
	 */
    public function indexAction()
    {
    }

	/**
	 * Allow users to join a club.
	 */
	public function joinAction()
	{	// TODO: ensure user is logged in
	}	
}

