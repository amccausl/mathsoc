<div id='main-header'><h1></h1></div>
<div class="section">
{if $policy}
  <h2>{$policy.name}</h2>
  <ol class='policies'>
{$policy.content|indent:4}
  </ol>
{else}
  <h2></h2>
  <p>The policy you're looking for doesn't exist.</p>
{/if}
</div>
