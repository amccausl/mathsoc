<?php
/**
 * Zend Framework
 *
 * LICENSE
 *
 * This source file is subject to the new BSD license that is bundled
 * with this package in the file LICENSE.txt.
 * It is also available through the world-wide-web at this URL:
 * http://framework.zend.com/license/new-bsd
 * If you did not receive a copy of the license and are unable to
 * obtain it through the world-wide-web, please send an email
 * to license@zend.com so we can send you a copy immediately.
 *
 * @category   Zend
 * @package    Zend_Auth
 * @subpackage Zend_Auth_Adapter
 * @copyright  Copyright (c) 2005-2008 Zend Technologies USA Inc. (http://www.zend.com)
 * @license    http://framework.zend.com/license/new-bsd     New BSD License
 * @version    $Id: OpenId.php 10096 2008-07-15 15:11:25Z dmitry $
 */


/**
 * @see Zend_Auth_Adapter_Interface
 */
require_once 'Zend/Auth/Adapter/Interface.php';

require_once 'CAS-1.1.2/CAS.php';


/**
 * A Zend_Auth Authentication Adapter allowing the use of KiwiID protocol as an
 * authentication mechanism
 *
 * @category   Zend
 * @package    Zend_Auth
 * @subpackage Zend_Auth_Adapter
 * @copyright  Copyright (c) 2005-2008 Zend Technologies USA Inc. (http://www.zend.com)
 * @license    http://framework.zend.com/license/new-bsd     New BSD License
 */
class Zend_Auth_Adapter_KiwiId implements Zend_Auth_Adapter_Interface
{
    /**
     * UW Kiwi specific settings
     *
     * @var string
     */
	//private $loginUrl = "https://strobe.uwaterloo.ca/cpadev/kiwi/user/login/";
	//private $kiwiKey = "1a9e0ea4-e5c4-4f66-a1fa-cd722aa982e7";
	//private $checkUrl = "http://kiwi.uwaterloo.ca/user/check";

    /**
     * Constructor
     *
     * @param string $id the identity value
     * @return void
     */
    public function __construct($id = null) {
    }

    /**
     * Sets the value to be used as the identity
     *
     * @param  string $id the identity value
     * @return Zend_Auth_Adapter_KiwiId Provides a fluent interface
     */
    public function setIdentity($id)
    {
        $this->_id = $id;
        return $this;
    }

    /**
     * Authenticates the given KiwiId identity.
     * Defined by Zend_Auth_Adapter_Interface.
     *
     * @throws Zend_Auth_Adapter_Exception If answering the authentication query is impossible
     * @return Zend_Auth_Result
     */
    public function authenticate()
	{
		$username = phpCAS::getUser();
		return new Zend_Auth_Result(Zend_Auth_Result::SUCCESS, $username, array("success"));
    }
}

