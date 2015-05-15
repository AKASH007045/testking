<?php
/**
 * @file
 */
function get_plans($acct_id = NULL, $invoice_id=NULL){
  if(
      (isset($acct_id) && !empty($acct_id))
      &&
      (isset($invoice_id) && !empty($invoice_id))
  ) {
    module_load_include('inc', 'tm_api', 'tm_api.helper');
    $plan_schedules = array();
    $plans = tmapi_get_payment_plans($acct_id, $invoice_id);
    if($plans){
      foreach($plans as $plan){
        $plan_schedules[$plan['payment_plan_id']]['plan'] = $plan; 
      }
      return $plan_schedules;
    }
    else{
      //No Plans present
      return false;
    }    
  }
  else{
    //Either account ID or Invoice ID is NULL
    return false;
  }
}

function get_plan_schedules($acct_id = NULL, $invoice_id=NULL, $plan_id=NULL){
  if(
      (isset($acct_id) && !empty($acct_id))
      &&
      (isset($invoice_id) && !empty($invoice_id))
      &&
      (isset($plan_id) && !empty($plan_id))
  ) {
    module_load_include('inc', 'tm_api', 'tm_api.helper');
    $plan_schedules = array();
    $payments = tmapi_get_payment_schedules($acct_id, $invoice_id, $plan_id);
    
    if($payments){
      foreach($payments as $payment){
        $plan_schedules[] = $payment;
      }
    } 
    return $plan_schedules;    
  }
  else{
    //Either account ID or Invoice ID is NULL
    return false;
  }
}

function get_schedule_for_plan_id($acct_id=NULL, $invoice_id=NULL, $plan_id=NULL){
  if(
      (isset($acct_id) && !empty($acct_id))
      &&
      (isset($invoice_id) && !empty($invoice_id))
      &&
      (isset($plan_id) && !empty($plan_id))
  ){ 
    $items = array();
    $title = t(''); //Blank to show no header for item list
    $amount_due_today = "0.00";
    $type = 'ol';
    $ttl = NULL;
    
    if($plan_id=='default'){
      $get_amount_info = get_tm_user_amount_info($acct_id);      
      $amount_due_today = isset($get_amount_info['due_amt'])?$get_amount_info['due_amt']:'0.00';
      $amount_due_today = ($amount_due_today<0)?"0.00":$amount_due_today;
      $items[] = array(
          'data' => '<span class="duedate">Full Payment</span> <span class="dollars">$'.$amount_due_today.'</span>'
      );
    }
    else{ 
      $schedules = get_plan_schedules($acct_id, $invoice_id, $plan_id);
      foreach($schedules as $schedule){ 
        $ttl = date('F d, Y', strtotime($schedule['payment_due_date']));      
        
        if(isset($schedule['amount_due_today']) && !empty($schedule['amount_due_today'])){
          $amount_due_today = $schedule['amount_due_today'];
        }
        
        $items[] = array(
          'data' => '<span class="duedate">'.$ttl.'</span> <span class="dollars">$'.$schedule['owed_amount'].'</span>'
        );
      }
    }    
    
    if(isset($items) &&count($items)>0){
      $desc_options = theme_item_list(array(
        'items' => $items, 
        'title' => $title, 
        'type' => $type, 
        'attributes' => array()
      ));
      
      $ops = array();
      $ops['amount_due_today'] = ($amount_due_today<0)?"0.00":$amount_due_today;
      $ops['item_list'] = $desc_options;
      drupal_json_output($ops);
      exit;
    }
    else{
      echo "";
      exit;
    }
  }
  else{
    echo "";
    exit;
  }  
}
/*
function filter_plans($plans=array()){
  if(count($plans)>0){
    $temp = array();
    $temp[] = array(
      'plan' => array(
        'payment_plan_id' => 'default',
        'plan_text' => '1 Payment',
        'suppress_payment_plan_details' => 'N'
      )
    );
    foreach($plans as $plan){
      if(in_array($plan['plan']['payment_plan_id'], array('81', 'default'))){
        $temp[] = $plan;
      }
    }
    return $temp;
  }
  else{
    return array();
  }
}*/
function filter_plans($plans=array()){
  if(count($plans)>0){
    $temp = array();
    $sort_arr = array();
    $sorted = array();
    /*$temp[0] = array(
      'plan' => array(
        'payment_plan_id' => 'default',
        'plan_long_name' => 'Full Payment',
        'suppress_payment_plan_details' => 'N'
      )
    );*/
    foreach($plans as $plan){
      if(in_array($plan['plan']['payment_plan_id'], array('110', '111'))){
        $temp[$plan['plan']['payment_plan_id']] = $plan;
        $sort_arr[$plan['plan']['payment_plan_id']] = $plan['plan']['plan_desc'];
      }
    }
    //sorting plan names
    arsort($sort_arr);
    foreach($sort_arr as $sa_key => $sa_value){
      $sorted[$sa_key] = $temp[$sa_key];
    }
    
    array_unshift($sorted, array(
      'plan' => array(
        'payment_plan_id' => 'default',
        'plan_long_name' => 'Full Payment',
        'plan_desc' => 'Full Payment',
        'suppress_payment_plan_details' => 'N'
      )
    ));

    return $sorted;
  }
  else{
    return array();
  }
}
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

  $params = array(
    'name' => $user->name,
    'subject' => variable_get('invoice_mail_subject', t('2014-15 Phoenix Suns Invoice')),
    'body'=> variable_get('invoice_mail_body', t('Attached to this email is a copy of your invoice for the 2014-15 Phoenix Suns Season Subscription.')),
  );

  // Checking mPDF library existence.
  if (pdf_using_mpdf_library_exist() == TRUE) {
    $filename = "Kings-Invoice_" . str_replace(' ', '-', $name);
    _pdf_using_mpdf_generator($output, $filename, 'F', $name);
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
    $playoff_data = get_playoff_data($acct_id);
    if (!count($seat_holds) && !count($playoff_data)) {
      drupal_set_message(t('Invoice data not avaiable'), 'warning');
      drupal_goto('invoice');
    }
  }
  $output = _tmapi_user_invoice('pdf');
  // Checking mPDF library existence.
 
  if (pdf_using_mpdf_library_exist() == TRUE) {
    $filename = "Kings-Invoice_" . str_replace(' ', '-', $name);
    $conf['pdf_using_mpdf_pdf_save_option'] = 0;
    _pdf_using_mpdf_generator($output, $filename, 'I', $name);
    exit;
  }
  return '';
}

function custom_invoice_page() {
  drupal_add_css(drupal_get_path('module', 'custom_invoice') . '/css/invoice.css');
  return _tmapi_user_invoice();
}

/**
 * Invoice Page Block
 */
function _tmapi_user_invoice($pdf = NULL) {
  //drupal_add_css(drupal_get_path('module', 'custom_block') . '/css/invoice.css');
  module_load_include('inc', 'tm_api', 'tm_api.helper');
  global $user;
  $acct_id = $user->name;
  $cnt = 0;
  $tmapi_invoice = tmapi_get_sth_invoice($acct_id);
  if (isset($acct_id)) {
    $seat_holds = get_seats_hold($acct_id);
    //$payment_flag = get_tm_payment_flag($acct_id);
    $user_info = invoice_user_info_block();
    $output = '';
    
    /*
    * Kings Specific - Add pricing link on invoice page
    */
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
      if ($pdf) {      
        $name = get_user_full_name($user);
        $user_info = get_user_info($acct_id);
         $output .= theme('renewal_invoice_' . $pdf, array('seat_holds' => $seat_holds, 'user_info' => $user_info, 'cnt' => $cnt, 'name' => $name));
      } else {
         $output .= theme('renewal_invoice', array('seat_holds' => $seat_holds, 'user_info' => $user_info, 'cnt' => $cnt, 'pdf_link' => $link));
      }
    endif;
    return theme('invoice_page', array('output' => $output));
  }
  return '';
}


function custom_invoice_payment_request() {
  global $user;
  drupal_add_css(drupal_get_path('module', 'custom_invoice') . '/css/invoice.css');
  $options_plans = $options = array();
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

    
  //$inv_detail will have the detail of the 
  //selected invoice from the invoice_list call to check for the payment plan id
  $inv_detail = array();
  if(isset($_SESSION['invoice_detail']) && !empty($_SESSION['invoice_detail'])){
    $inv_detail = $_SESSION['invoice_detail'];
  }
  
  $invoice_id = $get_amount_info['invoice_id']; 
  $plans = get_plans($acct_id, $invoice_id); 
  $plans = filter_plans($plans);

  $options = _get_us_states();
  foreach($plans as $plan){
    //Only select active plans
    if($plan['plan']['suppress_payment_plan_details']=='N'){
      $options_plans[$plan['plan']['payment_plan_id']] = isset($plan['plan']['plan_desc'])?$plan['plan']['plan_desc']:$plan['plan']['plan_name']; 
    }
  }  

  $form['#attributes']['autocomplete'] = 'off';
  if(
    isset($inv_detail['payment_plan_id']) 
    && 
    !empty($inv_detail['payment_plan_id'])
    &&
    in_array($inv_detail['payment_plan_id'], array_keys($options_plans))
  ){
    
    $form['payment_plans_title'] = array(
      '#type' => 'markup',
      '#prefix' => '<div class="col-sm-12 payment-user-form-wrapper"><div class="col-sm-1">&nbsp;</div><div class="col-sm-4 personal-info name-panel">',
      '#markup' => t('<label>Selected Payment Plan</label><br />')
    );
    
    $form['payment_plans'] = array(
      '#type' => 'markup',
      '#markup' => isset($options_plans[$inv_detail['payment_plan_id']])?'<div class="payplan">'.$options_plans[$inv_detail['payment_plan_id']].'</div></br><div class="form-item-payment-plan-id"><div class="DivSelectyze"></div></div>':"No plans are associated with this account",
      '#suffix' => '</div>',
    );
    
    $form['payment_plan_id'] = array(
      '#type' => 'hidden',
      '#value' => isset($inv_detail['payment_plan_id'])?$inv_detail['payment_plan_id']:"",
      '#attributes' => array('id'=>'payment_plan_id')
    );
  }
  else{
    $form['payment_plan_id'] = array(
      '#type' => 'select',
      '#prefix' => '<div class="col-sm-12 payment-user-form-wrapper"><div class="col-sm-1">&nbsp;</div><div class="col-sm-4 personal-info name-panel">',
      '#options' => $options_plans,
      '#title' => t('Select Payment Plan'),
      '#suffix' => '</div>',
      '#attributes' => array('id'=>'payment_plan_id')
    );
  }
  
  $form['tmuser_info'] = array(
    '#type' => 'hidden',
    '#value' => array('name' => $user_name, 'acct_id' => $acct_id, 'invoice_id' => isset($get_amount_info['invoice_id']) ? $get_amount_info['invoice_id'] : '' , 'due_amt' => isset($get_amount_info['due_amt']) ? $get_amount_info['due_amt'] : ''),
  );
  
  /*$form['amount_due_today'] = array(
    '#type' => 'textfield',
    '#value' => "",
    '#attributes' => array('id'=>'amount_due_today')
  );*/
  
  $form['acct_info'] = array(
    '#type' => 'markup',
    '#markup' => $user_info,
    '#weight' => -10,
  );
  /*$form['first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First Name'),
    '#prefix' => '<div class="col-sm-12 payment-user-form-wrapper"><div class="col-sm-1">&nbsp;</div><div class="col-sm-4 personal-info name-panel">',
    '#required' => TRUE,
    '#weight' => -9,
    '#maxlength' => 40,
    '#attributes' => array('tabindex' => '1')
  );
  $form['last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name'),
    '#required' => TRUE,
    '#maxlength' => 40,
    '#attributes' => array('tabindex' => '2')
  );
  $form['Avs_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Address'),
    '#required' => TRUE,
    '#maxlength' => 20,
    '#attributes' => array('tabindex' => '3')
  );
  $form['Avs_city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#maxlength' => 40,
    '#attributes' => array('tabindex' => '4')
  );
  
  $form['Avs_state'] = array(
    '#type' => 'select',
    '#title' => t('State'),
    '#options' => $options,
    '#required' => TRUE,
    '#maxlength' => 5,
    '#attributes' => array('tabindex' => '5')
  );
  $form['Avs_postal_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Zip code'),
    '#required' => TRUE,
    '#maxlength' => 10,
    '#suffix' => '<div class="required-error">* Required Fields</div></div>',
    '#attributes' => array('tabindex' => '6')
  );*/
   $form['ctype'] = array(
    '#type' => 'select',
    '#title' => t('Card Type:'),
    '#options' => array('VI' => 'Visa', 'MC' => 'MasterCard', 'AE' => 'American Express', 'DI' => 'Discover'),
    '#required' => TRUE,
    '#prefix' => '<div class="col-sm-2">&nbsp;</div><div class="col-sm-4 personal-info card-panel">',
    '#attributes' => array('tabindex' => '7')
  );
  $form['cc_num'] = array(
    '#type' => 'textfield',
    '#title' => t('Card Number:'),
    '#required' => TRUE,
    '#maxlength' => 16,
    '#element_validate' => array('element_validate_integer_positive'), 
    '#attributes' => array('tabindex' => '8')
  );
  $form['exp_date'] = array(
    '#type' => 'textfield',
    '#title' => t('Expiration Date:'),
    '#maxlength' => 4,
    '#element_validate' => array('element_validate_integer_positive'),
    '#required' => TRUE,
    '#attributes' => array('tabindex' => '9', 'placeholder' => 'mmyy')
  );
  $form['cin'] = array(
    '#type' => 'textfield',
    '#title' => t('Card Verification Number:'),
    '#required' => TRUE,
    '#maxlength' => 4,
    '#element_validate' => array('element_validate_integer_positive'), 
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
      '#attributes' => array('tabindex' => '12'),
      '#suffix' => '</div></div>'
    );
  
  /*$form['amount_due_today'] = array(
    '#type' => 'textfield',
    '#maxlength' => 100,
    '#prefix' => '<div id="div_amount_due_today" style="visibility:hidden;">',
    '#suffix' => '</div>',
    '#attributes' => array('tabindex' => '15')
  );*/
  $form['due_amount'] = array(
    '#type' => 'markup',
    '#prefix' => '<div class="col-sm-12 payment-amount-box"><div class="col-sm-7">&nbsp</div><div class="col-sm-4 amount-due">',
    '#markup' => '<label>' . t('Amount Due Today:') . '</label><div class="ttlamt">' . invoice_money_format($get_amount_info['due_amt']) . '</div>',
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
  //print_r($data);
  //print_r($_POST['amount_due_today']);

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
    if (!is_numeric($data['amount']) || $data['amount'] <= 0 || strstr($data['amount'], ' ')) {
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
  $data_renew = array(
    'acct_id' => $data['tmuser_info']['acct_id'],
    'show_popup' => 1
  );  

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
      _renew_status_table($data_renew); 
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
  // Generate the new link using the submitted text value.
  
  //$form_state['ajax_commands'][] = ctools_modal_command_display('<img src="/sites/all/themes/sports/images/starSmall.png">', $msg);
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
function _update_tm_payment_transaction_table($data) {
  db_insert('tm_payment_transaction_detail')
  ->fields(array(
    'first_name' => (isset($data['first_name']) && $data['first_name']) ? $data['first_name'] : '',
    'last_name' => (isset($data['last_name']) && $data['last_name']) ? $data['last_name'] : '',
    'address' => (isset($data['Avs_address']) && $data['Avs_address']) ? $data['Avs_address'] : '',
    'city' => (isset($data['Avs_city']) && $data['Avs_city']) ? $data['Avs_city'] : '',
    'state' => (isset($data['Avs_state']) && $data['Avs_state']) ? $data['Avs_state'] : '',
    'zip' => (isset($data['Avs_postal_code']) && $data['Avs_postal_code']) ? $data['Avs_postal_code'] : '',
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

function success_payment_user_notification($data=array(), $cc_num=NULL, $site_mail=NULL){
  if(
  (count($data)>0)
  &&
  (isset($cc_num) && !empty($cc_num))
  &&
  (isset($site_mail) && !empty($site_mail))
  ){
    $params = array();
    $to = $data['email'];
    //$to = "pmanas@io-media.com";
    $params['subject'] = variable_get('invoice_payment_subject', t('Thanks For Your Payment'));
    $params['body'] = t(variable_get('invoice_payment_body', 'Thanks for your payment.'), array('%amount' => $data['amount'], '%cc_num' => $cc_num));
    drupal_mail('custom_hooks', 'tm_payment_notify', $data['email'], LANGUAGE_NONE, $params, $site_mail);
  }
}

function success_payment_admin_notification($data=array(), $cc_num=NULL, $notify_email=NULL, $site_mail=NULL){
  if(
  (count($data)>0)
  &&
  (isset($cc_num) && !empty($cc_num))
  &&
  (isset($site_mail) && !empty($site_mail))
  &&
  (isset($notify_email) && !empty($notify_email))
  ){
    $params = array();
    $to = $notify_email;
    //$to = "pmanas@io-media.com";
    $params['subject'] = variable_get('invoice_payment_notify_subject', t('Payment has been received'));
    $params['body'] = t(variable_get('invoice_payment_notify_body', 'Payment has been received.<br/> $%amount was charged from card ending in %cc_num<br/>'), array('%amount' => $data['amount'], '%cc_num' => $cc_num));
    drupal_mail('custom_hooks', 'tm_payment_notify', $to, LANGUAGE_NONE, $params, $site_mail);
  }
}

function failure_payment_admin_notification($data=array(), $cc_num=NULL, $site_mail=NULL){
  if(
  (count($data)>0)
  &&
  (isset($cc_num) && !empty($cc_num))
  &&
  (isset($site_mail) && !empty($site_mail))
  ){
    $params = array();
    $to = 'awolfe@io-media.com, rkumar@io-media.com, adeb@io-media.com, ssingh@io-media.com, pmanas@io-media.com';
    $params['subject'] = t('Payment Request failed for Account -  %acct_id', array('%acct_id' => $data['tmuser_info']['acct_id']));
    $params['body'] = t('Payment Request has been failed for Acct_ID %acct_id with card ending in %cc_num<br/><br/>TM Raw Response: %response', array('%acct_id' => $data['tmuser_info']['acct_id'], '%cc_num' => $cc_num, '%response' => $data['tm_raw_response']));
    drupal_mail('custom_hooks', 'tm_payment_notify', $to, LANGUAGE_NONE, $params, $site_mail);
  }
}

function api_failure_mail($data=array(), $cc_num=NULL, $site_mail=NULL){
  if(
  (count($data)>0)
  &&
  (isset($cc_num) && !empty($cc_num))
  &&
  (isset($site_mail) && !empty($site_mail))
  ){
    // API Failure mail
    $to = 'awolfe@io-media.com, rkumar@io-media.com, adeb@io-media.com, ssingh@io-media.com, pmanas@io-media.com';
    $params['subject'] = t('No TM Response: Payment Request failed for Account- %acct_id', array('%acct_id' => $data['tmuser_info']['acct_id']));
    $params['body'] = t('Payment Request has been failed for Acct_ID %acct_id with card ending in %cc_num<br/> Resaon: No TM Response', array('%acct_id' => $data['tmuser_info']['acct_id'], '%cc_num' => $cc_num));
    drupal_mail('custom_hooks', 'tm_payment_notify', $to, LANGUAGE_NONE, $params, $site_mail);
  }
}

function _save_payment_plan($data=array()){
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

function _get_us_states() {
  return array(
        'AL' => 'Alabama',
        'AK' => 'Alaska',
        'AZ' => 'Arizona',
        'AR' => 'Arkansas',
        'CA' => 'California',
        'CO' => 'Colorado',
        'CT' => 'Connecticut',
        'DE' => 'Delaware',
        'FL' => 'Florida',
        'GA' => 'Georgia',
        'HI' => 'Hawaii',
        'ID' => 'Idaho',
        'IL' => 'Illinois',
        'IN' => 'Indiana',
        'IA' => 'Iowa',
        'KS' => 'Kansas',
        'KY' => 'Kentucky',
        'LA' => 'Louisiana',
        'ME' => 'Maine',
        'MD' => 'Maryland',
        'MA' => 'Massachusetts',
        'MI' => 'Michigan',
        'MN' => 'Minnesota',
        'MS' => 'Mississippi',
        'MO' => 'Missouri',
        'MT' => 'Montana',
        'NE' => 'Nebraska',
        'NV' => 'Nevada',
        'NH' => 'New Hampshire',
        'NJ' => 'New Jersey',
        'NM' => 'New Mexico',
        'NY' => 'New York',
        'NC' => 'North Carolina',
        'ND' => 'North Dakota',
        'OH' => 'Ohio',
        'OK' => 'Oklahoma',
        'OR' => 'Oregon',
        'PA' => 'Pennsylvania',
        'RI' => 'Rhode Island',
        'SC' => 'South Carolina',
        'SD' => 'South Dakota',
        'TN' => 'Tennessee',
        'TX' => 'Texas',
        'UT' => 'Utah',
        'VT' => 'Vermont',
        'VA' => 'Virginia',
        'WA' => 'Washington',
        'WV' => 'West Virginia',
        'WI' => 'Wisconsin',
        'WY' => 'Wyoming',
        'other' => 'Other'
  );
}