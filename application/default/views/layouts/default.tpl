<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
  <meta http-equiv="content-type" content="text/html; charset=utf-8" />
  <title>{$title}</title>
{foreach from=$stylesheets item=stylesheet}
  <link rel='stylesheet' type='text/css' href='{$stylesheet}' />
{/foreach}
{foreach from=$javascripts item=javascript}
  <script language='Javascript' type='text/javascript' src='{$javascript}'></script>
{/foreach}
  <!--[if IE]>
  <style type="text/css" media="screen">
  body {ldelim}
  behavior: url(csshover.htc); /* call hover behaviour file */
  font-size: 100%; /* enable IE to resize em fonts */
  {rdelim}
  #menu ul li {ldelim}
  float: left; /* cure IE5.x "whitespace in lists" problem */
  width: 100%;
  {rdelim}
  #menu ul li a {ldelim}
  height: 1%; /* make links honour display: block; properly */
  {rdelim}

  #menu a, #menu h2 {ldelim}
  font: bold 0.7em/1.4em arial, helvetica, sans-serif; 
  /* if required use em's for IE as it won't resize pixels */
  {rdelim}
  </style>
  <![endif]-->
</head>
<body>
<div id='wrapper'>
  <div id='header'>
    <div id="logo"><img src='{$baseUrl}/images/logo.jpg' alt='MathSoc Logo'/></div>
  </div>
  <div id="content">
{include file="../../../default/views/scripts/menu.tpl"}
    <div id="main">
{$content_data|indent:6}
    </div>
    <!-- TODO: Include javascript calls here -->
	<div id="footer">
	</div>
  </div>
</div>
</body>
</html>
