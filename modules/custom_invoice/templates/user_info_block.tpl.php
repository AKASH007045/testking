<?php if ($type == 1) { ?><article class="infoPanelwrap">
    <section class="namecntinf col-md-5">
        <h2><span class="blueTxt"><?php print $name;?></span></h2>
        <div class="contDetail">
          <?php print (isset($user_info->street_addr_1) && !empty($user_info->street_addr_1)) ? $user_info->street_addr_1 . ',<br/> ': '&nbsp;'; ?><?php
            print (isset($user_info->city) && !empty($user_info->city)) ? $user_info->city . ', ' : '' ;
            print (isset($user_info->state) && !empty($user_info->state)) ? $user_info->state . ', ' : '' ;
            print (isset($user_info->zip) && !empty($user_info->zip)) ? $user_info->zip : '&nbsp;' ; ?>
         </div>
        <div class="contDetail">Phone: <?php print (isset($user_info->phone_day) && $user_info->phone_day) ? $user_info->phone_day : '&nbsp;'; ?></div>
        <div class="contDetail">Email: <?php print (isset($user_info->email) && $user_info->email) ? $user_info->email : ''; ?></div>
        
        
    </section>                
    <section class="account-panel col-md-3">
     <h2>Account # <span class="blueTxt"><?php print isset($user_info->acct_id) ? $user_info->acct_id : ''; ?></span></h2>
    </section>
    <?php print isset($acct_rep) ? $acct_rep : '';?>
</article>
<?php } elseif ($type == 2) { ?>
    <div class="addtl"><?php print $name;?></div>
    <div class="addtl"><?php print isset($user_info->acct_id) ? $user_info->acct_id : ''; ?></div>
    <div class="addtl"><?php print (isset($user_info->street_addr_1) && !empty($user_info->street_addr_1)) ? $user_info->street_addr_1 : ''; ?></div>
    <div class="addtl"><?php
             print (isset($user_info->city) && !empty($user_info->city)) ? $user_info->city . ', ' : '' ;
            print (isset($user_info->state) && !empty($user_info->state)) ? $user_info->state . ', ' : '' ;
            print (isset($user_info->zip) && !empty($user_info->zip)) ? $user_info->zip : '&nbsp;' ; ?></div>
    <div class="addtl"><?php print (isset($user_info->email) && $user_info->email) ? $user_info->email : ''; ?></div>
<?php } ?>