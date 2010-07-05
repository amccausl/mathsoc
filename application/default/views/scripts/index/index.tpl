<div id='main-header'><h1></h1></div>
{foreach from=$posts item=post}
<div class="section">
  <h2>{$post.title}{if $post.event_date} ({$post.event_date}){/if}</h2>
      {$post.content}
</div>
{foreachelse}
<div class="section">
<p>There are no recent announcements.</p>
</div>
{/foreach}
<div class="section">
<p>To view older news, <a href="/Archive">click here</a>.</p>
</div>
