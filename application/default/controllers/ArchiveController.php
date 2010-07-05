<?php

require_once 'MathSocAction.inc';
require_once 'announceDB.inc';

class ArchiveController extends MathSoc_Controller_Action
{
  private $db;
  
  public function init()
  {
    parent::init();
    $this->db = new AnnounceDB();
  }

  public function indexAction()
  {
    $posts = $this->db->getAnnouncements();
    $this->view->posts = $posts;
  }

}

?>