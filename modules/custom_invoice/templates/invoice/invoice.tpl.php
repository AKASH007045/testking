<?php  global $base_url; ?>  
<?php print (isset($user_info) && $user_info) ? $user_info : ''; ?>
<article class="amountduewrap">
  <div class="col-xs-5"><div class="row"><?php print $invoice_other_info_text;?>&nbsp;</div></div><div class="col-xs-1"></div>
  <div class="col-xs-6 no-padding">
    <section class="clearfix amountdueinfo">
      <div class="col-xs-7">
        <?php print $total_label;?>
        <?php print $parking_label;?>
        <?php print $handling_label;?>
        <?php print $credits_label;?>
      </div>
      <div class="contactInfoRight col-xs-5 text-right">  
	  <?php echo $total;?>
	  <?php print $parking_fee; ?>
	  <?php print $handling_fee;?>
	  <?php echo $paid;?>
      </div>
    </section>
    <section class="clearfix amountduettl">
      <div class="col-xs-7">
        <h2>AMOUNT DUE</h2>
        <div class="deadlineTxt"><?php print $invoice_detail_desc;?></div>
      </div>  
      <div class="amountInfoRight col-xs-5 text-right">
        <h2><?php echo $due_amount;?></h2>
      </div>
      <div class="pull-right">
        <a data-title="Click To Pay" href="<?php print $pay_btn;?>" class="btn btn-default btn-pay btn-light">Pay <i class="fa fa-chevron-right"></i></a> 
      </div>
    </section>
  </div>
</article>
<section class="season-invoice-detail">
  <h3>INVOICE DETAILS</h3>
  <?php print $invoice_table;?>
  <div class="col-xs-12">
    <div class="row">
      <div class="col-xs-8"><div class="row"><?php print $invoice_text;?></div></div>
      <div class="col-xs-1">&nbsp;</div>
      <div class=" pull-right">
        <a data-title="Email Invoice" class="btn btn-default email" href="/invoice/<?php print $invoice_id;?>/email/<?php print $stp_inv_id?>">Email <i class="fa fa-chevron-right"></i></a>
        <a data-title="Print Invoice" class="btn btn-default btnprint" href="/invoice/<?php print $invoice_id;?>/print/<?php print $stp_inv_id?>" target="_blank">Print <i class="fa fa-chevron-right"></i></a>
      </div>
    </div>
  </div>
  <!--<div class="pull-right">
    <div class="col-xs-10"><?php print $invoice_text;?></div>
    <div class="col-xs-2">
      <a data-title="Email Invoice" class="btn btn-default email" href="/invoice/<?php print $invoice_id;?>/email/<?php print $stp_inv_id?>">Email <i class="fa fa-chevron-right"></i></a>
      <a data-title="Print Invoice" class="btn btn-default btnprint" href="/invoice/<?php print $invoice_id;?>/print/<?php print $stp_inv_id?>" target="_blank">Print <i class="fa fa-chevron-right"></i></a>
    </div>
  </div>-->
</section>                            