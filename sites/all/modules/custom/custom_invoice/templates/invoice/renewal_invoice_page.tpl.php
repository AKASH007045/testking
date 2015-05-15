<?php
   global $base_url;  
if (count($seat_holds)): ?>
   <?php
   $total = $paid = $parking_fee = $handlind_fee = $resale_acc_credit = 0;
   $seats_info = array(variable_get('sth_full_season', '14SEAS'), variable_get('sth_chasesuites', '14GSRO'));
   $parking_info = array(variable_get('sth_parking_full', '14PARK'), variable_get('sth_PARKVP', '14PARKVP'), variable_get('sth_PRANG', '14PRANG'));
   foreach ($seat_holds as $seats) {
         $event_name = ($seats->event_name && !is_null($seats->event_name))? $seats->event_name : $seats->item_name;
     if ($event_name == variable_get('sth_handlingfee', '14HSTH')) {
       //$handlind_fee += $seats->invoiced_amount ? $seats->invoiced_amount : 5;
     }
     elseif ($event_name == (in_array($seats->event_name, $parking_info))) {
       //$parking_fee += $seats->invoiced_amount;
     }
     else {
      $total += $seats->invoiced_amount;
     }
     $paid += $seats->paid_amount;
   }  
   endif;
  ?>
  
<?php print (isset($user_info) && $user_info) ? $user_info : ''; ?>
<article class="amountduewrap">
<section class="clearfix amountdueinfo">
                        <div class="col-sm-6 col-md-6">
                            <div>Total Ticket Amount:</div>
                            <!--<div>Parking Amount:</div>
                            <div>Handling Fee:</div>-->
                            <div>Less Payments/Credits (-):</div>
                        </div>
                        <div class="contactInfoRight col-sm-6 col-md-6">                            
                            <div><?php print isset($total) ? invoice_money_format($total) : '';?></div>
                            <!--<div><?php print isset($parking_fee) ? invoice_money_format($parking_fee) : '';?></div>
                            <div><?php print isset($handlind_fee) ? invoice_money_format($handlind_fee) : '';?></div>-->
                            <div><?php echo invoice_money_format($paid);?></div>
                        </div>
                     </section>
<section class="clearfix amountduettl">
                        <div class="col-sm-6 col-md-6">
                            <h2>AMOUNT DUE</h2>
			    <?php if (isset($payment_flag) && $payment_flag):?>
			      <div class="amount-disclaimer"> Payments recently made to this account may not be reflected in the Amount Due displayed.</div>
			    <?php endif;?>
                            <!--<div class="deadlineTxt">Deadline for Payment: 5/1/14</div>-->
                        </div>  
                        <div class="amountInfoRight col-sm-6 col-md-6">
                              <h2><?php echo invoice_money_format(($total + $parking_fee + $handlind_fee) - $paid);?></h2>
                        </div>
                        <div class="pull-right">
   <a data-title="Click To Pay" href="/invoice/payment" class="btn btn-default btn-light">Pay <i class="fa fa-chevron-right"></i></a>     
                         </div>
                    </section>
                    
                   
                    </article>
                <section class="season-invoice-detail">
                  <h3>INVOICE DETAILS</h3>
                    <table class="ticketTable" border="0" cellspacing="0" cellpadding="0">
                    <thead>
  <tr>
    <th width="20%">EVENT</th>
    <th width="12%">TQTY</th>
    <th width="17%">SECTION</th>
    <th width="17%">ROW</th>
    <th width="17%">SEATS</th>
    <th width="17%">TOTAL COST</th>
  </tr>
  </thead>
  <?php
   foreach ($seat_holds as $seats) {
      $event_name = ($seats->event_name && !is_null($seats->event_name))? $seats->event_name : $seats->item_name;
      if ($event_name != variable_get('sth_handlingfee', '14HSTH ')) { ?>
  <tbody>
  <tr>
    <td><?php print isset($seats->item_name_long) ? $seats->item_name_long : $seats->event_name; ?></td>
    <td><?php print isset($seats->num_seat) ? $seats->num_seat : "" ?></td>
    <td><?php print (isset($seats->section_name) && !empty($seats->section_name)) ? $seats->section_name : "" ?></td>
    <td><?php print (isset($seats->row_name) && !empty($seats->row_name)) ? $seats->row_name : "" ?></td>
    <td><?php print (isset($seats->seat_num) && !empty($seats->seat_num)) ? $seats->seat_num . '-' . $seats->last_seat : "" ?></td>
    <td><?php print isset($seats->invoiced_amount) ? invoice_money_format($seats->invoiced_amount) : "" ?></td>
  </tr>
            <?php }
			   }
			?>
  </tbody>
</table>

                  
                    <div class="pull-right">
                    <?php print $pdf_link; ?>
            <a data-title="Email Invoice" class="btn btn-default email" href="/invoice/email">Email <i class="fa fa-chevron-right"></i></a>
			<a data-title="Print Invoice" class="btn btn-default btnprint" href="/invoice/print" target="_blank">Print <i class="fa fa-chevron-right"></i></a>  
                    </div>
                </section>                
            
