<div id='main-header'><h1></h1></div>
<div class="section">
  <h3>MathSoc's club listing:</h3>
  <ul>
{foreach from=$clubs item=club}
{if $club['url']}
    <li><a href="{$club.url}">{$club.name}</a></li>
{else}
    <li>{$club.name}</li>
{/if}
{/foreach}
  </ul>
</div>
