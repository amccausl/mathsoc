{* Display title information for the page here *}
<div id='main-header'><h1></h1></div>
<div class="section">
  <h2>MathSoc Bylaws</h2>
  <ol>
{foreach from=$bylaws item=bylaw name=loop}
    <li><a href="{$baseUrl}/council/bylaws/{$bylaw.name|replace:' ':'+'}">{$bylaw.name}</a></li>
{/foreach}
  </ol>
</div>
