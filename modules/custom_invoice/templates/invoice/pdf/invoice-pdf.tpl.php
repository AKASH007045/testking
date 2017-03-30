    <?php
   global $base_url;
   $reps_info = strip_tags($acct_rep_info);
   $rep_str = trim($reps_info);
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
   //Already enabled
   //$footer1 =  variable_get('invoice_footer', array());
   //$terms =  isset($footer1['value']) ? $footer1['value'] : '';
?>
<style>
      
     .printHeader{ border-bottom:5px solid <?php print $color; ?>;}
     .head-cnt-mid .teamname{ color:<?php print $color; ?>; }
     .bdr5lft{ border-left:5px solid <?php print $color; ?>; }
     .lftblck .actinf, .rgtblck .actinf {color:<?php print $color; ?>;}
     .rgtblck .ttlamtd .tlyl {color:<?php print $color; ?>;}
     .chrt-dtl tr.head td{color:<?php print $color; ?>;}
	 .tbl-chart{height:325px;padding-bottom:25px;}
</style>
<div class="printWrapper printContent">
  <div class="wrp-main bdr3btn">
    <div class="lftblck">
      <div class="actinf">Account Info</div>
    </div>
    <div class="cmtblck">
      <?php print (isset($user_info) && $user_info) ? $user_info : ''; ?>
    </div>
    <div class="rgtblck">
      <div class="ttlamtd">Total Amount Due: <span class="tlyl"><?php print $due_amount;?></span></div>
    </div>
  </div>
  <div class="wrp-main bdr3btn">
    <div class="lftblck"> <div class="actinf">Invoice Details</div></div>
    <div class="tbl-chart"> <?php print $invoice_table;?></div>
  </div>
  <div class="wrp-main bdr2btn">
    <div class="cmtblck">
      <div class="addtl">
        <?php print $invoice_other_info_text;?>  
      </div>
    </div>
    <div class="cmtblck">
      <div class="addtl"></div>
      <div class="addtl"></div>
    </div>
    <div class="rgtblck bdr5lft billing">
      <?php print $total;?>
      <?php print $parking_fee;?>
      <?php print $handling_fee;?>
      <?php print $paid;?>
        <div class="tcwrp"><div class="tcnm fnt16glb">Total Amount Due:</div> <div class="tcamt actinf"><?php echo $due_amount;?></div></div>
    </div>
  </div>
  <?php if (!empty($rep_str)): ?>
   <div class="wrp-main bdr2btn">
     <div class="cmtblck">
       <div class="addtl">For assistance please contact your Account Rep below:</div>
          <?php print $acct_rep_info; ?>
       </div>
   </div>
    <?php endif; ?>
</div>

<?php if (isset($terms) && !empty($terms)): ?>
 <div class="pgbrk"></div>
<div class="printWrapper printContent">
   <?php print $terms;?>
</div>
<?php endif; ?>
  

