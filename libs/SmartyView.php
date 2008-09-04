<?php
class SmartyView extends Zend_View_Abstract
{
    protected $_smarty;
	protected $_default_template;
	protected $_layout_dir;

    public function __construct($config = array())
    {
        $this->_smarty = new Smarty();

        if(!isset($config->compiledir))
            throw new Exception('compileDir is not set for '.get_class($this));
        else
            $this->_smarty->compile_dir = $config->compiledir;

        if(isset($config->pluginDir))
            $this->_smarty->plugin_dir[] = $config->pluginDir;

		$this->_default_template = $config->layout->default . "." . $config->suffix;

		$this->_layout_dir = $config->layout->path;

        parent::__construct($config);
    }

    public function getEngine()
    {
        return $this->_smarty;
    }

    public function __set($key,$val)
    {
        $this->_smarty->assign($key,$val);
    }

    public function __isset($key)
    {
        $var = $this->_smarty->get_template_vars($key);
        if($var)
            return true;
        
        return false;
    }

    public function __unset($key)
    {
        $this->_smarty->clear_assign($key);
    }

    public function assign($spec,$value = null)
    {
        if($value === null)
            $this->_smarty->assign($spec);
        else
            $this->_smarty->assign($spec,$value);
    }

    
    public function clearVars()
    {
        $this->_smarty->clear_all_assign();
    }

    protected function _run()
    {
        $this->strictVars(true);

        $this->_smarty->assign_by_ref('this',$this);

        $templateDirs = $this->getScriptPaths();

        $file = substr(func_get_arg(0),strlen($templateDirs[0]));
        $this->_smarty->template_dir = $templateDirs[0];
        $this->_smarty->compile_id = $templateDirs[0];

		if( $this->_smarty->get_template_vars("content_data") )
		{	echo $this->_smarty->fetch($this->_layout_dir . $this->_default_template);
		}else
		{	$content_data = $this->_smarty->fetch($file);
			$this->_smarty->assign("content_data",$content_data);
			if( $layout = $this->_smarty->get_template_vars('layout') )
			{	echo $this->_smarty->fetch($this->_layout_dir . $layout);
			}else
			{	echo $this->_smarty->fetch($this->_layout_dir . $this->_default_template);
			}
		}
    }
}

