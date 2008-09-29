{* Display title information for the page here *}
<div class="section">
  <h2>MathSoc Policies</h2>
  <ol>
{foreach from=$policies item=policy name=loop}
    <li><a href="{$baseUrl}/council/policies/{$policy.name|replace:' ':'+'}">{$policy.name}</a></li>
{/foreach}
  </ol>
</div>
