<div id='main-header'><h1></h1></div>
<div class="section">
{if $novelty}
	Name: {$novelty.name}<br/>
	Submitted By: {$novelty.submitter}<br/>
	Type: {$novelty.style}<br/>
	Name: {$novelty.name}<br/>
	Description:
	<p>{$novelty.description}</p>
	Notes:
	<p>{$novelty.notes}</p>
	Images:<br/>
{foreach from=$novelty.images item=image}
  <img src="{$baseUrl}/novelties/display?id={$novelty.id}&image={$image.name}" alt="{$image.name}"/>
{/foreach}
{/if}
  <h2>Other Novelties</h2>
  <ul>
{foreach from=$novelties item=novelty}
    <li><a href="{$baseUrl}/novelties/display?id={$novelty.id}">{$novelty.name}</a></li>
{/foreach}
  </ul>
</div>
