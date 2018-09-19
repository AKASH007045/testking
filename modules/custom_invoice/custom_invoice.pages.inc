<?php
use Mpdf\Mpdf;
/**
 * @file
 */
/**
 * Menu Callback for invoice list page.
 */
function invoices_list_page() {
  drupal_add_css(drupal_get_path('module', 'custom_invoice') . '/css/invoice.css');
  global $user;
  $inv_title = variable_get('inv_list_page_title', 'My Invoices');
  drupal_set_title($inv_title);
  $acct_id = $user->name;
  module_load_include('inc', 'tm_api', 'tm_api.helper');  
  $stp_invoices = db_query("SELECT * FROM {stp_invoice} WHERE invoice_status = 1")->fetchAll();
  foreach ($stp_invoices as $inv_key => $stp_invoice) {
    $invoice_arr[$stp_invoice->invoice_season][$stp_invoice->id] = $stp_invoice;
  }
  $rows = array();
  $invoices = array();
  // We will clear all the cached invoice list data here and get updated data from ticketmaster
  cache_clear_all('invoice_list-' . $acct_id, 'cache_custom', $wildcard = FALSE);
  $invoices = get_invoice_list($acct_id, 1);
  $cnt = 0;
  $headers = array('', variable_get('inv_list_date_title', 'DUE DATE'), variable_get('inv_list_balance_title', 'BALANCE DUE'), '');
  $prnt_btn = variable_get('inv_list_page_print', 'Print');
  $pay_btn = variable_get('inv_list_page_pay', 'Pay');
  $date_format = variable_get('inv_list_date_format', 'm/d/Y');
  
  if (count($invoices)) {
    foreach ($invoices as $invoice_id => $invoice_details) {
      // Current Invoices
      if (count($invoice_arr['current']) > 0) {
        foreach ($invoice_arr['current'] as $stp_invoice_current) {
          $invoice_filter = $stp_invoice_current->stp_invoice_filter;
          $invoice_desc= array_map('strtolower', explode(",", $stp_invoice_current->invoice_value));
          if ((($invoice_filter == 2 && stp_strpos_array(strtolower($invoice_details[$stp_invoice_current->invoice_field]), $invoice_desc)) !== false) || ($invoice_filter == 0) || ($invoice_filter == 1)) {
            if ($cnt && $invoice_filter == 1) {
              continue;
            }
            if ($invoice_details['balances'] && $invoice_details['balances'] > 0) {
              $pending_invoice[$invoice_details['invoice_ids']]['event_name'] = isset($invoice_details['invoice_long_descriptions']) ? $invoice_details['invoice_long_descriptions'] : $invoice_details['invoice_descriptions'];
              $pending_invoice[$invoice_details['invoice_ids']]['due_date'] = date($date_format, strtotime($invoice_details['due_dates']));
              $pending_invoice[$invoice_details['invoice_ids']]['current_due_amounts'] = invoice_money_format($invoice_details['balances']);
              $pending_invoice[$invoice_details['invoice_ids']]['action_button'] = '<a data-title="' . $pay_btn . '" href="/invoice/' . $invoice_details['invoice_ids'] . '/' . $stp_invoice_current->id .'" class="btn btn-default email-button pay" title="' . $pay_btn . '">' . $pay_btn . ' <i class="fa fa-chevron-right"></i></a> &nbsp;&nbsp;' . '<a target="_blank" data-title="' . $prnt_btn . '" href="' . base_path() . 'invoice/' . $invoice_details['invoice_ids'] . '/print/' . $stp_invoice_current->id . '" class="btn btn-default email-button btnprint" title="' . $prnt_btn . '">' . $prnt_btn . ' <i class="fa fa-chevron-right"></i></a>';
            }
            else {
              $past_invoice[$invoice_details['invoice_ids']]['event_name'] = isset($invoice_details['invoice_long_descriptions']) ? $invoice_details['invoice_long_descriptions'] : $invoice_details['invoice_descriptions'];
              $past_invoice[$invoice_details['invoice_ids']]['action_button'] = '<a target="_blank" data-title="' . $prnt_btn . '" href="' . base_path() . 'invoice/' . $invoice_details['invoice_ids'] . '/print/' . $stp_invoice_current->id . '" class="btn btn-default email-button btnprint" title="' .$prnt_btn . '">' . $prnt_btn . ' <i class="fa fa-chevron-right"></i></a>';
            }
            $cnt++;
          }
        }
      }
      // Past Invoices
      if (count($invoice_arr['past']) > 0) {
        foreach ($invoice_arr['past'] as $stp_invoice_past) {
          $invoice_filter = $stp_invoice_past->stp_invoice_filter;
          $invoice_desc= array_map('strtolower', explode(",", $stp_invoice_past->invoice_value));
          if ((($invoice_filter == 2 && stp_strpos_array(strtolower($invoice_details[$stp_invoice_past->invoice_field]), $invoice_desc)) !== false) || ($invoice_filter == 0) || ($invoice_filter == 1)) {
            if ($cnt && $invoice_filter == 1) {
              continue;
            }
            $past_invoice[$invoice_details['invoice_ids']]['event_name'] = isset($invoice_details['invoice_long_descriptions']) ? $invoice_details['invoice_long_descriptions'] : $invoice_details['invoice_descriptions'];
            $past_invoice[$invoice_details['invoice_ids']]['action_button'] = '<a target="_blank" data-title="' . variable_get('inv_list_page_print', 'Print') . '" href="' . base_path() . 'invoice/' . $invoice_details['invoice_ids'] . '/print/' . $stp_invoice_past->id . '" class="btn btn-default email-button btnprint" title="' . t('View') . '">' . t('View') . ' <i class="fa fa-chevron-right"></i></a>';
          }
          $cnt++;
        }
      }
    }
  }
  $data['pending'] = '<div class="no-invoice-found text-center">' . t('No Invoice found.') . '</div>';
  $data['past'] = '<div class="no-invoice-found text-center">' . t('No Invoice found.') . '</div>';
  if (isset($pending_invoice) && $pending_invoice && count($pending_invoice)) {
    $data['pending'] = theme('table', array(
      'header' => $headers,
      'rows' => $pending_invoice,
      'sticky' => FALSE,
      'empty' => t('No Invoice found.'),
      'attributes' => array('id' => 'invoices-list-pending-table'),
      )
    );
  }
  if (isset($past_invoice) && count($past_invoice)) {
    $data['past'] = theme('table', array(
        'header' => array('', ''),
        'rows' => $past_invoice,
        'sticky' => FALSE,
        'empty' => t('No invoice found.'),
        'attributes' => array('id' => 'invoices-list-past-table'),
      )
    );
  }
  $output = theme('invoice_list', array('data' => $data));
  return $output;
}

/**
* function to print invoice
* @param integer $invoice_id
* @return pdf version of invoice emailed to user
*/
function custom_invoice_email($invoice_id, $stp_inv_id) {
  global $user;
  $site_name =  variable_get('site_name', 'STP');
  $acct_id = $user->name;
  $name = get_user_full_name($user);
  $data = _tmapi_user_invoice($invoice_id, $stp_inv_id, 'pdf');
  $header = $data['header'];
  $footer = $data['footer'];
  $output = theme('invoice_page', array('output' => $data['output']));
  $wfid = variable_get('invoice_watermark_fid', '');
  $watermarkImage = '';
  if ($wfid != '') {
    $file = file_load($wfid);
    if ($file && isset($file->fid)) {
      $watermarkImage = file_create_url($file->uri);
    }
  }
  $template_format = custom_invoice_get_stored_invoice($stp_inv_id);
  $stp_invoice_setttings = node_load($template_format->invoice_template);

  $params = array(
    'name' => $user->name,
    'subject' => (isset($stp_invoice_setttings->field_subject['und'][0]['value']) && $stp_invoice_setttings->field_subject['und'][0]['value']) ? $stp_invoice_setttings->field_subject['und'][0]['value'] : variable_get('invoice_mail_subject', t('2015 STP Invoice')),
    'body'=> (isset($stp_invoice_setttings->body['und'][0]['value']) && $stp_invoice_setttings->body['und'][0]['value']) ? $stp_invoice_setttings->body['und'][0]['value'] : variable_get('invoice_mail_body', t('Attached to this email is a copy of your invoice for the 2016 STP Season Subscription.')),
  );

  // Checking mPDF library existence.
  if (pdf_using_mpdf_library_exist() == TRUE) {
  //user variable_get for sitename
    $filename = str_replace(' ', '-', $site_name) . "-Invoice_" . str_replace(' ', '-', $name);
    _pdf_using_mpdf_generator($output, $filename, 'F', $name, $watermarkImage, $header, $footer);
    $file = file_directory_temp() . '/' . $filename.".pdf";
    $params['file'] = $file;
    $params['filename'] = $filename . '.pdf';
  }

  $mail = $user->mail;
  drupal_mail('custom_hooks', 'stpmail_template', $mail, language_default(), $params);
//  drupal_mail('custom_hooks', 'triggerkey', $mail, language_default(), $params);
  drupal_set_message('Invoice mailed successfully.');
  drupal_goto('invoice/' . $invoice_id . '/' . $stp_inv_id);
  return '';
}

/**
* function to print invoice
* @param integer $invoice_id
* @return print pdf version of invoice
*/
function custom_invoice_print($invoice_id, $stp_inv_id) {
  global $user, $conf;
  $site_name =  variable_get('site_name', 'STP');
  $acct_id = $user->name;
  $name = get_user_full_name($user);
  $wfid = variable_get('invoice_watermark_fid', '');
  $watermarkImage = '';
  if($wfid != '') {
    $file = file_load($wfid);
    if($file && isset($file->fid)) {
      $watermarkImage = file_create_url($file->uri);
    }
  }
  $data = _tmapi_user_invoice($invoice_id, $stp_inv_id, 'pdf');
  $header = $data['header'];
  $footer = $data['footer'];
  $output = theme('invoice_page', array('output' => $data['output']));
  // Checking mPDF library existence.
  require_once drupal_get_path('module','pdf_using_mpdf'). '/MPDF7/vendor/autoload.php';
  //
// Checking mPDF library existence.
  // if (pdf_using_mpdf_library_exist() == TRUE) {
  //   //user variable_get for sitename
  //   $conf['pdf_using_mpdf_pdf_save_option'] = 0;
  //   $filename = str_replace(' ', '-', $site_name) . "-Invoice_" . str_replace(' ', '-', $name);
  //   // print "<pre>"; print_r($filename); die("test");
  //   $pdf = _pdf_using_mpdf_generator($output, $filename, 'F', $name, $watermarkImage, $header, $footer);
  //   if ($pdf) {
  //     $file = file_directory_temp() . '/' . $filename . ".pdf";
  //     // print "<pre>"; print_r($file); die("test");
  //     return $base_url . '/' . $file;
  //   }
  // }

  
  //
  // new \Mpdf\Mpdf();
  if (pdf_using_mpdf_library_exist() == TRUE) {
    $filename = str_replace(' ', '-', $site_name) . "-Invoice_" . str_replace(' ', '-', $name);
    $conf['pdf_using_mpdf_pdf_save_option'] = 0;
    _pdf_using_mpdf_generator($output, $filename, 'I', $name, $watermarkImage, $header, $footer);
    exit;
  }
  return '';
}
/**
* function to fetch invoice
* @param integer $invoice_id
* @return invoice page
*/
function custom_invoice_page($invoice_id, $stp_invoice_id) {
  return _tmapi_user_invoice($invoice_id, $stp_invoice_id);
}

/**
 * Invoice Page helper function
 * @param $invoice_id integer Invoice ID of the customer
 * @pdf return pdf output of the invoice if set value to pdf
 * @return user invoice
 */
function _tmapi_user_invoice($invoice_id = NULL, $stp_inv_id = NULL, $pdf = NULL) {
  drupal_add_css(drupal_get_path('module', 'custom_invoice') . '/css/invoice.css');
  module_load_include('inc', 'tm_api', 'tm_api.helper');
  global $user;
  $acct_id = $user->name;
  $stp_invoice_setttings = custom_invoice_get_stored_invoice($stp_inv_id);
  $typeform_nid = custom_invoice_get_assigned_typeform($stp_inv_id);
  $pay_btn_url = '/invoice/'. $invoice_id .'/payment/' . $stp_inv_id;
  if (isset($stp_invoice_setttings->invoice_en_typeform) && $stp_invoice_setttings->invoice_en_typeform == 1 && $typeform_nid) {
    $pay_btn_url = '/invoice/'. $invoice_id .'/typeform/' . $stp_inv_id . '/' .$typeform_nid;
  }
  $cnt = 0;
  cache_clear_all('invoice_detail-' . $acct_id . '-' . $invoice_id, 'cache_custom', $wildcard = FALSE);
  if (isset($acct_id)) {
    $seat_holds = get_invoice_details($acct_id, $invoice_id);
    $user_info = invoice_user_info_block();
    // Other Info Text
    $invoice_text = isset($stp_invoice_setttings->invoice_details_text) ? check_markup($stp_invoice_setttings->invoice_details_text, 'full_html', $langcode = '', $cache = FALSE) : '';
    $invoice_other_info_text = isset($stp_invoice_setttings->invoice_other_info_text) ? check_markup($stp_invoice_setttings->invoice_other_info_text, 'full_html', $langcode = '', $cache = FALSE) : '';
    $invoice_list = custom_invoice_InvoiceList($acct_id, $invoice_id);
    $event_details_invoice = tmapi_get_event_details_cache();
    if (isset($invoice_list['invoice_long_descriptions']) && $invoice_list['invoice_long_descriptions']) {
      drupal_set_title($invoice_list['invoice_long_descriptions']);
    }
    $output = '';
    if (count($seat_holds)):
	  //module_load_include('module', 'custom_invoice', 'custom_invoice.module');
	  drupal_load('module', 'custom_invoice');
      $total = $paid = $parking_fee = $handling_fee = 0;
      $invoice_table = custom_invoice_fetch_invoice_table($stp_invoice_setttings, $seat_holds);
      // Invoice Header
      $rows = array();
      $hrow = $invoice_table['header'];
      // Invoice Header
      $rows[] = array('data' => $hrow, 'class' => 'head');
      $due_date = (isset($seat_holds[0]['due_date']) && $seat_holds[0]['due_date']) ? date('l, F d, Y', strtotime($seat_holds[0]['due_date'])) : date('l, F d, Y');
      $invoice_date = (isset($seat_holds[0]['invoice_date']) && $seat_holds[0]['invoice_date']) ? date('l, F d, Y', strtotime($seat_holds[0]['invoice_date'])) : date('l, F d, Y');
      $row = array_merge($rows, $invoice_table['data']);
      $total = $invoice_table['total'];
      $paid = $invoice_table['paid'];
      $parking_fee = $invoice_table['parking'];
      $handling_fee = $invoice_table['handling'];
      // Parking & Handling Labels
      $p_label = $h_label = $park_fee_pdf = $hand_fee_pdf = '';
      if ($stp_invoice_setttings->{invoice_en_park}) {
        $p_label = '<div>' . $stp_invoice_setttings->invoice_park_title . ':</div>';
        $park_fee = '<div>' . invoice_money_format($parking_fee) . '</div>';
        $park_fee_pdf = '<div class="tcwrp"><div class="tcnm">'. $stp_invoice_setttings->invoice_park_title  .':</div> <div class="tcamt">' . invoice_money_format($parking_fee) . '</div></div>';
      }
      if ($stp_invoice_setttings->{invoice_en_hand}) {
        $h_label = '<div>' . $stp_invoice_setttings->invoice_hand_title . ':</div>';
        $hand_fee = '<div>' . invoice_money_format($handling_fee) . '</div>';
        $hand_fee_pdf = '<div class="tcwrp"><div class="tcnm">'. $stp_invoice_setttings->invoice_hand_title  .':</div> <div class="tcamt">' . invoice_money_format($handling_fee) . '</div></div>';
      }
      $due_amount = ($total + $parking_fee + $handling_fee) - $paid;
      $invoice_table = theme('table', array('header' => $header, 'rows' => $row, 'attributes' => array('class' => array('ticketTable chrt-dtl')), 'sticky' => FALSE));
      // If print or Email Invoice button is clicked
      if ($pdf) {
        $total_pdf = '<div class="tcwrp"><div class="tcnm">'. $stp_invoice_setttings->invoice_total_title .':</div> <div class="tcamt actinf">'. invoice_money_format($total) .'</div></div>';
        $paid_pdf = '<div class="tcwrp bdr3btn pdg5mrg"><div class="tcnm">'. $stp_invoice_setttings->invoice_credits_title .':</div> <div class="tcamt">-('. invoice_money_format($paid) .')</div></div>';
        $user_info = invoice_user_info_block(2);
        $acct_rep_info = custom_block_account_rep('block_1');
        $output .= theme('invoice_' . $pdf, array(
          'seat_holds' => $seat_holds,
          'user_info' => $user_info,
          'acct_rep_info' => $acct_rep_info,
          'cnt' => $cnt,
          'invoice_id' => $invoice_id,
          'invoice_table' => $invoice_table,
          'total' => $total_pdf,
          'paid' => $paid_pdf,
          'parking_fee' => $park_fee_pdf,
          'handling_fee' => $hand_fee_pdf,
          'due_amount' => invoice_money_format($due_amount),
          'due_date' => $due_date,
          'invoice_date' => $invoice_date,
          'invoice_list' => $invoice_list,
          'invoice_other_info_text' => $invoice_other_info_text,
          'terms' => (isset($stp_invoice_setttings->invoice_en_terms_pdf) && $stp_invoice_setttings->invoice_en_terms_pdf) ? $stp_invoice_setttings->invoice_terms: '',
          )
        );
        // PDF header
        $header = theme('invoice_header', array(
          'due_amount' => invoice_money_format($due_amount),
          'due_date' => $due_date,
          'invoice_date' => $invoice_date,
          'invoice_id' => $invoice_id,
          'invoice_list' => $invoice_list,
          )
        );
        //PDF footer
        $footer = theme('invoice_footer', array(
          'due_amount' => invoice_money_format($due_amount),
          'due_date' => $due_date,
          'invoice_date' => $invoice_date,
          'invoice_id' => $invoice_id
          )
        );
        return array('output' => $output, 'header' => $header, 'footer' => $footer);
      }
      else {
        $output .= theme('invoice', array(
          'seat_holds' => $seat_holds,
          'user_info' => $user_info,
          'cnt' => $cnt,
          'payment_flag' => $payment_flag,
          'invoice_table' => $invoice_table,
          'total' => '<div>' . invoice_money_format($total) . '</div>',
          'paid' => '<div>' . invoice_money_format($paid) . '</div>',
          'parking_label' => $p_label,
          'handling_label' => $h_label,
          'total_label' => '<div>' . $stp_invoice_setttings->invoice_total_title .':</div>',
          'credits_label' => '<div>' . $stp_invoice_setttings->invoice_credits_title .':</div>',
          'parking_fee' => $park_fee,
          'handling_fee' => $hand_fee,
          'due_amount' => invoice_money_format($due_amount),
          'invoice_id' => $invoice_id,
          'pay_btn' => $pay_btn_url,
          'stp_inv_id' => $stp_inv_id,
          'invoice_detail_desc' => isset($stp_invoice_setttings->invoice_amount_desc) ? $stp_invoice_setttings->invoice_amount_desc : '',
          'invoice_text' => $invoice_text,
          'invoice_other_info_text' => $invoice_other_info_text,
          )
        );
      }
    endif;
    return theme('invoice_page', array('output' => $output));
  }
  return '';
}

function custom_invoice_payment_request($form, $form_state, $invoice_id, $stp_inv_id) {
  $btn_label = t('Submit Payment <i class="fa fa-chevron-right"></i>');
  global $user;
  $acct_id = $user->name; 
  module_load_include('inc', 'tm_api', 'tm_api.helper');
  drupal_add_css(drupal_get_path('module', 'custom_invoice') . '/css/invoice.css');
  drupal_add_js(drupal_get_path('module', 'custom_invoice') . '/js/plan.js');
  $cc_que_opt = array('cache' => FALSE, 'preprocess' => FALSE);
  drupal_add_js(drupal_get_path('module', 'custom_invoice') . '/js/cc_query.js', $cc_que_opt);
  $user_info = invoice_user_info_block();
  cache_clear_all('invoice-setting-' . $acct_id, 'cache_custom', $wildcard = FALSE);
  $stp_invoice_settings = custom_invoice_get_stored_invoice($stp_inv_id);
  cache_set('invoice-setting-' . $acct_id, $stp_invoice_settings, 'cache_custom');
  $name_label = isset($stp_invoice_settings->invoice_surname) && $stp_invoice_settings->invoice_surname ? $stp_invoice_settings->invoice_surname : 'Last Name';
  $state_label = isset($stp_invoice_settings->invoice_state) && $stp_invoice_settings->invoice_state ? $stp_invoice_settings->invoice_state : 'State';
  $zip_label = isset($stp_invoice_settings->invoice_zipcode) && $stp_invoice_settings->invoice_zipcode ? $stp_invoice_settings->invoice_zipcode : 'Zip';
  $city_label = isset($stp_invoice_settings->invoice_city) && $stp_invoice_settings->invoice_zipcode ? $stp_invoice_settings->invoice_city : 'City';
  $card_typ = stp_payment_invoice_card_types();
  $form = array();  
  if ($user->uid) {
    $acct_id = $user->name;
    if ($acct_id) {
      $user_name = get_user_full_name($user);
      $get_amount_info = custom_invoice_InvoiceList($acct_id, $invoice_id);
      //$get_amount_info =  $invoice_list[$invoice_id];
      cache_clear_all('user_info-' . $acct_id, 'cache_custom', $wildcard = FALSE);
      $cust_info = get_user_info($acct_id);
      if (isset($get_amount_info['invoice_long_descriptions']) && $get_amount_info['invoice_long_descriptions']) {
        drupal_set_title($get_amount_info['invoice_long_descriptions']);
      }
    }
  }

  $form['#attributes']['autocomplete'] = 'off';
  $form['tmuser_info'] = array(
    '#type' => 'hidden',
    '#value' => array(
    'name' => $user_name,
    'acct_id' => $acct_id,
    'invoice_id' => isset($get_amount_info['invoice_ids']) ? $get_amount_info['invoice_ids'] : '' ,
    'due_amt' => isset($get_amount_info['current_due_amounts']) ? $get_amount_info['current_due_amounts'] : ''),
  );
  $form['invoice_id'] = array(
    '#type' => 'hidden',
    '#value' => $invoice_id,
    '#attributes' => array('id' => 'tm_invoice_id'),
  );
  $form['acct_info'] = array(
    '#type' => 'markup',
    '#markup' => $user_info,
    '#weight' => -10,
  );
  $plans = tmapi_get_payment_plans();
  $plans = filter_payment_plans($acct_id, $invoice_id, $plans);
  $plan_default = array(0 => array(
    'payment_plan_id' => 'default',
    'plan_long_name' => 'Full Payment',
    'plan_desc' => 'Full Payment',
    'suppress_payment_plan_details' => 'N'
  ));
  $en_full_payment = (isset($stp_invoice_settings->invoice_en_fullpayment) && $stp_invoice_settings->invoice_en_fullpayment) ? 1 : 0;
  if ($plans && count($plans)) {
    if ($en_full_payment) {
      $plans = array_merge($plan_default, $plans); 
    }    
  }
  else {
    $plans = $plan_default;
  }
  foreach($plans as $plan) {	  
    //Only select active plans
    $options_plans[$plan['payment_plan_id']] = isset($plan['plan_long_name']) ? $plan['plan_long_name'] : $plan['plan_name'];
  }

  if (isset($get_amount_info['payment_plan_id']) && !empty($get_amount_info['payment_plan_id'])) {
    $form['payment_plans_title'] = array(
      '#type' => 'markup',
      '#prefix' => '<div class="col-sm-12 payment-user-form-wrapper"><div class="col-sm-5 col-lg-5 personal-info name-panel">',
      '#markup' => t('<h4 class="invoice-box-head">Selected Payment Plan</h4><br /><br />')
    );
    $form['payment_plans'] = array(
      '#type' => 'markup',
      '#markup' => isset($options_plans[$get_amount_info['payment_plan_id']])?'<div class="payplan">'.$options_plans[$get_amount_info['payment_plan_id']].'</div></br><div class="form-item-payment-plan-id"><div class="DivSelectyze"></div></div>':"No plans are associated with this account",
      '#suffix' => '</div>',
    );
    $form['payment_plan_id'] = array(
      '#type' => 'hidden',
      '#value' => isset($get_amount_info['payment_plan_id']) ? $get_amount_info['payment_plan_id'] : "",
      '#attributes' => array('id'=>'payment_plan_id')
    );
  }
  else {
    $form['payment_plan_id'] = array(
      '#type' => 'select',
      '#prefix' => '<div class="col-sm-12 payment-user-form-wrapper"><div class="col-sm-5 col-lg-5 personal-info name-panel"><h4 class="invoice-box-head">Select Payment Plan</h4>',
      '#options' => $options_plans,
      //'#title' => t('Select Payment Plan'),
      '#suffix' => '</div>',
      '#attributes' => array('id'=>'payment_plan_id')
    );
  } 

  $stored_cc = _tmapi_cc_query();
  $credit_card = $stored_cc['command1']['credit_card']; 
  $stored_cards = array('new_card' => 'Add New Card');
  foreach ($credit_card as $key => $card){    
    $cc_key = $key ."_". $card['data_mask']; //to find the position
    $stored_cards[$cc_key] = $card['data_mask'];    
  }
  $form['choose_card'] = array(
    '#type' => 'select',
    '#title' => t('Choose Card'),
    '#options' => isset($stored_cards) && $stored_cards ? $stored_cards : array('new_card' => 'Add New Card'),
    '#required' => TRUE,
    '#prefix' => '<div class="col-sm-6 col-md-5 col-lg-5 personal-info card-panel pull-right"><h4 class="invoice-box-head">Select Payment Method</h4>',
    '#attributes' => array('tabindex' => '5'),
  ); 

  $form['ctype'] = array(
    '#type' => 'select',
    '#title' => t('Card Type'),
    '#options' => isset($card_typ) && $card_typ ? $card_typ : array('new_card' => 'Add New Card'),
    '#required' => TRUE,
    '#attributes' => array('tabindex' => '6')
  );
  $form['cc_num'] = array(
    '#type' => 'textfield',
    '#title' => t('Card Number *'),
    //'#required' => TRUE,
    '#maxlength' => 16,
    //'#element_validate' => array('element_validate_integer_positive'), 
    '#attributes' => array('tabindex' => '7')
  );
  $form['exp_date'] = array(
    '#type' => 'textfield',
    '#title' => t('Exp. Date'),
    '#maxlength' => 4,
    '#element_validate' => array('element_validate_integer_positive'),
    '#required' => TRUE,
    '#attributes' => array('tabindex' => '8', 'placeholder' => 'mmyy')
  );
  $form['cin'] = array(
    '#type' => 'textfield',
    '#title' => t('CVC'),
    '#required' => TRUE,
    '#maxlength' => 4,
    '#element_validate' => array('element_validate_integer_positive'), 
    '#attributes' => array('tabindex' => '9')
  );



  
  //Billing Address fields
  $form['billing_address_label'] = array(
    '#type' => 'markup',
    //'#prefix' => '<div class="col-sm-12 payment-user-form-wrapper"><div class="col-lg-1 col-sm-1">&nbsp;</div><div class="col-sm-4 col-lg-4 personal-info name-panel">',
    '#markup' => t('<h4 class="invoice-box-head">Billing Information</h4>')
  );
  $form['name_first'] = array(
    '#type' => 'textfield',
    '#title' => t('First Name'),
    '#required' => TRUE,
    '#default_value' => isset($cust_info->customer['name_first']) ? $cust_info->customer['name_first'] : "",
    '#maxlength' => 40,
    //'#prefix' => '<div class="col-sm-6 col-md-5 col-lg-5 personal-info card-panel pull-right"><h4 class="invoice-box-head">Enter Card Details</h4>',
    //'#prefix' => '<div class="col-sm-6 col-md-5 col-lg-5 personal-info card-panel pull-right">',
    '#attributes' => array('tabindex' => '10')
  );
  $form['name_last'] = array(
    '#type' => 'textfield',
    '#title' => t($name_label),
    '#required' => TRUE,
    '#default_value' => isset($cust_info->customer['name_last']) ? $cust_info->customer['name_last'] : "",
    '#maxlength' => 40,
    '#attributes' => array('tabindex' => '11')
  );
   
  $form['street_addr_1'] = array(
    '#type' => 'textfield',
    '#title' => t('Address Line 1'),
    '#required' => TRUE,
    '#default_value' => isset($cust_info->customer['street_addr_1']) ? $cust_info->customer['street_addr_1'] : "",
    '#attributes' => array('tabindex' => '13')
  );
  $form['street_addr_2'] = array(
    '#type' => 'textfield',
    '#title' => t('Address Line 2'),
    '#default_value' => isset($cust_info->customer['street_addr_2']) ? $cust_info->customer['street_addr_2'] : "",
    '#attributes' => array('tabindex' => '14')
  );
  $form['city'] = array(
    '#type' => 'textfield',
    '#title' => t($city_label),   
    '#default_value' => isset($cust_info->customer['city']) ? $cust_info->customer['city'] : "",       
    '#attributes' => array('tabindex' => '15')
  );
  $form['state'] = array(
    '#type' => 'textfield',
    '#title' => t($state_label),
    '#required' => TRUE,
    '#default_value' => isset($cust_info->customer['state']) ? $cust_info->customer['state'] : "",      
    '#attributes' => array('tabindex' => '16')
  );
  $form['zip'] = array(
    '#type' => 'textfield',
    '#title' => t($zip_label),
    '#required' => TRUE,
    '#default_value' => isset($cust_info->customer['zip']) ? $cust_info->customer['zip'] : "",
    '#maxlength' => 10,
    '#attributes' => array('tabindex' => '17')
  );
  $form['email_confirmation'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="conf-email">' . t('Enter your email to receive payment confirmation') . '</div>',
  );
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email:'),
    '#required' => TRUE,
    '#default_value' => isset($cust_info->customer['email']) ? $cust_info->customer['email'] : "",
    '#maxlength' => 100,
    '#suffix' => '</div></div>',
    '#attributes' => array('tabindex' => '18')
  );
  $form['due_amount'] = array(
    '#type' => 'markup',
    '#prefix' => '<div class="col-sm-12 payment-amount-box"><div class="col-sm-6 col-md-5 col-lg-5 amount-due pull-right">',
    '#markup' => '<label>' . t('Total Amount Due:') . '</label><div class="ttlamt">' . invoice_money_format($get_amount_info['current_due_amounts']). '</div>',
    '#suffix' => '',
  );
  $amount_style = (isset($stp_invoice_settings->invoice_hide_enter_amount) && $stp_invoice_settings->invoice_hide_enter_amount) ? 'visibility:hidden;' : '';
  $amount_grey = (isset($stp_invoice_settings->invoice_grey_enter_amount) && $stp_invoice_settings->invoice_grey_enter_amount) ? 1 : 0;
  if ($amount_grey || $amount_style != '') {
    $btn_label = t('Submit <i class="fa fa-chevron-right"></i>');
  }
   $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => 'Enter Amount:',
    '#prefix' => '<div class="make-payment-wrapper"><div style="'. $amount_style .'">',
    '#suffix' => '</div>',
    '#required' => TRUE,
    '#default_value' => $get_amount_info['current_due_amounts'],
    '#maxlength' => 15,
    '#attributes' => array('tabindex' => '19')
  );
  if ($amount_grey) {
    $form['amount']['#attributes']['readonly'] = TRUE;
  }
  if (isset($stp_invoice_settings->invoice_en_terms) && $stp_invoice_settings->invoice_en_terms) {
    $form['terms'] = array(
      '#type' => 'checkbox',
      '#title' => '<small class="txtblk">' . isset($stp_invoice_settings->invoice_terms_header) && $stp_invoice_settings->stp_terms_filter == 0 ? t($stp_invoice_settings->invoice_terms_prefix_text . " " . '<a href = "' . $stp_invoice_settings->invoice_terms_page_link . '" target = _blank>' . $stp_invoice_settings->invoice_terms_header . '</a>' . " " . $stp_invoice_settings->invoice_terms_suffix_text) : t($stp_invoice_settings->invoice_terms_prefix_text . " " . '<a href="#tcmodal" data-toggle="modal" data-target="#terms_conditions">' . $stp_invoice_settings->invoice_terms_header . '</a>' . " " . $stp_invoice_settings->invoice_terms_suffix_text) . '</small>',
//      '#title' => '<small class="txtblk">I accept the <a href="#tcmodal" data-toggle="modal" data-target="#terms_conditions">Terms and Conditions</a></small>',
      '#required' => TRUE,
      '#attributes' => array('tabindex' => '20')
    );
    if($stp_invoice_settings->stp_terms_filter == 1) {
      $termshtml = $stp_invoice_settings->invoice_terms;
      $markup = '<div class="modal fade" id="terms_conditions">
         <div class="modal-dialog">
           <div class="modal-content">
             <div class="modal-header">
               <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
               <h4 class="modal-title">Terms and Conditions</h4>
             </div>
             <div class="modal-body">
               <p>'. $termshtml .'</p>
             </div>
             <div class="modal-footer">
               <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
             </div>
           </div><!-- /.modal-content -->
         </div><!-- /.modal-dialog -->
       </div><!-- /.modal -->';
      $form['terms_information'] = array(
         '#type' => 'markup',
         '#markup' => $markup,
         '#weight' => 5
    );
   }
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $btn_label,
    '#attributes' => array('class' => 'btn-light'),
    '#suffix' => '</div></div></div>',
    '#attributes' => array('tabindex' => '21')
  );
  return $form;
}

/**
 * Validate function for payment request form
 *
 */
function custom_invoice_payment_request_validate(&$form, &$form_state) {
  $ctype = array('VI' => 4, 'MC' => 5, 'AE' => 3, 'DI' => 6);
  $clength = array('VI' => 16, 'MC' => 16, 'AE' => 15, 'DI' => 16);
  $clength_stored = array('VI' => 5, 'MC' => 5, 'AE' => 5, 'DI' => 5);
  $ccin = array('VI' => 3, 'MC' => 3, 'AE' => 4, 'DI' => 3);
  $data = $form_state['values'];
  $type = (isset($data['ctype']) && $data['ctype']) ? $data['ctype'] : '';
  $exp_valid_year = date('y');
  $exp_valid_month = date('m');
  $data_exp_year = substr($data['exp_date'], -2, 2);
  $data_expr_month = substr($data['exp_date'], 0, 2);
  $month_check = ($exp_valid_month <= $data_expr_month) ? 'true' : 'false';
  $stp_inv_settings = custom_invoice_get_stored_invoice(arg(3));
  $partial_payment = isset($stp_inv_settings->invoice_en_partial_payment) && $stp_inv_settings->invoice_en_partial_payment ? $stp_inv_settings->invoice_en_partial_payment : 0;
  // Credit card number validation
  if ((isset($data['cc_num']) && $data['cc_num']) && $type && $data['choose_card'] == 'new_card') {
    if (!is_numeric($data['cc_num']) || strstr($data['cc_num'], ' ')) {
      form_set_error('cc_num', t('Invalid Number entered. Please enter numeric digits only'));
    }
    else if ((strlen($data['cc_num']) != $clength[$type]) || is_float($data['cc_num'])) {
      form_set_error('cc_num', 'Invalid Card Number');
    }
    elseif ($data['cc_num'] && (substr($data['cc_num'], 0, 1) != $ctype[$type])) {
      form_set_error('cc_num', 'Invalid Card Number');
    }
    
  }
  //validate stored cc
  else if ($data['choose_card'] && $data['choose_card'] != 'new_card' && isset($data['cc_num']) && $data['cc_num']) {
    if ((strlen($data['cc_num']) != $clength_stored[$type]) || is_float($clength_stored['cc_num'])) {
      form_set_error('cc_num', 'Invalid Card Number');
    }    
  }
  //  CIN number validation
  if (isset($data['cin']) && $data['cin'] && $type) {
    if ($data['choose_card'] == 'new_card' && !preg_match('/^[0-9]{'. $ccin[$type].'}$/', $data['cin'])) {
      //use {3} for non-AMEX cards
      form_set_error('cin', 'Invalid Card verification Number');
    }
  }
  
  // check for amount value
  if (isset($data['amount'])) {
    if($_POST['payment_plan_id'] == 'default' && $partial_payment == 0){
      if ($data['amount'] < $_POST['amount_due_today']) {
        form_set_error('amount', 'Entered Amount must be greater than or equal to Amount Due Today.');
      }
     }
    if ($data['amount'] < $_POST['amount_due_today'] && $_POST['payment_plan_id'] != 'default') {
       form_set_error('amount', 'Entered Amount must be greater than or equal to Amount Due Today.');
    }
    if ($data['amount'] == 0 && $_POST['payment_plan_id'] == 'default' && $_POST['amount_due_today'] != 0) {
       form_set_error('amount', 'Entered Amount must be greater than Zero.');
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
 * Drupal form submit handler for payment request form
 */
function custom_invoice_payment_request_submit(&$form, &$form_state) {
  module_load_include('inc', 'tm_api', 'tm_api.helper');
  global $user;
  $acct_id = $user->name;
  $stp_inv_id = arg(3);
  $site_mail = variable_get('site_mail', ini_get('sendmail_from'));
  $data = $form_state['values'];
  $data['address'] = '';
  if (isset($data['street_addr_1']) && $data['street_addr_1']) {
    $data['address'] .= $data['street_addr_1'];
  }
  $response = array();
  watchdog('tmapi_payment', 'TM Payment request API for ACCT_ID. %data', array('%data' => $data['tmuser_info']['acct_id']), WATCHDOG_NOTICE, NULL);
  $tm_response = _tmapi_payment_request($data);
  $cc_num = '************'. substr($data['cc_num'], -4, 4);
  $data['tm_raw_response'] = '';
  //print_r($tm_response['response']); exit();
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

      //Try to store the new/updated credit card details.
      if (isset($data['choose_card']) && $data['choose_card'] == 'new_card') {
        $data['cc_num_full'] = $form_state['values']['cc_num']; //We need full credit card number
      }

      $res = _tmapi_cc_update_cc_query($acct_id, $data);
      if ($res) {
        watchdog('_tmapi_cc_update_cc_query', 'TM CC_UPDATE add Success for ACCT_ID. %data', array('%data' => $data['tmuser_info']['acct_id']), WATCHDOG_NOTICE, NULL);          
      }
      $cid = 'get_cc_query_'.$acct_id;
      cache_clear_all($cid, 'cache', TRUE);
  
      //notify user about the payment received.
      success_payment_user_notification($data, $cc_num, $site_mail, $stp_inv_id);
      //notify admin about the payment received.
      if ($notify_email = variable_get('invoice_payment_notify_mail', '')) {
        success_payment_admin_notification($data, $cc_num, $notify_email,  $site_mail);
      }
      _update_tm_payment_transaction_table($data); 
       drupal_goto('invoice/' . $data['tmuser_info']['invoice_id'] .'/payment/' . $stp_inv_id . '/success');
    }
    // Payment Failure
    else {
      watchdog('tmapi_payment', 'TM Payment Failure for ACCT_ID. %data', array('%data' => $data['tmuser_info']['acct_id']), WATCHDOG_NOTICE, NULL);
      _update_tm_payment_transaction_table($data);
      // Payment request failure mail
      failure_payment_admin_notification($data, $cc_num, $site_mail);
      if (function_exists('custom_commons_tm_payment_transaction_message')) {
        $message = custom_commons_tm_payment_transaction_message($data['tm_cc_results']);
        $_SESSION['failure_message'] = $message;
      }
      drupal_goto('invoice/' . $data['tmuser_info']['invoice_id'] .'/payment/' . $stp_inv_id . '/failure');
    }
  }
  else {
    watchdog('tmapi_payment', 'TM Payment Failure due to no response from TM for ACCT_ID. %data ', array('%data' => $data['tmuser_info']['acct_id']), WATCHDOG_NOTICE, NULL);
    // API Failure mail
    api_failure_mail($data, $cc_num, $site_mail);
    drupal_goto('invoice/' . $data['tmuser_info']['invoice_id'] .'/payment/' . $stp_inv_id . '/failure');
  }
  $cc_num = '';
}

/**
 * Ticketmaster Payment Success
 */
function custom_invoice_payment_success($invoice_id, $stp_inv_id) {
  //module_load_include('inc', 'custom_block', 'custom_block.block');
  drupal_add_css(drupal_get_path('module', 'custom_invoice') . '/css/invoice.css');
  global $user;
  $roles_curr = array_flip($user->roles);
  // Invoice Page Title
  if ($user->uid) {
    $acct_id = $user->name;
    $get_amount_info = custom_invoice_InvoiceList($acct_id, $invoice_id);
    //$get_amount_info =  $invoice_list[$invoice_id];
    if (isset($get_amount_info['invoice_long_descriptions']) && $get_amount_info['invoice_long_descriptions']) {
      drupal_set_title($get_amount_info['invoice_long_descriptions']);
    }
  }
  $stp_success_message = check_markup(custom_invoice_get_success_msg($stp_inv_id), 'full_html', $langcode = '', $cache = FALSE);
  $user_info = invoice_user_info_block();
  return theme('tm_payment_success', array('user_info' => $user_info, 'acct_id' => $user->name, 'stp_invoice_message' => $stp_success_message));
}

/**
 * Ticketmaster Payment failure
 */
function custom_invoice_payment_failure($invoice_id, $stp_inv_id) {
  global $user;
  drupal_add_css(drupal_get_path('module', 'custom_invoice') . '/css/invoice.css');
  // Invoice Page Title
  if ($user->uid) {
    $acct_id = $user->name;
    $get_amount_info = custom_invoice_InvoiceList($acct_id, $invoice_id);
    //$get_amount_info =  $invoice_list[$invoice_id];
    if (isset($get_amount_info['invoice_long_descriptions']) && $get_amount_info['invoice_long_descriptions']) {
      drupal_set_title($get_amount_info['invoice_long_descriptions']);
    }
  }
  $stp_invoice_setttings = custom_invoice_get_stored_invoice($stp_inv_id);
  $stp_invoice_message = isset($stp_invoice_setttings->invoice_payment_failure_body) ? $stp_invoice_setttings->invoice_payment_failure_body : '';
  module_load_include('inc', 'custom_block', 'custom_block.block');
  $user_info = invoice_user_info_block();
  return theme('tm_payment_error', array('user_info' => $user_info, 'acct_id' => $user->name, 'stp_invoice_message' => $stp_invoice_message));
}

/**
 * update TM API payment success update
 */
function success_payment_user_notification($data=array(), $cc_num=NULL, $site_mail=NULL, $stp_inv_id=NULL) {
  if ((count($data)>0) && (isset($cc_num) && !empty($cc_num)) && (isset($site_mail) && !empty($site_mail))) { 
  $template_format = custom_invoice_get_stored_invoice($stp_inv_id);
  $stp_invoice_setttings = node_load($template_format->payment_template);
    $params = array();
    $to = $data['email'];
    $params['subject'] = (isset($stp_invoice_setttings->field_subject['und'][0]['value']) && $stp_invoice_setttings->field_subject['und'][0]['value']) ? $stp_invoice_setttings->field_subject['und'][0]['value'] : variable_get('invoice_mail_subject', t('2015 STP Invoice'));
    $params['body']= (isset($stp_invoice_setttings->body['und'][0]['value']) && $stp_invoice_setttings->body['und'][0]['value']) ? t($stp_invoice_setttings->body['und'][0]['value'], array('%amount' => invoice_money_format($data['amount']), '%cc_num' => $cc_num)) : variable_get('invoice_mail_body', t('Attached to this email is a copy of your invoice for the 2016 STP Season Subscription.'));
  
//    $params['subject'] = variable_get('invoice_payment_subject', t('Thanks For Your Payment'));
//    $params['body'] = t(variable_get('invoice_payment_body', 'Thanks for your payment.'), array('%amount' => $data['amount'], '%cc_num' => $cc_num));
    $params['headers']['Bcc'] = 'rkumar@io-media.com,pmanas@io-media.com,amishra@io-media.com,debraj.naik@io-media.com';    
    drupal_mail('custom_hooks', 'stpmail_template', $to, language_default(), $params);
//    drupal_mail('custom_hooks', 'tm_payment_notify', $data['email'], LANGUAGE_NONE, $params, $site_mail);
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
    //$to = 'award@io-media.com, rkumar@io-media.com, adeb@io-media.com, pmanas@io-media.com';
    $to = '';
    if (variable_get('invoice_payment_failure_additional_mail', '')) {
      $to = variable_get('invoice_payment_failure_additional_mail', '');
      $to_array = explode(',', $to);
      $to_array = array_map('trim',$to_array);	 
      $to = implode(',', array_unique($to_array));
    $params['subject'] = t('Payment Request failed for Account -  %acct_id', array('%acct_id' => $data['tmuser_info']['acct_id']));
    $params['body'] = t('Payment Request has been failed for Acct_ID %acct_id with card ending in %cc_num<br/><br/>TM Raw Response: %response', array('%acct_id' => $data['tmuser_info']['acct_id'], '%cc_num' => $cc_num, '%response' => $data['tm_raw_response']));
    drupal_mail('custom_hooks', 'tm_payment_notify', $to, LANGUAGE_NONE, $params, $site_mail);
  }
}
}

/**
 * update TM API failure admin update
 */
function api_failure_mail($data=array(), $cc_num=NULL, $site_mail=NULL){
  if((count($data)>0) && (isset($cc_num) && !empty($cc_num)) && (isset($site_mail) && !empty($site_mail))) {
    // API Failure mail
    //$to = 'award@io-media.com, rkumar@io-media.com, adeb@io-media.com, pmanas@io-media.com';
    $to = '';
    if (variable_get('invoice_payment_failure_additional_mail', '')){
      $to = variable_get('invoice_payment_failure_additional_mail', '');
      $to_array = explode(',', $to);
      $to_array = array_map('trim',$to_array);	 
      $to = implode(',', array_unique($to_array));
    $params['subject'] = t('No TM Response: Payment Request failed for Account- %acct_id', array('%acct_id' => $data['tmuser_info']['acct_id']));
    $params['body'] = t('Payment Request has been failed for Acct_ID %acct_id with card ending in %cc_num<br/> Resaon: No TM Response', array('%acct_id' => $data['tmuser_info']['acct_id'], '%cc_num' => $cc_num));
    drupal_mail('custom_hooks', 'tm_payment_notify', $to, LANGUAGE_NONE, $params, $site_mail);
  }
}
}

/**
 * update tm_payment_transaction_table update
 */
function _update_tm_payment_transaction_table($data) {
  db_insert('tm_payment_transaction_detail')
  ->fields(array(
    'first_name' => (isset($data['name_first']) && $data['name_first']) ? $data['name_first'] : '',
    'last_name' => (isset($data['name_last']) && $data['name_last']) ? $data['name_last'] : '',
    'address' => (isset($data['street_addr_1']) && $data['street_addr_1']) ? $data['street_addr_1'] : '',
    'zip' => (isset($data['zip']) && $data['zip']) ? $data['zip'] : '',
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


/*
* function to fetch payment plan for an invoice
* New onchange of Plans
* @return json response of the plans
*/
function custom_invoice_payment_plans_desc($pid, $invoice_id) {
  module_load_include('inc', 'tm_api', 'tm_api.helper');
  global $user;
  
  if (!is_numeric($pid) && ($pid!='default')) {
    return "";
  }
  global $user;
  if ($user->uid) {
    $acct_id = $user->name;
    if ($acct_id) {
      $invoice_list = get_invoice_list($acct_id, 0);
      $get_amount_info =  $invoice_list[$invoice_id];
    }
  }
  $plans = tmapi_get_payment_plans();
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
  if(isset($plandesc) && $plandesc){
    $ops['desc'] = $plandesc;
  }
  else{
    $ops['desc'] = "No description available for this plan.";
  }
  drupal_json_output($ops);
  exit;
}

/**
 *
 * function to filter payment plans for an invoice
 */
function filter_payment_plans($acct_id, $invoice_id, $plans) {
  $pay_plans = "";
  $payment_plans = $plan_schedules = array();
  if (
    (isset($acct_id) && $acct_id) &&
    (isset($invoice_id) && $invoice_id)
  ) {
    module_load_include('inc', 'tm_api', 'tm_api.helper');
    $pay_plans = tmapi_get_payment_plans_for_invoice($acct_id, $invoice_id);
    $payment_plans = explode(',', $pay_plans);
    //add default plan for full payment
    //comment the line below if you want to exclude
    //full payment option
    array_unshift($payment_plans, "default");
    if ($plans) {
      foreach ($plans as $plan) {
        if (in_array($plan['payment_plan_id'], $payment_plans)) {
          $plan_schedules[$plan['payment_plan_id']] = $plan;
          $sort_arr[$plan['payment_plan_id']] = $plan['plan_desc'];
        }
      }
      arsort($sort_arr);
      foreach ($sort_arr as $sa_key => $sa_value) {
        $plans_sch[$sa_key] = $plan_schedules[$sa_key];
      }
      return $plans_sch;
    }
    else{
      return FALSE;
    }
  }
  else{
    return FALSE;
  }
} 

/*
 * function which will fetch the invoice_list of a invoice_id and return the Invoice list data
 * @param $acct_id account ID of the user
 * @param $invoice_id integer invoice_id of the invoice
 * @return array invoice_list data of invoice
 *
 */
function custom_invoice_InvoiceList($acct_id, $invoice_id) {
  $invoices = get_invoice_list($acct_id, 0);
  return $invoices[$invoice_id];
}

function custom_invoice_get_stored_invoice($stp_inv_id) {
  $stp_invoice = cache_get('invoice-' . $stp_inv_id, 'cache_custom');
  if (!$stp_invoice) {
    $stp_invoice = db_query("SELECT * FROM {stp_invoice} WHERE invoice_status = 1 AND id = :id", array(':id' => $stp_inv_id))->fetchObject();
    $invoice_list_query = db_select('stp_invoice_details', 'n');
    $invoice_list_query->fields('n');
    $invoice_list_query->condition('n.inv_conf_id', $stp_inv_id);
    $invoice_list_query->condition('n.enable', 1);
    $invoice_list_query->orderby('n.weight');
    $invoice_data = $invoice_list_query->execute()->fetchAll();
    $stp_invoice->details = $invoice_data;
    cache_set('invoice-' . $stp_inv_id, $stp_invoice, 'cache_custom');
    return $stp_invoice;
  }
  return $stp_invoice->data;
}

function custom_invoice_get_stored_mob_invoice($stp_inv_id) {
  $stp_invoice = cache_get('invoice-mob-' . $stp_inv_id, 'cache_custom');
  if (!$stp_invoice) {
    $stp_invoice = db_query("SELECT * FROM {stp_invoice} WHERE invoice_status = 1 AND id = :id", array(':id' => $stp_inv_id))->fetchObject();
    $invoice_list_query = db_select('stp_invoice_details', 'n');
    $invoice_list_query->fields('n');
    $invoice_list_query->condition('n.inv_conf_id', $stp_inv_id);
    $invoice_list_query->condition('n.mob_enable', 1);
    $invoice_list_query->orderby('n.weight');
    $invoice_data = $invoice_list_query->execute()->fetchAll();
    $stp_invoice->details = $invoice_data;
    cache_set('invoice-mob-' . $stp_inv_id, $stp_invoice, 'cache_custom');
    return $stp_invoice;
  }
  return $stp_invoice->data;
}

/*
 * Get payment methods(Card Types)
 * @ API System Options
 */

function stp_payment_invoice_card_types() {
  $vi = variable_get('visa_card') == 1 ? 'Visa' : '';
  $ae = variable_get('amex_card') == 1 ? 'American Express' : '';
  $di = variable_get('discover_card') == 1 ? 'Discover' : '';
  $mc = variable_get('master_card') == 1 ? 'MasterCard' : ''; 
  $c_type = array('VI' => $vi, 'MC' => $mc, 'AE' => $ae, 'DI' => $di);
  $c_type = array_filter($c_type);
  return $c_type;
}

/*
 * Function callback for typeform data
 */
function custom_invoice_get_stored_typeform($invoice_id, $inv_conf_id, $node) {
  $lang = isset($node->language) ? $node->language : LANGUAGE_DEFAULT;
  if (function_exists('custom_typeform_access')) {
    $typeform_access = custom_typeform_access('view', $node);
    if (isset($typeform_access) && $typeform_access == 1) {
      if(isset($node->field_typeform_type[$lang][0]['value']) && ($node->field_typeform_type[$lang][0]['value'] == 'invoice')) {
        $typeform_title = $node->title;
        drupal_set_title($typeform_title);
        $typeform_title = $node->field_typeform_type[$lang][0]['value'];
        $node_view = node_view($node);
        $rendered_node = drupal_render($node_view);
       }
       return $rendered_node;
    }
  }
  else{
      drupal_goto('/invoice/'. $invoice_id .'/payment/' . $inv_conf_id);
  }
}

function custom_invoice_get_assigned_typeform($stp_inv_id) {
  global $user;
  $typeform_data = cache_get('stp-typeform-' . $stp_inv_id, 'cache_custom')->data;
  $roles_curr = array_flip($user->roles);
  $typeform_nid = 0;
  if (!$typeform_data) {
  $typeform_qry = db_select('stp_invoice_typeform', 'n');
    $typeform_qry->fields('n');
    $typeform_qry->condition('n.inv_conf_id', $stp_inv_id);
    $typeform_data = $typeform_qry->execute()->fetchAll();
    cache_set('stp-typeform-' . $stp_inv_id, $typeform_data, 'cache_custom');
  }
  foreach($typeform_data as $typefrm){
    $accnt_type = explode(',', $typefrm->acct_grp);
      foreach($accnt_type as $type){
        if(in_array($type, $roles_curr)){
          $typeform_nid =  $typefrm->typeform_nid;
        }
      }
    }
    $typeform_node = node_load($typeform_nid);
      if (function_exists('custom_typeform_access')) {

    $typeform_access = custom_typeform_access('view', $typeform_node);
  }
    if(isset($typeform_access) && $typeform_access == 1) {
      return $typeform_nid;
    }
  return '';
}
/*
 * Function callback for stored success message
 */

function custom_invoice_get_success_msg($stp_inv_id) {
  global $user;
  $roles_curr = array_flip($user->roles);
  $thankyou_success = cache_get('stp-success-' . $stp_inv_id, 'cache_custom')->data;
  if(!$thankyou_success) {
    $thankyou_msg = db_select('stp_invoice_thankyou', 'n');
    $thankyou_msg->fields('n');
    $thankyou_msg->condition('n.inv_conf_id', $stp_inv_id);
    $thankyou_success = $thankyou_msg->execute()->fetchAll();
    cache_set('stp-success-' . $stp_inv_id, $thankyou_success, 'cache_custom');
  }
  foreach($thankyou_success as $success){
    $accnt_type = explode(',', $success->acct_grp);
      foreach($accnt_type as $type){
        if(in_array($type, $roles_curr)){
          $stp_invoice_message =  $success->message;
        }
      }
    }
  return $stp_invoice_message;   
}

function stp_strpos_array($haystack, $needles) {
  if (is_array($needles)) {
    foreach ($needles as $str) {
      $pos = strpos($haystack, $str);
      if ($pos !== FALSE) {
         return true;
      }
    }
  }
  return false;
}

/**
 * function, all we do here is select the element and return it to be updated.
 * @return renderable value in the second textbox
 */
function custom_invoice_ajax_callback($form, $form_state) {
    return $form['ctype'];
}

/**
 * custom ajax function, implement cc_query.
 * @return json output if credit card match
 */
function custom_invoice_get_cc_query($cc_pos = 0){
  if ($cc_pos == 'new_card') {
    global $user;
    if ($user->uid && $acct_id = $user->name) {
      $cust_info = get_user_info($acct_id);
      $customer = $cust_info->customer;
      if ($customer) {
        drupal_json_output($customer);
        exit;
      }
      else {
        echo "";
        exit;
      }
    }
  }
  else {
    $stored_cc = _tmapi_cc_query();  
    $credit_card = $stored_cc['command1']['credit_card'];
    
    $stored_cards = array();
    foreach ($credit_card as $key => $card) {    
      $cc_key = $key ."_". $card['data_mask']; //prepare to find the position
      $stored_cards[$cc_key] = $card;
    }
    $stored_cc = $stored_cards[$cc_pos];
    if ($stored_cc) {
      drupal_json_output($stored_cc);
      exit;
    }
    else {
        echo "";
        exit;
    }
  }  
}

