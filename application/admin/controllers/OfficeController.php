<?php

require_once 'MathSocAction.inc';

require_once 'officeDB.inc';

class Admin_OfficeController extends MathSoc_Controller_Action
{
	private $db;

	public function init($secure = true)
	{	parent::init($secure);

		$this->db = new OfficeDB();
	}

	/** /admin/office
	 *
	 */
	public function indexAction()
	{
	}

	/** /admin/office/emails - Retrieve a list of the email address of the office workers for the term
	 * TODO: in the long run this should be a mail list
	 */
	public function emailAction()
	{
	}
}
