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
	   ?>
       <?php }
	$paid += $seats->paid_amount;
     }  endif;
?>
<div class="printWrapper">
<div class="printHeader">
   <div class="logo"><img src="sites/all/modules/custom/custom_block/images/print_logo.png" alt="Phoenix Suns" title="Phoenix Suns" /></div>
   <div class="printDate"><?php print date('m/d/y');?></div>
</div>
<div class="infoPanelLeft">                
                <div class="namePanel greyBorder topPanel">
                    <!--<div class="leftBorder greyColor"></div>-->
                    <div class="printHeading">2015 Season Ticket Invoice</div>
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
                    <div class="yearHeading">2015 Season Tickets</div>
                    <table class="ticketTable">
                        <tbody>
			  <tr class="head">
          <td class="tqty">EVENT</td>
          <td>TQTY</td>
          <td>SECTION</td>
          <td>ROW</td>
          <td>SEATS</td>
          <td class="totalCost">TOTAL COST</td>
			    </tr>
			  <?php
			  $cnt = 0;
                            foreach ($seat_holds as $seats) {
                              $event_name = ($seats->event_name && !is_null($seats->event_name))? $seats->event_name : $seats->item_name;
      if ($event_name != variable_get('sth_handlingfee', '14HSTH ')) { $cnt++;?>
				 <tr class="tableRow">
           <td class="td1"><?php print isset($seats->item_name_long) ? $seats->item_name_long : $seats->event_name; ?></td>
				   <td class="td2"><?php print isset($seats->num_seat) ? $seats->num_seat : "" ?></td>
				   <td class="td3"><?php print (isset($seats->section_name) && !empty($seats->section_name)) ? $seats->section_name : "" ?></td>
				   <td class="td2"><?php print (isset($seats->row_name) && !empty($seats->row_name)) ? $seats->row_name : "" ?></td>
				   <td class="td4"><?php print (isset($seats->seat_num) && !empty($seats->seat_num)) ? $seats->seat_num . '-' . $seats->last_seat : "" ?></td>
				   <td class="td5"><?php print isset($seats->invoiced_amount) ? invoice_money_format($seats->invoiced_amount) : "" ?></td>
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
                            <!--<div>Parking:</div>
                            <div>Handling Fee:</div>-->
                            <div>Less Payments/Credits (-):</div>
                        </div>
                        <div class="contactInfoRight">                            
                            <div><?php print isset($total) ? invoice_money_format($total) : '';?></div>
                            <!--<div><?php print isset($parking_fee) ? invoice_money_format($parking_fee) : '';?></div>
                            <div><?php print isset($handlind_fee) ? invoice_money_format($handlind_fee) : '';?></div>-->
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
                <div class="mainHeading">Sleep Train Arena</div>
                <div class="address">One Sports Parkway</div>
                <div class="address">Sacramento, CA 95834</div>
            </div>
            </div>
