<div class="section">
  <h3>Executive</h3>
  <p>The Executives of the society represent the interests of math students.  They are elected for winter and fall or summer of a calendar year.</p>
  <h4>Winter/Fall Executives</h4>
  <ul>
{foreach from=$EXC item=position}
	{implode subject=position.holders glue=', ' assign=holders}
    <li><a href="{$baseUrl}/positions/{$position.alias}">{$position.name}</a> : {$holders}</li>
 {foreach from=$position.holders item=holder}{$holder.name} - {mailto address=$holder.email}{/foreach}
{/foreach}
  </ul>

  <h4>Summer Executives</h4>
  <ul>
{foreach from=$OEX item=position}
	{implode subject=position.holders glue=', ' assign=holders}
    <li><a href="{$baseUrl}/positions/{$position.alias}">{$position.name}</a> : {$holders}</li>
 {foreach from=$position.holders item=holder}{$holder.name} - {mailto address=$holder.email}{/foreach}
{/foreach}
  </ul>

  <h3>Directors</h3>
  <p></p>
  <ul>
{foreach from=$DIR item=position}
	{implode subject=position.holders glue=', ' assign=holders}
    <li><a href="{$baseUrl}/positions/{$position.alias}">{$position.name}</a> : {$holders}</li>
 {foreach from=$position.holders item=holder}{$holder.name} - {mailto address=$holder.email}{/foreach}
{/foreach}
  </ul>

  <h3>Student Representatives</h3>
  <p></p>
  <ul>
{foreach from=$REP item=position}
	{implode subject=position.holders glue=', ' assign=holders}
    <li><a href="{$baseUrl}/positions/{$position.alias}">{$position.name}</a> : {$holders}</li>
 {foreach from=$position.holders item=holder}{$holder.name} - {mailto address=$holder.email}{/foreach}
{/foreach}
  </ul>
 
  <h3>Appointed Positions</h3>
  <p></p>
  <ul>
{foreach from=$APP item=position}
	{implode subject=position.holders glue=', ' assign=holders}
    <li><a href="{$baseUrl}/positions/{$position.alias}">{$position.name}</a> : {$holders}</li>
 {foreach from=$position.holders item=holder}{$holder.name} - {mailto address=$holder.email}{/foreach}
{/foreach}
  </ul>
 
</div>
  <h3>Affiliates</h3>
  <p></p>
  <ul>
{foreach from=$AFF item=position}
	{implode subject=position.holders glue=', ' assign=holders}
    <li><a href="{$baseUrl}/positions/{$position.alias}">{$position.name}</a> : {$holders}</li>
 {foreach from=$position.holders item=holder}{$holder.name} - {mailto address=$holder.email}{/foreach}
{/foreach}
  </ul>
 
</div>
</div>
