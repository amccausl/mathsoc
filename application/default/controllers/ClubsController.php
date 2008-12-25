<?php

require_once 'MathSocAction.inc';

/**
 * Allows users access to common functions for a club interaction
 *
 * @category   Zend
 * @package    Zend_Controller
 * @subpackage Zend_Controller_Action
 */
class ClubsController extends MathSoc_Controller_Action
{
	/**
	 * Display information about clubs.
	 */
    public function indexAction()
    {
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
	{	// TODO: ensure user is logged in
	}
}

