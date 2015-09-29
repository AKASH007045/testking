<?php
/**
 * @file
 */

/**
* function to send invoice email to users
*/
function custom_invoice_email() {
  global $user;
  $acct_id = $user->name;
  $name = get_user_full_name($user);
  if (isset($acct_id)) {
    $seat_holds = get_seats_hold($acct_id);
    $playoff_data = get_playoff_data($acct_id);
    if (!count($seat_holds) && !count($playoff_data)) {
      drupal_set_message(t('Invoice data not avaiable'), 'warning');
      drupal_goto('invoice');
    }
  }
  module_load_include('inc', 'custom_block', 'custom_block.block'); 
  $output = _tmapi_user_invoice('pdf');
  /*************  For Tickets STPDEV-21, STPDEV-455 and STPDEV-526     ********/
    $wfid = variable_get('invoice_watermark_fid', '');
    $watermarkImage = '';
    if($wfid != '') 
    {
	$file = file_load($wfid);
	if($file && isset($file->fid))
        {
            $watermarkImage = file_create_url($file->uri);
        }
   }
  /*************  End For Tickets STPDEV-21, STPDEV-455 and STPDEV-526     *****/
  $params = array(
    'name' => $user->name,
    'subject' => variable_get('invoice_mail_subject', t('2015 Sacramento Kings Invoice')),
    'body'=> variable_get('invoice_mail_body', t('Attached to this email is a copy of your invoice for the 2015 Sacramento Kings Season Subscription.')),
  );

  // Checking mPDF library existence.
  if (pdf_using_mpdf_library_exist() == TRUE) {
  //user variable_get for sitename
    $filename = "Kings-Invoice_" . str_replace(' ', '-', $name);
    _pdf_using_mpdf_generator($output, $filename, 'F', $name, $watermarkImage);
    $file=file_directory_temp() . '/' . $filename.".pdf";
    $params['file'] = $file;
    $params['filename'] = $filename . '.pdf';
  }

  $mail= $user->mail;
  drupal_mail('custom_hooks', 'triggerkey', $mail, language_default(), $params);
  drupal_set_message('Invoice mailed successfully.');
  drupal_goto('invoice');
  return '';
}
/**
* function to print invoice
*/
function custom_invoice_print() {
  global $user, $conf;
  $acct_id = $user->name;
  $name = get_user_full_name($user);
  if (isset($acct_id)) {
    $seat_holds = get_seats_hold($acct_id);
    //$playoff_data = get_playoff_data($acct_id);
    if (!count($seat_holds)) {
      drupal_set_message(t('Invoice data not avaiable'), 'warning');
      drupal_goto('invoice');
    }
  }
  /*************  For Tickets STPDEV-21, STPDEV-455 and STPDEV-526     ********/
    $wfid = variable_get('invoice_watermark_fid', '');
    $watermarkImage = '';
    if($wfid != '') 
    {
	$file = file_load($wfid);
	if($file && isset($file->fid))
        {
            $watermarkImage = file_create_url($file->uri);
        }
   }
  /*************  End For Tickets STPDEV-21, STPDEV-455 and STPDEV-526     *****/
  $output = _tmapi_user_invoice('pdf');
  // Checking mPDF library existence.
 
  if (pdf_using_mpdf_library_exist() == TRUE) {
    $filename = "kings-Invoice_" . str_replace(' ', '-', $name);
    $conf['pdf_using_mpdf_pdf_save_option'] = 0;
    _pdf_using_mpdf_generator($output, $filename, 'I', $name, $watermarkImage);
    exit;
  }
  return '';
}

function custom_invoice_page() {
  return _tmapi_user_invoice();
}

/**
 * Invoice Page Block
 */
function _tmapi_user_invoice($pdf = NULL) {
  drupal_add_css(drupal_get_path('module', 'custom_invoice') . '/css/invoice.css');
  module_load_include('inc', 'tm_api', 'tm_api.helper');
  global $user;
  $acct_id = $user->name;
  $cnt = 0;
  $tmapi_invoice = tmapi_get_sth_invoice($acct_id);
  if (isset($acct_id)) {
    $plans = get_plans($acct_id, $invoice_id);
    $seat_holds = get_seats_hold($acct_id);
    $payment_flag = get_tm_payment_flag($acct_id);
    $user_info = invoice_user_info_block();
    $invoice_id = 0;
    $output = '';
    
	 /*
    * Kings Specific - Add pricing link on invoice page
    
    $pdf_link = 'sites/default/files/STH_15-16_RENEWAL_NEW_SeatingPricing_8.pdf';
    if(_price_code_check()){
      $pdf_link = 'sites/default/files/STH_15-16_RENEWAL_SeatingPricing_8.pdf';
    }
    $link = l(
      t('CLICK FOR PRICING MAP'), 
      $pdf_link,      
      array(
        'attributes' => array(
          'class' => array('pricing-map'),
          'target' => "_blank"
        )
      )
    );
    /*
    * Pricing link generated
    */
	
	
	
    if (count($seat_holds)):
      //
      $total = $paid = $parking_fee = $handling_fee = 0;
      $rows = array();
      $row[] = array('data' => 'TQTY', 'type' => 'field', 'class' => 'tqty');
      $row[]=  array('data' => 'DESCRIPTION', 'class' => 'decr');
      $row[]=array('data' => 'SECTION');
      $row[]=array('data' => 'ROW');
      $row[]=array('data' => 'SEATS');
      $row[]= array('data' => 'TOTAL COST', 'class' => 'totalCost');
      $rows[] = array('data' => $row, 'class' => 'head');
      foreach ($seat_holds as $seats) {
        $row = array();
        $event_name = ($seats->item_name_long && !is_null($seats->item_name_long))? $seats->item_name_long : $seats->item_name;
        $event_name1 = strtolower($event_name);
		//create parking variable and use here
        $pos = strpos($event_name1, variable_get('sth_parking_full', 'parking'));
        $h_fee = strpos($event_name1, variable_get('sth_handlingfee', 'handling'));
        $invoice_id = (isset($invoice_id) && $invoice_id) ? $invoice_id : $seats->invoice_id;
        if ($pos!== false) {
          $parking_fee += $seats->invoiced_amount;
        }
        elseif($h_fee!==false) {
          $handling_fee += $seats->invoiced_amount;
        }
        else {
          $total += $seats->invoiced_amount;
        }
        $paid += $seats->paid_amount;
        $row['num_seat'] = array('data' => (isset($seats->num_seat) ? $seats->num_seat : ""), 'class' => 'td1');
        $row['event_name'] = array('data' => ((isset($seats->item_name_long) && $seats->item_name_long) ? $seats->item_name_long : $seats->event_name), 'class' => 'td2');
        $row['section_name'] = array('data' => ((isset($seats->section_name) && !empty($seats->section_name)) ? $seats->section_name : ""), 'class' => 'td3');
        $row['row_name'] = array('data' => ((isset($seats->row_name) && !empty($seats->row_name)) ? $seats->row_name : ""), 'class' => 'td4');
        $row['seat_num'] = array('data' => ((isset($seats->seat_num) && !empty($seats->seat_num)) ? $seats->seat_num . '-' . $seats->last_seat : ""), 'class' => 'td5');
        $row['invoiced_amount'] = array('data' => (isset($seats->invoiced_amount) ? invoice_money_format($seats->invoiced_amount) : ""), 'class' => 'td6');
        $rows[] = array('data' => $row, 'class' => 'tbr');
      }
      //$payment_plans = tmapi_get_payment_plans();
      //$due_amount = ($total + $parking_fee + $handling_fee) - $paid;
	   $due_amount = ($total) - $paid;
      $invoice_table = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('ticketTable chrt-dtl')), 'sticky' => FALSE));
      if ($pdf) {
        $user_info = invoice_user_info_block(2);
        $acct_rep_info = custom_block_account_rep('block_1');
        $output .= theme('renewal_invoice_' . $pdf, array('seat_holds' => $seat_holds, 'user_info' => $user_info, 'acct_rep_info' => $acct_rep_info, 'cnt' => $cnt, 'payment_flag' => $payment_flag, 'invoice_id' => $invoice_id, 'invoice_table' => $invoice_table, 'total' => invoice_money_format($total), 'paid' => invoice_money_format($paid), 'parking_fee' => invoice_money_format($parking_fee), 'handling_fee' => invoice_money_format($handling_fee), 'due_amount' => invoice_money_format($due_amount)));
      } else {
        $output .= theme('renewal_invoice', array('seat_holds' => $seat_holds, 'user_info' => $user_info, 'cnt' => $cnt, 'payment_flag' => $payment_flag, 'invoice_table' => $invoice_table, 'total' => invoice_money_format($total), 'paid' => invoice_money_format($paid), 'parking_fee' => invoice_money_format($parking_fee), 'handling_fee' => invoice_money_format($handling_fee), 'due_amount' => invoice_money_format($due_amount), 'pdf_link' => $link));
      }
    endif;
    return theme('invoice_page', array('output' => $output));
  }
  return '';
}

function custom_invoice_payment_request() {
  global $user;
  module_load_include('inc', 'tm_api', 'tm_api.helper');
  drupal_add_css(drupal_get_path('module', 'custom_invoice') . '/css/invoice.css');
  drupal_add_js(drupal_get_path('module', 'custom_invoice') . '/js/plan.js');
  $user_info = invoice_user_info_block();
  $form = array();
  if ($user->uid) {
    $acct_id = $user->name;
    if ($acct_id) {
      $user_name = get_user_full_name($user);
      $get_amount_info = get_tm_user_amount_info($acct_id);
    }
  }
  $form['#attributes']['autocomplete'] = 'off';
  $form['tmuser_info'] = array(
    '#type' => 'hidden',
    '#value' => array('name' => $user_name, 'acct_id' => $acct_id, 'invoice_id' => isset($get_amount_info['invoice_id']) ? $get_amount_info['invoice_id'] : '' , 'due_amt' => isset($get_amount_info['due_amt']) ? $get_amount_info['due_amt'] : ''),
  );
  $form['acct_info'] = array(
    '#type' => 'markup',
    '#markup' => $user_info,
    '#weight' => -10,
  );
  //$inv_detail will have the detail of the 
  //selected invoice from the invoice_list call to check for the payment plan id
  $inv_detail = array();
  if(isset($_SESSION['invoice_detail']) && !empty($_SESSION['invoice_detail'])){
    $inv_detail = $_SESSION['invoice_detail'];
  }
  $invoice_id = $get_amount_info['invoice_id'];
  $plans = get_plans($acct_id, $invoice_id);
  //filter plans
  $plans = filter_payment_plans($acct_id, $invoice_id, $plans);
  
  foreach($plans as $plan) {
    //Only select active plans
    $options_plans[$plan['payment_plan_id']] = isset($plan['plan_long_name']) ? $plan['plan_long_name'] : $plan['plan_name'];
  }
  if (isset($inv_detail['payment_plan_id']) && !empty($inv_detail['payment_plan_id'])) {
    $form['payment_plans_title'] = array(
      '#type' => 'markup',
      '#prefix' => '<div class="col-sm-12 payment-user-form-wrapper"><div class="col-lg-1">&nbsp;</div><div class="col-sm-4 col-lg-4 personal-info name-panel">',
      '#markup' => t('<label>Selected Payment Plan</label>')
    );
    
    $form['payment_plans'] = array(
      '#type' => 'markup',
      '#markup' => isset($options_plans[$inv_detail['payment_plan_id']])?'<div class="payplan">'.$options_plans[$inv_detail['payment_plan_id']].'</div></br><div class="form-item-payment-plan-id"><div class="DivSelectyze"></div></div>':"No plans are associated with this account",
      '#suffix' => '</div>',
    );
    
    $form['payment_plan_id'] = array(
      '#type' => 'hidden',
      '#value' => isset($inv_detail['payment_plan_id']) ? $inv_detail['payment_plan_id'] : "",
      '#attributes' => array('id'=>'payment_plan_id')
    );
  }
  else{
    $form['payment_plan_id'] = array(
      '#type' => 'select',
      '#prefix' => '<div class="col-sm-12 payment-user-form-wrapper"><div class="col-lg-1">&nbsp;</div><div class="col-sm-4 col-lg-4 personal-info name-panel">',
      '#options' => $options_plans,
      '#title' => t('Select Payment Plan'),
      '#suffix' => '</div>',
      '#attributes' => array('id'=>'payment_plan_id')
    );
  }
   $form['ctype'] = array(
    '#type' => 'select',
    '#title' => t('Card Type'),
    '#options' => array('VI' => 'Visa', 'MC' => 'MasterCard', 'AE' => 'American Express', 'DI' => 'Discover'),
    '#required' => TRUE,
    '#prefix' => '<div class="col-lg-1">&nbsp;</div><div class="col-sm-6 col-lg-5 personal-info card-panel">',
    '#attributes' => array('tabindex' => '7')
  );
  $form['cc_num'] = array(
    '#type' => 'textfield',
    '#title' => t('Card Number'),
    '#required' => TRUE,
    '#maxlength' => 16,
    '#element_validate' => array('element_validate_integer_positive'), 
    '#attributes' => array('tabindex' => '8')
  );
  $form['exp_date'] = array(
    '#type' => 'textfield',
    '#title' => t('Expiration Date'),
    '#maxlength' => 4,
    '#element_validate' => array('element_validate_integer_positive'),
    '#required' => TRUE,
    '#attributes' => array('tabindex' => '9', 'placeholder' => 'mmyy')
  );
  $form['cin'] = array(
    '#type' => 'textfield',
    '#title' => t('Card Verification Number'),
    '#required' => TRUE,
    '#maxlength' => 4,
    '#attributes' => array('tabindex' => '10')
  );
  $form['email_confirmation'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="conf-email">' . t('Enter your email to receive payment confirmation') . '</div>',
  );
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email:'),
    '#required' => TRUE,
    '#maxlength' => 100,
    '#suffix' => '</div></div>',
    '#attributes' => array('tabindex' => '12')
  );
  $form['due_amount'] = array(
    '#type' => 'markup',
    '#prefix' => '<div class="col-sm-12 payment-amount-box"><div class="col-sm-6 col-lg-6">&nbsp</div><div class="col-sm-6 col-lg-5 amount-due">',
    '#markup' => '<label>' . t('Total Amount Due:') . '</label><div class="ttlamt">' . invoice_money_format($get_amount_info['due_amt']) . '</div>',
    '#suffix' => '',
  );
   $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => 'Enter Amount:',
    '#prefix' => '<div class="make-payment-wrapper">',
    '#required' => TRUE,
    '#maxlength' => 15,
    '#attributes' => array('tabindex' => '13')
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Make Payment <i class="fa fa-chevron-right"></i>'),
    '#attributes' => array('class' => 'btn-light'),
    '#suffix' => '</div></div></div>',
    '#attributes' => array('tabindex' => '14')
  );
 
  return $form;
}

function custom_invoice_payment_request_validate(&$form, &$form_state) {
  $ctype = array('VI' => 4, 'MC' => 5, 'AE' => 3, 'DI' => 6);
  $clength = array('VI' => 16, 'MC' => 16, 'AE' => 15, 'DI' => 16);
  $ccin = array('VI' => 3, 'MC' => 3, 'AE' => 4, 'DI' => 3);
  $data = $form_state['values'];
  $type = (isset($data['ctype']) && $data['ctype']) ? $data['ctype'] : '';
  $exp_valid_year = date('y');
  $exp_valid_month = date('m');
  $data_exp_year = substr($data['exp_date'], -2, 2);
  $data_expr_month = substr($data['exp_date'], 0, 2);
  $month_check = ($exp_valid_month <= $data_expr_month) ? 'true' : 'false';
  // Credit card number validation
  if ((isset($data['cc_num']) && $data['cc_num']) && $type) {
    if ((strlen($data['cc_num']) != $clength[$type]) || is_float($data['cc_num']) || (strlen((int)($data['cc_num'])) < $clength[$type])) {
      form_set_error('cc_num', 'Invalid Card Number');
    }
    elseif ($data['cc_num'] && (substr($data['cc_num'], 0, 1) != $ctype[$type])) {
      form_set_error('cc_num', 'Invalid Card Number');
    }
  }
  //  CIN number validation
  if (isset($data['cin']) && $data['cin'] && $type) {
    if (!preg_match('/^[0-9]{'. $ccin[$type].'}$/', $data['cin'])) {
      //use {3} for non-AMEX cards 
      form_set_error('cin', 'Invalid Card verification Number');
    }
  }
  
  // check for amount value
  if (isset($data['amount']) && $data['amount']) {
    if ($data['amount'] < $_POST['amount_due_today']) {
      form_set_error('amount', 'Entered Amount must be greater than or equal to Amount Due Today.');
    }
    if (!is_numeric($data['amount']) || $data['amount'] < 0 || strstr($data['amount'], ' ')) {
      form_set_error('amount', t('Invalid amount entered. Please enter numeric digits only e.g. 1234.50'));
    }
  }
  // Email validation
  if (!valid_email_address($data['email'])) {
    form_set_error('email', t('The e-mail address %mail is not valid.', array('%mail' => $data['email'])));
  }
  // Expiry Date validation
  if (isset($data['exp_date']) && $data['exp_date']) {
    if (strlen($data['exp_date']) < 4) {
      form_set_error('exp_date', 'Invalid Expiration Date.');
    }
    if (($data_exp_year < $exp_valid_year) || ($data_expr_month > 12) || ($data_expr_month < 1) || strstr($data['exp_date'], ' ')) {
      form_set_error('exp_date', 'Invalid Expiration Date.');
    }
    if (($data_exp_year == $exp_valid_year) && ($month_check == 'false')) {
      form_set_error('exp_date', 'Invalid Expiration Date.');
    }
  }
}

/**
 * Drupal form submit handler.
 */
function custom_invoice_payment_request_submit(&$form, &$form_state) {
  module_load_include('inc', 'tm_api', 'tm_api.helper');
  global $user;
  $site_mail = variable_get('site_mail', ini_get('sendmail_from'));
  $data = $form_state['values'];
  $response = array();
  watchdog('tmapi_payment', 'TM Payment request API for ACCT_ID. %data', array('%data' => $data['tmuser_info']['acct_id']), WATCHDOG_NOTICE, NULL);
  $tm_response = _tmapi_payment_request($data);
  $cc_num = '************'. substr($data['cc_num'], -4, 4);
  $data['tm_raw_response'] = '';
    
  if (isset($tm_response['response_ok']) && $tm_response['response_ok']) {
    $response_data =  (isset($tm_response['response']) && $tm_response['response']) ? $tm_response['response'] : '';
    $data['tm_raw_response'] = $response_data;
    $response = drupal_json_decode($response_data);
    $data['cc_num'] = substr($data['cc_num'], -4, 4);
    $data['tm_response_cc_captured'] = isset($response['command1']['mop']['0']['cc_captured']) ? $response['command1']['mop']['0']['cc_captured'] : 'N';
    $data['tm_cc_results'] = isset($response['command1']['mop']['0']['cc_results']) ? $response['command1']['mop']['0']['cc_results'] : '0';
    // If Payment success
    if (isset($response['command1']['mop']['0']['cc_captured']) && $response['command1']['mop']['0']['cc_captured'] == 'Y') {
      watchdog('tmapi_payment', 'TM Payment Success for ACCT_ID. %data', array('%data' => $data['tmuser_info']['acct_id']), WATCHDOG_NOTICE, NULL);
      //notify user about the payment received.
      success_payment_user_notification($data, $cc_num, $site_mail);
      //notify admin about the payment received.
      if ($notify_email = variable_get('invoice_payment_notify_mail', '')) {
        success_payment_admin_notification($data, $cc_num, $notify_email,  $site_mail);
      }
      _save_payment_plan($data);
       _update_tm_payment_transaction_table($data);
       
      db_insert('tbl_tm_payment_flag')
       ->fields(array(
        'acct_id' => (isset($data['tmuser_info']['acct_id']) && $data['tmuser_info']['acct_id']) ? $data['tmuser_info']['acct_id'] : '',
        'flag' => 1
      ))->execute();      
       drupal_goto('invoice/payment/success');
    }
    // Payment Failure
    else {
      watchdog('tmapi_payment', 'TM Payment Failure for ACCT_ID. %data', array('%data' => $data['tmuser_info']['acct_id']), WATCHDOG_NOTICE, NULL);
      _update_tm_payment_transaction_table($data);
      // Payment request failure mail
      failure_payment_admin_notification($data, $cc_num, $site_mail);
      drupal_goto('invoice/payment/failure');
    }
  }
  else {
    watchdog('tmapi_payment', 'TM Payment Failure due to no response from TM for ACCT_ID. %data ', array('%data' => $data['tmuser_info']['acct_id']), WATCHDOG_NOTICE, NULL);
    // API Failure mail
    api_failure_mail($data, $cc_num, $site_mail);
    drupal_goto('invoice/payment/failure');
  }
  $cc_num = '';
}

/**
 * Ticketmaster Payment Success
 */
function custom_invoice_payment_success() {
  //module_load_include('inc', 'custom_block', 'custom_block.block');
  drupal_add_css(drupal_get_path('module', 'custom_invoice') . '/css/invoice.css');
  global $user;
  $user_info = invoice_user_info_block();
  return theme('tm_payment_success', array('user_info' => $user_info, 'acct_id' => $user->name));
}

/**
 * Ticketmaster Payment failure
 */
function custom_invoice_payment_failure() {
  global $user;
  drupal_add_css(drupal_get_path('module', 'custom_invoice') . '/css/invoice.css');
  module_load_include('inc', 'custom_block', 'custom_block.block');
  $user_info = invoice_user_info_block();
  return theme('tm_payment_error', array('user_info' => $user_info, 'acct_id' => $user->name));
}

/**
 * update TM API payment success update
 */
function success_payment_user_notification($data=array(), $cc_num=NULL, $site_mail=NULL) {
  if ((count($data)>0) && (isset($cc_num) && !empty($cc_num)) && (isset($site_mail) && !empty($site_mail))) {
    $params = array();
    $to = $data['email'];
    $params['subject'] = variable_get('invoice_payment_subject', t('Thanks For Your Payment'));
    $params['body'] = t(variable_get('invoice_payment_body', 'Thanks for your payment.'), array('%amount' => $data['amount'], '%cc_num' => $cc_num));
    $params['headers']['Bcc'] = 'rkumar@io-media.com,adeb@io-media.com';
    drupal_mail('custom_hooks', 'tm_payment_notify', $data['email'], LANGUAGE_NONE, $params, $site_mail);
  }
}

/**
 * update TM API payment success admin update
 */
function success_payment_admin_notification($data=array(), $cc_num=NULL, $notify_email=NULL, $site_mail=NULL) {
  if((count($data)>0) && (isset($cc_num) && !empty($cc_num)) && (isset($site_mail) && !empty($site_mail)) && (isset($notify_email) && !empty($notify_email))) {
    $params = array();
    $to = $notify_email;
    $params['subject'] = variable_get('invoice_payment_notify_subject', t('Payment has been received'));
    $params['body'] = t(variable_get('invoice_payment_notify_body', 'Payment has been received.<br/> $%amount was charged from card ending in %cc_num<br/>'), array('%amount' => $data['amount'], '%cc_num' => $cc_num));
    drupal_mail('custom_hooks', 'tm_payment_notify', $to, LANGUAGE_NONE, $params, $site_mail);
  }
}

/**
 * update TM API payment failure admin update
 */
function failure_payment_admin_notification($data=array(), $cc_num=NULL, $site_mail=NULL){
  if ((count($data)>0) && (isset($cc_num) && !empty($cc_num)) && (isset($site_mail) && !empty($site_mail))) {
    $params = array();
    //$to = 'awolfe@io-media.com, rkumar@io-media.com, adeb@io-media.com, pmanas@io-media.com';
	$to = '';
	if (variable_get('invoice_payment_failure_additional_mail', '')){
	 $to = variable_get('invoice_payment_failure_additional_mail', '');
	 $to_array = explode(',', $to);
	 $to_array = array_map('trim',$to_array);	 
	 $to = implode(',', array_unique($to_array));
	}
    $params['subject'] = t('Payment Request failed for Account -  %acct_id', array('%acct_id' => $data['tmuser_info']['acct_id']));
    $params['body'] = t('Payment Request has been failed for Acct_ID %acct_id with card ending in %cc_num<br/><br/>TM Raw Response: %response', array('%acct_id' => $data['tmuser_info']['acct_id'], '%cc_num' => $cc_num, '%response' => $data['tm_raw_response']));
    drupal_mail('custom_hooks', 'tm_payment_notify', $to, LANGUAGE_NONE, $params, $site_mail);
  }
}

/**
 * update TM API failure admin update
 */
function api_failure_mail($data=array(), $cc_num=NULL, $site_mail=NULL){
  if((count($data)>0) && (isset($cc_num) && !empty($cc_num)) && (isset($site_mail) && !empty($site_mail))) {
    // API Failure mail
    //$to = 'awolfe@io-media.com, rkumar@io-media.com, adeb@io-media.com, pmanas@io-media.com';
	$to = '';
	if (variable_get('invoice_payment_failure_additional_mail', '')){
	 $to = variable_get('invoice_payment_failure_additional_mail', '');
	 $to_array = explode(',', $to);
	 $to_array = array_map('trim',$to_array);	 
	 $to = implode(',', array_unique($to_array));
	}
    $params['subject'] = t('No TM Response: Payment Request failed for Account- %acct_id', array('%acct_id' => $data['tmuser_info']['acct_id']));
    $params['body'] = t('Payment Request has been failed for Acct_ID %acct_id with card ending in %cc_num<br/> Resaon: No TM Response', array('%acct_id' => $data['tmuser_info']['acct_id'], '%cc_num' => $cc_num));
    drupal_mail('custom_hooks', 'tm_payment_notify', $to, LANGUAGE_NONE, $params, $site_mail);
  }
}

/**
 * update tm_payment_transaction_table update
 */
function _update_tm_payment_transaction_table($data) {
  db_insert('tm_payment_transaction_detail')
  ->fields(array(
    'cc_type' => (isset($data['ctype']) && $data['ctype']) ? $data['ctype'] : '',
    'cc_num' => (isset($data['cc_num']) && $data['cc_num']) ? $data['cc_num'] : '',
    'exp_date' => (isset($data['exp_date']) && $data['exp_date']) ? $data['exp_date'] : '',
    'amount' => (isset($data['amount']) && $data['amount']) ? $data['amount'] : '',
    'acct_id' => (isset($data['tmuser_info']['acct_id']) && $data['tmuser_info']['acct_id']) ? $data['tmuser_info']['acct_id'] : '',
    'timestamp' => time(),
    'tm_cc_captured' => $data['tm_response_cc_captured'],
    'tm_cc_results' => (isset($data['tm_cc_results']) && $data['tm_cc_results']) ? $data['tm_cc_results'] : '',
    'tm_raw_response' => (isset($data['tm_raw_response']) && $data['tm_raw_response']) ? $data['tm_raw_response'] : '',
    'email_addr' => $data['email']
    ))->execute();
}



function _save_payment_plan($data=array()) {
  if(isset($data) && count($data)){
    //save account id and payment plan id in db
    $acctid = (isset($data['tmuser_info']['acct_id']) && $data['tmuser_info']['acct_id']) ? $data['tmuser_info']['acct_id'] : '';
    $plid = (isset($data['payment_plan_id']) && $data['payment_plan_id']) ? $data['payment_plan_id'] : '';
    
    $query = db_select('tbl_tm_payment_plan', 'b');
    $result = (bool)$query->fields('b', array('payment_plan_id'))
    ->condition('acct_id', $acctid, '=')
    ->condition('payment_plan_id', $plid, '=')
    ->execute()
    ->fetchField();
    
    if(!$result){
      db_insert('tbl_tm_payment_plan')
       ->fields(array(
        'acct_id' => $acctid,
        'payment_plan_id' => $plid
      ))->execute();
    }
  }
}

/*
* New onchange of Plans
*/
function custom_invoice_payment_plans_desc($pid) {
  if (!is_numeric($pid) && ($pid!='default')) {
    return "";
  }
  /*$ops['type'] = arg(2);
  drupal_json_output($ops);
 exit;*/
  module_load_include('inc', 'tm_api', 'tm_api.helper');
  global $user;
  if ($user->uid) {
    $acct_id = $user->name;
    if ($acct_id) {
      $get_amount_info = $_SESSION['invoice_detail'];
    }
  }
  
  $invoice_id = $get_amount_info['invoice_ids'];
  $plans = get_plans($acct_id, $invoice_id);
  //filter plans
  $plans = filter_payment_plans($acct_id, $invoice_id, $plans);
  foreach ($plans as $plan) {
    if ($plan['payment_plan_id'] == $pid) {
      $plandesc = $plan['plan_desc'] . '<br/>' . $plan['plan_text'];
    }
  }

  $ops = cache_get('tmapi_get_payment_schedules-' . $pid);
  if (!$ops) {
    $ops = get_schedule_for_plan_id($acct_id, $invoice_id, $pid, $get_amount_info, $plans);
  }
  else{
    $ops = isset($ops->data)?$ops->data:"";
  }
  //$ops['amount_due_today'] = "0.00";
  //$ops['item_list'] = "No Payment breakup available for this plan";
  if(isset($plandesc) && $plandesc){
    $ops['desc'] = $plandesc;
  }
  else{
    $ops['desc'] = "No description available for this plan.";
  }
  drupal_json_output($ops);
  exit;
}

function get_plans($acct_id, $invoice_id){
  module_load_include('inc', 'tm_api', 'tm_api.helper');
  /*
  $plans = cache_get('tmapi_get_payment_plans');
  if (!cache_get('tmapi_get_payment_plans')) {
    $plans = tmapi_get_payment_plans($acct_id, $invoice_id);
  }
  else{
    $plans = isset($plans->data)?$plans->data:"";
  }*/
  //cache payment plans
  $cid = 'tmapi_get_payment_plans';        
  $cache = cache_get($cid);  
  //$timed = format_date($cache->expire);
  //$now_time = format_date(time());
  //flog_it('now time '.$now_time);  
  if (!empty($cache->data)) {
   //flog_it('Expire_time '.$timed);
    if (time() >= $cache->expire) {
      $plans = tmapi_get_payment_plans($acct_id, $invoice_id);            
    }
     else {
       $plans = $cache->data;
    }
  }
  //If first time user is calling.
  else {
    $plans = tmapi_get_payment_plans($acct_id, $invoice_id);
  }
  return $plans;
}