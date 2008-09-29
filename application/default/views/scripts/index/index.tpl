<div id='main-header'><h1></h1></div>
{foreach from=$posts item=post}
<div class="section">
  <h2>{$post->title}</h2>
      {$post->content}
</div>
{foreachelse}
<div class="section">
  <h2>Page Content Missing</h2>
  <p>This page has no content to display.  If you were expecting content and suspect a problem, try emailing website@mathsoc.uwaterloo.ca to have the situation remedied.  Sorry for the inconvience.
</div>
{/foreach}
