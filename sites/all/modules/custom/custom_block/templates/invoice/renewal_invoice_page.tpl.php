<?php
   global $base_url;  
if (count($seat_holds)): ?>
                    <?php
                    $total = $paid = $parking_fee = $handlind_fee = $resale_acc_credit = $sum_purchase_price = 0;
		    $seats_info = array(variable_get('sth_full_season', '14SEAS'), variable_get('sth_chasesuites', '14GSRO'));
                    $parking_info = array(variable_get('sth_parking_full', '14PARK'), variable_get('sth_PARKVP', '14PARKVP'), variable_get('sth_PRANG', '14PRANG'));
                      foreach ($seat_holds as $seats) {
                          if ($seats->event_name == variable_get('sth_handlingfee', '14HSTH ')) {
                            $handlind_fee += $seats->invoice_gross_invoice_amount ? $seats->invoice_gross_invoice_amount : 5;
                          }
			  if ($seats->event_name == (in_array($seats->event_name, $parking_info))) {
                            $parking_fee += $seats->invoice_gross_invoice_amount;
                          }
                        if (isset($seats->seat_num) && $seats->seat_num && (in_array($seats->event_name, $seats_info))) {
                           $total += $seats->invoice_gross_invoice_amount;
                           $sum_purchase_price += ($seats->num_seats * $seats->purchase_price);
                            ?>
                        <?php }
			 $paid += $seats->paid_amount;
                      }  endif;
?>
  
<?php print (isset($user_info) && $user_info) ? $user_info : ''; ?>
<article class="amountduewrap">
<section class="clearfix amountdueinfo">
                        <div class="col-md-7">
                            <div>Total Ticket Amount:</div>
                            <div>Parking Amount:</div>
                            <div>Handling Fee:</div>
                            <div>Less Payments/Credits (-):</div>
                        </div>
                        <div class="contactInfoRight col-md-5">                            
                            <div><?php print isset($total) ? invoice_money_format($total) : '';?></div>
                            <div><?php print isset($parking_fee) ? invoice_money_format($parking_fee) : '';?></div>
                            <div><?php print isset($handlind_fee) ? invoice_money_format($handlind_fee) : '';?></div>
                            <div><?php echo invoice_money_format($paid);?></div>
                        </div>
                     </section>
<section class="clearfix amountduettl">
                        <div class="col-md-7">
                            <h2>AMOUNT DUE</h2>
			    <?php if ($payment_flag):?>
			      <div class="amount-disclaimer"> Payments recently made to this account may not be reflected in the Amount Due displayed.</div>
			    <?php endif;?>
                            <div class="deadlineTxt">Deadline for Payment: 5/1/14</div>
                        </div>  
                        <div class="amountInfoRight col-md-5">
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
    <th width="32%">TQTY</th>
    <th width="17%">SECTION</th>
    <th width="17%">ROW</th>
    <th width="17%">SEATS</th>
    <th width="17%">TOTAL COST</th>
  </tr>
  </thead>
  <?php
                            foreach ($seat_holds as $seats) {
                              if (isset($seats->seat_num) && $seats->seat_num && ((in_array($seats->event_name, $seats_info)) || (in_array($seats->event_name, $parking_info)))) { ?>
  <tbody>
  <tr>
    <td><?php print isset($seats->num_seats) ? $seats->num_seats : "" ?></td>
    <td><?php print (isset($seats->section_name) && !empty($seats->section_name)) ? $seats->section_name : "" ?></td>
    <td><?php print (isset($seats->row_name) && !empty($seats->row_name)) ? $seats->row_name : "" ?></td>
    <td><?php print (isset($seats->seat_num) && !empty($seats->seat_num)) ? $seats->seat_num . '-' . $seats->last_seat : "" ?></td>
    <td><?php print isset($seats->invoice_gross_invoice_amount) ? invoice_money_format($seats->invoice_gross_invoice_amount) : "" ?></td>
  </tr>
            <?php }
			   }
			?>
  </tbody>
</table>

                  
                    <div class="pull-right">
            <a data-title="Email Invoice" class="btn btn-default email" href="/invoice/email">Email <i class="fa fa-chevron-right"></i></a>
			<a data-title="Print Invoice" class="btn btn-default btnprint" href="/invoice/print" target="_blank">Print <i class="fa fa-chevron-right"></i></a>  
                    </div>
                </section>                
            
