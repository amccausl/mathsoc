<div class="section">
  <h2>{$position.name}</h2>

  <h3>Description</h3>
  {$position.description}

  INCLUDE HOW TO APPLY FOR THE POSITION HERE

  {$position.number} positions available each term.

  <h3>Contact Information</h3>
{if isset($position.email)}
	{foreach from=position.current item=$current}
    	{$current.name}<br/>
	{/foreach}
    <br/>{mailto address=$position.email}
{else}
	{foreach from=position.current item=$current}
        {$current.name} - {mailto address=$current.email}<br/>
	{/foreach}
{/if}

  <h3>Position History</h3>
  <ul>
	{foreach from=$position['holders'] key=term item=holders}
	{implode subject=holders glue=', ' assign=holders}
    <li>{$term} -> {$holders}</li>
	{/foreach}
  </ul>
</div>
