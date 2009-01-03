<div id='main-header'><h1></h1></div>
<div class="section">
{if $content}
  <h2>{$name}</h2>
  <ol id='policies'>
{$content|replace:'&lt;':'<'|replace:'&gt;':'>'|indent:4}
  </ol>
{else}
  <h2></h2>
  <p>The policy you're looking for doesn't exist.</p>
{/if}
</div>
