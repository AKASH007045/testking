<?php if ($type == 1) { ?><article class="infoPanelwrap">
	
    <section class="namecntinf col-xs-4">
        <h2><span class="blueTxt"><?php print $name;?></span></h2>
        <div class="contDetail">
          <?php print (isset($user_info->customer['street_addr_1']) && !empty($user_info->customer['street_addr_1'])) ? $user_info->customer['street_addr_1'] . ',<br/> ': '&nbsp;'; ?><?php
            print (isset($user_info->customer['city']) && !empty($user_info->customer['city'])) ? $user_info->customer['city'] . ', ' : '' ;
            print (isset($user_info->customer['state']) && !empty($user_info->customer['state'])) ? $user_info->customer['state'] . ', ' : '' ;
            print (isset($user_info->customer['zip']) && !empty($user_info->customer['zip'])) ? $user_info->customer['zip'] : '&nbsp;' ; ?>
         </div>
         <?php if (isset($user_info->customer['phone_day']) || isset($user_info->customer['phone_cell'])) { ?>
           <div class="contDetail">Phone: <?php print (isset($user_info->customer['phone_day']) && $user_info->customer['phone_day']) ? $user_info->customer['phone_day'] : $user_info->customer['phone_cell']; ?></div>
         <?php } ?>
        <div class="contDetail">Email: <?php print (isset($user_info->customer['email']) && $user_info->customer['email']) ? $user_info->customer['email'] : ''; ?></div>
        
        
    </section>                
    <section class="account-panel col-xs-4 text-center">
     <h2>Account # <span class="blueTxt"><?php print isset($user_info->customer['acct_id']) ? $user_info->customer['acct_id'] : ''; ?></span></h2>
    </section>
    <?php print isset($acct_rep) ? $acct_rep : '';?>
</article>
<?php } elseif ($type == 2) { ?>
    <div class="addtl"><?php print $name;?></div>
    <div class="addtl"><?php print isset($user_info->customer['acct_id']) ? $user_info->customer['acct_id'] : ''; ?></div>
    <div class="addtl"><?php print (isset($user_info->customer['street_addr_1']) && !empty($user_info->customer['street_addr_1'])) ? $user_info->customer['street_addr_1'] : ''; ?></div>
    <div class="addtl"><?php
             print (isset($user_info->customer['city']) && !empty($user_info->customer['city'])) ? $user_info->customer['city'] . ', ' : '' ;
            print (isset($user_info->customer['state']) && !empty($user_info->customer['state'])) ? $user_info->customer['state'] . ', ' : '' ;
            print (isset($user_info->customer['zip']) && !empty($user_info->customer['zip'])) ? $user_info->customer['zip'] : '&nbsp;' ; ?></div>
    <div class="addtl"><?php print (isset($user_info->customer['email']) && $user_info->customer['email']) ? $user_info->customer['email'] : ''; ?></div>
<?php } ?>