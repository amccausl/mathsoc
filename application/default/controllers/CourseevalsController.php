<?php

require_once 'MathSocAction.inc';
require_once 'courseevalsDB.inc';

class CourseevalsController extends MathSoc_Controller_Action
{
	private $db;
	private $examDir;

	public function init()
	{
		parent::init(true);
		$config = new Zend_Config_Ini('../config/main.ini', 'exambank');
		$this->db = new CourseEvalsDB();
		$this->examDir = $config->examDir;
	}

	// Browsing Functions
	public function indexAction()
	{
		$evals = $this->db->getEvaluations();
		$this->view->evals = $evals;
	}

	public function displayAction()
	{
		$page = $this->_getParam("page");
		$term = $this->_getParam("term");

		if ($page == "profs" || $page == "tas")
		{
			$terminfo = $this->db->getTermEvaluation($term);
			if ($terminfo)
			{
				Zend_Controller_Front::getInstance()->setParam('noViewRenderer', true);
				if ($page == "profs")
				{
					$type = $terminfo[0]['prof_type'];
					$file = $terminfo[0]['prof_file'];
				}
				else
				{
					$type = $terminfo[0]['ta_type'];
					$file = $terminfo[0]['ta_file'];
				}

				if ($buffer = file_get_contents($this->examDir . $file))
				{
					$ext = explode("\t", `cat /etc/mime.types | grep $type | awk 'BEGIN {FS=" ";} {print $2}'`);
					$ext = $ext[count($ext)-1];
					header("Content-type: ".$type);
					header("Content-Disposition: attachment; filename={$term}{$page}.{$ext}");
					echo ($buffer);
					exit;
				}
			}
		}
	}
}
