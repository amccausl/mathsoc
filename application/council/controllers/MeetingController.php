<?php

require_once 'MathSocAction.inc';

/**
 * This page should contain some general information about meetings and some Speaker and Secretary convience functions.
 */

class Council_MeetingController extends MathSoc_Controller_Action
{
	private $db;

	public function init($secure = false)
	{	parent::init($secure);
	}

	public function indexAction()
	{
	}

	/*
	 * Display information about a given meeting.  This includes the agenda (linked to policies/bylaws, attachments discussed), minutes
	 */
	public function infoAction()
	{
	}

	/*
	 * Allow the Speaker or President to call a meeting.  Post notice on webpage, calendar, mailing list.
	 */
	public function callAction()
	{
	}

	/*
	 * Allow for editting the agenda or location of a meeting
	 */
	public function editAction()
	{
	}

	public function minutesAction()
	{
		// TODO: extract params, call wikiPage
		// Create skeleton 
	}
}
