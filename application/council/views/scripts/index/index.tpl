<div id='main-header'><h1></h1></div>
<div class="section">
  <p>MathSoc council is the governing body of the society.  The make decisions as to funding and administrational decisions.  The Council is composed of the following members:</p>

  <h3>Executive</h3>
  <p>The Executives of the society are elected by math students and entrusted to run the society on their behalf.  They are elected for either the winter and fall or summer terms of a calendar year.</p>
  <h4>Spring 2010 Executives</h4>
  <ul>
{foreach from=$EXC item=position}
    <li><a href="{$baseUrl}/positions/details?position={$position.alias}">{$position.name}</a> :
  {foreach from=$position.holders item=holder}{$holder}, {foreachelse}Position Vacant{/foreach}</li>
{/foreach}
  </ul>

<h4>Fall 2010 Executives</h4>
  <ul>
{foreach from=$OEX item=position}
    <li><a href="{$baseUrl}/positions/details?position={$position.alias}">{$position.name}</a> :
  {foreach from=$position.holders item=holder}{$holder}, {foreachelse}Position Vacant{/foreach}</li>
{/foreach}
  </ul>
  <h3>Class Representatives</h3>
  <p></p>
  <ul>
{foreach from=$REP item=position}
    <li><a href="{$baseUrl}/positions/details?position={$position.alias}">{$position.name}</a> :
  {foreach from=$position.holders item=holder}{$holder}, {foreachelse}Position Vacant{/foreach}</li>
{/foreach}
  </ul>

  <h3>Appointed Positions</h3>
  <p></p>
  <ul>
{foreach from=$APP item=position}
    <li><a href="{$baseUrl}/positions/details?position={$position.alias}">{$position.name}</a> :
  {foreach from=$position.holders item=holder}{$holder}, {foreachelse}Position Vacant{/foreach}</li>
{/foreach}
  </ul>

</div>
