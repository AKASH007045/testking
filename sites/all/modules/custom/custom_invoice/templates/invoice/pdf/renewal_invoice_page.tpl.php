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
		$wfid = variable_get('invoice_watermark_fid', '');
		$watermark = '';
		if ($wfid != '') {
			$file = file_load($wfid);
			if ($file && isset($file->fid)) {
       $watermark = file_create_url($file->uri);
      }
		}
      $team = variable_get('invoice_team_name', t('STP'));
			$color = variable_get('invoice_color', t('#000'));
			 $footer1 =  variable_get('invoice_footer', array());
			 $footer =  isset($footer1['value']) ? $footer1['value'] : '';
     ?>
<style>
	 .printWrapper{
			background:url(<?php print $watermark; ?>) center center no-repeat;
			/*background:transparent url(<?php print $watermark; ?>) repeat fixed right top;*/
			
       }
     .printHeader{ border-bottom:5px solid <?php print $color; ?>;}
     .head-cnt-mid .teamname{ color:<?php print $color; ?>; }
     .bdr5lft{ border-left:5px solid <?php print $color; ?>; }
     .lftblck .actinf, .rgtblck .actinf {color:<?php print $color; ?>;}
     .rgtblck .ttlamtd .tlyl {color:<?php print $color; ?>;}
     .chrt-dtl tr.head td{color:<?php print $color; ?>;}
</style>
<div class="printWrapper">
  <div class="printHeader">
    <div class="logo"><img src="<?php print $logo== "" ? 'sites/all/modules/custom/custom_block/images/print_logo.png' : $logo ?>" alt="<?php print $team ?>" title="<?php print $team ?>" /></div>
    <div class="head-cnt-mid">
      <div class="teamname"><?php print $team ?></div>
      <div class="teamdtl">2015-16 MEMBERSHIP INVOICE</div>
      <div class="teamdtl"><?php print $invoice_id; ?></div>
    </div>
    <div class="head-cnt-rgt">
      <div class="date-time">Invoice Date: <?php print date('l, F d, Y');?></div>
     <!-- <div class="date-tkt"><em>Renew by Feb 27 to get your season tickets!</em></div>-->
    </div>
  </div>
  <div class="wrp-main bdr3btn ">
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
    <div class="lftblck"> <div class="actinf">Invoice Details</div> </div>
    

 <div class="tbl-chart"> <?php print $invoice_table;?></div>
 
 
 
  </div>
   <div class="wrp-main bdr2btn">
    <div class="lftblck">
      <div class="actinf"></div>
    </div>
    <div class="cmtblck">
      <div class="addtl"></div>
      <div class="addtl"></div>
    </div>
    <div class="rgtblck bdr5lft">
     <div class="tcwrp"><div class="tcnm">Total Invoice Amount:</div> <div class="tcamt"><?php print isset($total) ? $total : '$0';?></div></div>
      <!--<div class="tcwrp"><div class="tcnm">Parking:</div> <div class="tcamt"><?php print isset($parking_fee) ? $parking_fee : '$0';?></div></div>
       <div class="tcwrp"><div class="tcnm">Handling Fee:</div> <div class="tcamt"><?php print isset($handling_fee) ? $handling_fee : '$0';?></div></div>-->
        <div class="tcwrp bdr3btn pdg5mrg"><div class="tcnm">Payments/Credits (-):</div> <div class="tcamt">-(<?php echo $paid;?>)</div></div>
        <div class="tcwrp"><div class="tcnm fnt16glb">Total Amount Due:</div> <div class="tcamt actinf"><?php echo $due_amount;?></div></div>
    </div>
  </div>
    <div class="wrp-main bdr2btn">
    <div class="lftblck">
      <div class="actinf">&nbsp;</div>
    </div>
    <div class="cmtblck">
      <div class="addtl">For assistance please contact your Account Rep below:</div>
      <?php print $acct_rep_info; ?>
    </div>
  </div>
  <div class="wrp-main fnt12ftr pgbrk"> Sacramento Kings &nbsp;I&nbsp; Sleep Train Arena &nbsp;I&nbsp; One Sports Parkway &nbsp;I&nbsp; Sacramento, CA 95834 &nbsp;I&nbsp; 916.928.0000 &nbsp;I&nbsp; www.kingsmembership.com  </div> 
</div>
  <!----------------------------------------Next Page start here---------------------------------------> 
 <!-- <div class="printWrapper">
   <div class="printHeader">
    <div class="logo"><img src="<?php print $logo== "" ? 'sites/all/modules/custom/custom_block/images/print_logo.png' : $logo ?>" alt="<?php print $team ?>" title="<?php print $team ?>" /></div>
    <div class="head-cnt-mid">
      <div class="teamname"><?php print $team ?></div>
      <div class="teamdtl">Terms & Conditions</div>
      <div class="teamdtl"><?php print $invoice_id; ?></div>
    </div>
    <div class="head-cnt-rgt">
      <div class="date-time">Invoice Date: <?php print date('l, F d, Y');?></div>
      <div class="date-tkt"><em>Renew by Feb 27 to get your season tickets!</em></div>
    </div>
  </div>
     <?php// print $footer; ?>
  </div>-->
  

