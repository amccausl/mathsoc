<div id='main-header'><h1></h1></div>
<div class="section">
  <h3>Executive</h3>
  <p>The Executives of the society are elected by math students and entrusted to run the society on their behalf.  They are elected for either the winter and fall or summer terms of a calendar year.</p>
  <h4>Winter/Fall Executives</h4>
  <ul>
{foreach from=$EXC item=position}
    <li><a href="{$baseUrl}/positions/details?position={$position.alias}">{$position.name}</a> : 
  {foreach from=$position.holders item=holder}{$holder}{foreachelse}Position Vacant{/foreach}</li>
{/foreach}
  </ul>

  <h4>Summer Executives</h4>
  <ul>
{foreach from=$OEX item=position}
    <li><a href="{$baseUrl}/positions/details?position={$position.alias}">{$position.name}</a> : 
  {foreach from=$position.holders item=holder}{$holder}{foreachelse}Position Vacant{/foreach}</li>
{/foreach}
  </ul>

  <h3>Directors</h3>
  <p></p>
  <ul>
{foreach from=$DIR item=position}
    <li><a href="{$baseUrl}/positions/details?position={$position.alias}">{$position.name}</a> : 
  {foreach from=$position.holders item=holder}{$holder}{foreachelse}Position Vacant{/foreach}</li>
{/foreach}
  </ul>

  <h3>Student Representatives</h3>
  <p></p>
  <ul>
{foreach from=$REP item=position}
    <li><a href="{$baseUrl}/positions/details?position={$position.alias}">{$position.name}</a> : 
  {foreach from=$position.holders item=holder}{$holder}{foreachelse}Position Vacant{/foreach}</li>
{/foreach}
  </ul>
 
  <h3>Appointed Positions</h3>
  <p></p>
  <ul>
{foreach from=$APP item=position}
    <li><a href="{$baseUrl}/positions/details?position={$position.alias}">{$position.name}</a> : 
  {foreach from=$position.holders item=holder}{$holder}{foreachelse}Position Vacant{/foreach}</li>
{/foreach}
  </ul>
 
  <h3>Affiliates</h3>
  <p></p>
  <ul>
{foreach from=$AFF item=position}
    <li><a href="{$baseUrl}/positions/details?position={$position.alias}">{$position.name}</a> : 
  {foreach from=$position.holders item=holder}{$holder}{foreachelse}Position Vacant{/foreach}</li>
{/foreach}
  </ul>
 
</div>
