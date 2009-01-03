<div id='main-header'><h1></h1></div>
<div class="section">
{if $bylaw}
  <h2>{$bylaw.name}</h2>
  <ol class='bylaws'>
{$bylaw.content|indent:4}
  </ol>
{else}
  <h2></h2>
  <p>The bylaw you're looking for doesn't exist.</p>
{/if}
</div>
