<div id='main-header'><h1></h1></div>
<div class="section">
{if $position}
  <h2>{$position.name}</h2>

  <h3>Description</h3>
  {$position.description}

  <br/><h3>Availability</h3>
  {$position.current_count} of {$position.number} positions taken for this term.<br/>
  
{if $position.current_count < $position.number}
  <p>If you're interested in applying for this position, you can email the VPAS to figure out how.</p>
{/if}

  <br/><h3>Contact Information</h3>
{if isset($position.email)}
	{foreach from=$position.current item=current}
    	{$current.name}<br/>
	{/foreach}
    <br/>{mailto address=$position.email}
{else}
	{foreach from=$position.current item=current}
        {$current.name} - {if isset( $current.email )}{mailto address=$current.email}{/if}<br/>
	{foreachelse}
		{if isset( $position.default_email ) }{mailto address=$position.default_email}{else}<p>There is currently no volunteers for this position.</p>{/if}
	{/foreach}
{/if}

  <br/><h3>Position History</h3>
  <ul>
	{foreach from=$position.holders key=term item=holders}
	{implode subject=$holders glue=', ' assign=holders}
    <li>{term id=$term display="long"} -&gt; {$holders}</li>
	{/foreach}
  </ul>
{else}
  <p>The position you're looking for doesn't exist</p>
{/if}
</div>
