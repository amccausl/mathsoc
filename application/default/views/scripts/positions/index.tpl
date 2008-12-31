<div class="section">
  <ul>
{foreach from=$positions item=position}
	{implode subject=position.holders glue=', ' assign=holders}
    <li><a href="{$baseUrl}/positions/{$position.alias}">{$position.name}</a> : {$holders}</li>
 {foreach from=$position.holders item=holder}{$holder.name} - {mailto address=$holder.email}{/foreach}
{/foreach}
  </ul>
</div>
