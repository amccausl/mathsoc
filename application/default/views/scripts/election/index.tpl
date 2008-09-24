{assign var='layout' value='blank'}
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN">
<html>
<head>
<title>MathSoc Online Voting</title>

<link rel="stylesheet" href="js/lists.css" type="text/css"/>
<script language="javascript" type="text/javascript" 
            src="firebug/firebug.js"></script>
<script language="JavaScript" type="text/javascript" src="js/coordinates.js"></script>
<script language="JavaScript" type="text/javascript" src="js/prototype.js"></script>
<script language="JavaScript" type="text/javascript" src="js/drag.js"></script>
<script language="JavaScript" type="text/javascript" src="js/request.js"></script>
<script language="JavaScript" type="text/javascript" src="js/election.js"></script>
<script language="JavaScript" type="text/javascript"><!--
  window.onload = function() {ldelim}
{foreach from=$elections item=election}
  {foreach from=$election.candidates key=candidate item=info name=loop}
  {if $smarty.foreach.loop.first}var C_{$election.id} = [{/if}
    {ldelim} id : '{$candidate}',
       display : '{$info.name}'{if $info.link},
       link : '{$info.link}'{/if}
    {rdelim}
    {if !$smarty.foreach.loop.last}, {else}];
    var election{$election.id} = new Election( 'election{$election.id}', C_{$election.id}, false, false);{/if}
  {foreachelse}
    var election{$election.id} = new Election( 'election{$election.id}', null, false, false );
  {/foreach}
{/foreach}
{ldelim};
  //-->
</script>
</head>
<body>
<div id="instructions">
Please rate the following candidates in order of preference (1 being the best candidate for the job and n being the worst) by dragging the candidates onto your "ballot" box from your "candidates" box or by double-clicking on the desired candidate in each election.  You do not need to rank all candidates if you don't wish to.  You can rank as many or as few candidates as you like.  The votes will be tallied by the <a href="http://www.bcstv.ca/">BC-STV</a> voting system.  If you have any problems with the system, you can email <a href="mailto:website@mathsoc.uwaterloo.ca">website@mathsoc.uwaterloo.ca</a> or <a href="mailto:cro@mathsoc.uwaterloo.ca">cro@mathsoc.uwaterloo.ca</a> for assistance. 
</div>
{foreach from=$elections item=election}
<div id='election{$election.id}'>
  <div class='electionHead'>
    <h2>{$election.position}</h2>
    <p>{$election.description}</p>
  </div>
</div>
{/foreach}
</body>
</html>
