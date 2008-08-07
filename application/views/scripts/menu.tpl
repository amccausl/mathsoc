<div id='menu'>
{foreach name=top from=$menu item=menu_item1}
  {if $smarty.foreach.top.first}
  <ul>
  {/if}
    <li><a href='{$menu_item1.link}'>{$menu_item1.title}</a>
  {foreach name=middle from=$menu_item1.sub item=menu_item2}
    {if $smarty.foreach.middle.first}
	  <ul>
	{/if}
        <li><a href='{$menu_item2.link}'>{$menu_item2.title}</a>
    {foreach name=bottom from=$menu_item2.sub item=menu_item3}
	  {if $smarty.foreach.bottom.first}
		<ul>
	  {/if}
          <li><a href='{$menu_item3.link}'>{$menu_item3.title}</a></li>
      {if $smarty.foreach.bottom.last}
		</ul>
	  {/if}
    {/foreach}
		</li>
    {if $smarty.foreach.middle.last}
	  </ul>
	{/if}
  {/foreach}
    </li>
  {if $smarty.foreach.top.last}
  </ul>
  {/if}
{/foreach}
</div>
