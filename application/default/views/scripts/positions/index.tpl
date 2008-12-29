<div class="section">
  <ul>
{foreach from=$positions item=position}
    <li>{$position.name} : {foreach from=$position.holders item=holder}{/foreach}</li>
{/foreach}
  </ul>
</div>
