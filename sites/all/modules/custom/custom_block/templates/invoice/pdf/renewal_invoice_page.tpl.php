<?php
   global $base_url;  
if (count($seat_holds)): ?>
                    <?php
                    $total = $paid = $parking_fee = $handlind_fee = $resale_acc_credit = $sum_purchase_price = 0;
                    $seats_info = array(variable_get('sth_full_season', '14SEAS'), variable_get('sth_chasesuites', '14GSRO'));
                    $parking_info = array(variable_get('sth_parking_full', '14PARK'), variable_get('sth_PARKVP', '14PARKVP'), variable_get('sth_PRANG', '14PRANG'));
                      foreach ($seat_holds as $seats) {
                          if ($seats->event_name == variable_get('sth_handlingfee', '14HSTH')) {
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
                      } endif;?>
<div class="printWrapper">
<div class="printHeader">
   <div class="logo"><img src="sites/all/modules/custom/custom_block/images/print_logo.png" alt="CowBoy" title="CowBoy" /></div>
   <div class="printDate"><?php print date('m/d/y');?></div>
</div>
<div class="infoPanelLeft">                
                <div class="namePanel greyBorder topPanel">
                    <!--<div class="leftBorder greyColor"></div>-->
                    <div class="printHeading">2014 Season Ticket Invoice</div>
                    <h2><?php print isset($name) ? $name : ''; ?></h2>
                    <div class="accountDetail">Account#: <span class="blueTxt"><?php print isset($user_info->acct_id) ? $user_info->acct_id : ''; ?></span></div>
               
                    <div class="contDetail">
		      <?php print (isset($user_info->street_addr_1) && !empty($user_info->street_addr_1)) ? $user_info->street_addr_1 . ', ': '&nbsp;'; ?><?php
		        print (isset($user_info->city) && !empty($user_info->city)) ? $user_info->city . ', ' : '' ;
                        print (isset($user_info->state) && !empty($user_info->state)) ? $user_info->state . ', ' : '' ;
                        print (isset($user_info->zip) && !empty($user_info->zip)) ? $user_info->zip : '&nbsp;' ; ?>
		     </div>
                    <div class="contDetail">P: <?php print (isset($user_info->phone_day) && $user_info->phone_day) ? $user_info->phone_day : '&nbsp;'; ?></div>
                    <div class="contDetail">E: <?php print (isset($user_info->email) && $user_info->email) ? $user_info->email : ''; ?></div>
                </div>
                
                <div class="namePanel ticketTableHolder">
                    <div class="clearfix lightgreyBorder">
                    <div class="yearHeading">2014 Season Tickets</div>
                    <table class="ticketTable">
                        <tbody>
			  <tr class="head">
                            <td class="tqty">TQTY</td>
                            <td>SECTION</td>
                            <td>ROW</td>
                            <td>SEATS</td>
                            <td class="totalCost">TOTAL COST</td>
			    </tr>
			  <?php
			  $cnt = 0;
                            foreach ($seat_holds as $seats) {
                              if (isset($seats->seat_num) && $seats->seat_num && ((in_array($seats->event_name, $seats_info)) || (in_array($seats->event_name, $parking_info)))) { $cnt++;?>
				 <tr class="tableRow">
				   <td class="td1"><?php print isset($seats->num_seats) ? $seats->num_seats : "" ?></td>
				   <td class="td2"><?php print (isset($seats->section_name) && !empty($seats->section_name)) ? $seats->section_name : "" ?></td>
				   <td class="td3"><?php print (isset($seats->row_name) && !empty($seats->row_name)) ? $seats->row_name : "" ?></td>
				   <td class="td4"><?php print (isset($seats->seat_num) && !empty($seats->seat_num)) ? $seats->seat_num . '-' . $seats->last_seat : "" ?></td>
				   <td class="td5"><?php print isset($seats->invoice_gross_invoice_amount) ? invoice_money_format($seats->invoice_gross_invoice_amount) : "" ?></td>
				 </tr>
                        <?php }
			   }
			?>
                        </tbody>
		    </table>
                </div>
                </div>   
                <div class="namePanel contactInfoPanel blueBorder">
                    <div class="clearfix contactInfo">
                        <div class="contactInfoLeft">
                            <div>Total Tickets:</div>
                            <div>Parking:</div>
                            <div>Handling Fee:</div>
                            <div>Less Payments/Credits (-):</div>
                        </div>
                        <div class="contactInfoRight">                            
                            <div><?php print isset($total) ? invoice_money_format($total) : '';?></div>
                            <div><?php print isset($parking_fee) ? invoice_money_format($parking_fee) : '';?></div>
                            <div><?php print isset($handlind_fee) ? invoice_money_format($handlind_fee) : '';?></div>
                            <div><?php echo invoice_money_format($paid);?></div>
                        </div>
                    </div>
                    <div class="clearfix amountInfo">
                        <div class="amountInfoLeft" >
                            Total Amount Due:
                           
                        </div>  
                        <div class="amountInfoRight">
                          <?php echo invoice_money_format(($total + $parking_fee + $handlind_fee) - $paid);?>
                        </div>
                        
                    </div>
                </div>             
            </div>
            <div class="printFooter">
                <div class="mainHeading">Sacramento Kings</div>
                <div class="mainHeading"></div>
                <div class="address"></div>
                <div class="address"></div>
                <div class="address pull-left width45"></div><div class="address pull-right width45 text-right grayTxt"></div>
            </div>
            </div>
