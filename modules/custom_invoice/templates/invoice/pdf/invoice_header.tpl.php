<?php
   global $base_url;  										
   $lfid = variable_get('invoice_logo_fid', '');
   $logo = '';
   if ($lfid != '') {
      $file = file_load($lfid);
      if ($file && isset($file->fid)) {
       $logo = file_create_url($file->uri);
      }
   }
   $team = variable_get('invoice_team_name', t('STP'));
   $color = variable_get('invoice_color', t('#000'));
   $abn_number = variable_get('abn_number', 0);
?>
<div class="printHeader">
  <div class="logo"><img src="<?php print $logo== "" ? 'sites/all/modules/custom/custom_block/images/print_logo.png' : $logo ?>" alt="<?php print $team ?>" title="<?php print $team ?>" /></div>
  <div class="head-cnt-mid">
     <div class="teamname"><?php print $team ?></div>
     <div class="teamdtl"><?php print $invoice_list['invoice_long_descriptions'];?></div>
     <div class="teamdtl"><?php print $invoice_id; ?></div>
  </div>
  <div class="head-cnt-rgt">
     <div class="date-time">Invoice Date: <?php print $invoice_date;?></div>
     <?php if ($abn_number): ?><div class="teamdtl">&nbsp;</div><div class="date-time"><?php print $abn_number;?></div> <?php endif; ?>
   </div>
</div>
  