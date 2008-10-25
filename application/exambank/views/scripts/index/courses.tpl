{assign var='layout' value='blank'}
([{foreach from=$values item=value name=loop}{if $smarty.foreach.loop.first}'{$value}'{else}, '{$value}'{/if}{/foreach}])
