<?php  global $base_url; ?>  
<?php print (isset($user_info) && $user_info) ? $user_info : ''; ?>
<article class="amountduewrap">
  <section class="clearfix amountdueinfo">
      <div class="col-md-7 col-xs-7">
	  <div>Total Ticket Amount:</div>
	  <div>Parking Amount:</div>
	  <div>Handling Fee:</div>
	  <div>Less Payments/Credits (-):</div>
      </div>
      <div class="contactInfoRight col-md-5 col-xs-5">                            
	  <div><?php print isset($total) ? $total : '$0';?></div>
	  <div><?php print isset($parking_fee) ? $parking_fee : '$0';?></div>
	  <div><?php print isset($handling_fee) ? $handling_fee : '$0';?></div>
	  <div><?php echo $paid;?></div>
      </div>
   </section>
   <section class="clearfix amountduettl">
      <div class="col-md-7 col-xs-7">
	  <h2>AMOUNT DUE</h2>
	  <?php if ($payment_flag):?>
	    <div class="amount-disclaimer"> Payments recently made to this account may not be reflected in the Amount Due displayed.</div>
	  <?php endif;?>
	  <div class="deadlineTxt">Deadline for Payment: 5/1/14</div>
      </div>  
      <div class="amountInfoRight col-md-5 col-xs-5">
	    <h2><?php echo $due_amount;?></h2>
      </div>
      <div class="pull-right">
         <a data-title="Click To Pay" href="/invoice/payment" class="btn btn-default btn-light">Pay <i class="fa fa-chevron-right"></i></a>     
       </div>
   </section>
</article>
<section class="season-invoice-detail">
  <h3>INVOICE DETAILS</h3>
    <?php print $invoice_table;?>
    <div class="pull-right">
<a data-title="Email Invoice" class="btn btn-default email" href="/invoice/email">Email <i class="fa fa-chevron-right"></i></a>
	<a data-title="Print Invoice" class="btn btn-default btnprint" href="/invoice/print" target="_blank">Print <i class="fa fa-chevron-right"></i></a> 
    </div>
</section>                
            