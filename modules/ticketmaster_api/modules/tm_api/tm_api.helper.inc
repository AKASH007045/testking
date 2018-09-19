<?php

// @file

/**
 * function to rest rerquest by curl on ticketmaster server.
 * @param $ch1
 *   object - return by curl_init() callback.
 * @param $cookies_file
 *   string - filepath curl cookie file.
 * @param $json
 *   string - json for ticketmaster header and command1 data.
 * @return
 *   array - response array
 *    - httpcode - response http code.
 *    -  - response string.
 *     
 **/
function _custom_tm_rest_call($ch1, $cookies_file, $json = NULL, $web_url = NULL) {
  $dsn = variable_get('tm_dsn', '');
  //ticketmaster url
  //$adb_url = variable_get('tm_login_url', 'https://ws.ticketmaster.com/archtics/ats/ticketing_services.aspx?dsn=cowboys');
  $adb_url = 'https://ws.ticketmaster.com/archtics/ats/' . $web_url . '?dsn=' . $dsn;
  // set header for ticketmaster
  $header = array(
      'Content-Type: text/json; charset=utf-8',
      'Content-Length:' . strlen($json),
      'Connection: Keep-Alive',
      'Keep-Alive: timeout=15, max=100'
  );
  $cert = realpath(drupal_get_path('module', 'tm_api') . '/certificates/live_tm_v2.pem');
  $cert_type = variable_get('tm_cert_type', 'Linux');
  if ($cert_type == 'Windows') {
    $cert = realpath(drupal_get_path('module', 'tm_api') . '/certificates/windows/live.iomedia_tm_v2.pfx');
  }
  
  
  //ticketmaster curl request otions 
  $adb_option_defaults = array(
      CURLOPT_URL => $adb_url,
      CURLOPT_CUSTOMREQUEST => 'POST',
      CURLOPT_HTTPHEADER => $header,
      CURLOPT_POSTFIELDS => $json, // post data.
      CURLOPT_HEADER => 0,
      CURLOPT_RETURNTRANSFER => TRUE,
      CURLOPT_TIMEOUT => 30,
      //ticketmaster ssl options
      CURLOPT_SSL_VERIFYPEER => TRUE,
      CURLOPT_SSL_VERIFYHOST => 2,
      CURLOPT_SSLCERT => $cert,
      CURLOPT_SSLCERTPASSWD => 'letmein',
      // curl cookies file.
      CURLOPT_COOKIEJAR => $cookies_file,
  );
  curl_setopt_array($ch1, $adb_option_defaults);
  $response = curl_exec($ch1);
  $httpcode = curl_getinfo($ch1, CURLINFO_HTTP_CODE);
  $header_size = curl_getinfo($ch1, CURLINFO_HEADER_SIZE);
  return array('httpcode' => $httpcode, 'response' => $response);
}

/**
 * function to rest rerquest by curl on ticketmaster server.
 * @param $ch1
 *   object - return by curl_init() callback.
 * @param $cookies_file
 *   string - filepath curl cookie file.
 * @param $json
 *   string - json for ticketmaster header and command1 data.
 * @return
 *   array - response array
 *    - httpcode - response http code.
 *    -  - response string.
 *     
 **/
function _custom_tm_rest_call_test($ch1, $cookies_file, $json = NULL, $web_url = NULL) {
  $api_sufix="";
  if (variable_get('tm_api_test', 0)) {
    $api_sufix="_test";
  }
  $dsn = variable_get('tm_dsn' . $api_sufix, '');
  //ticketmaster url
  $adb_url = variable_get('tm_login_url_test', 'https://ws-test.ticketmaster.net/archtics/ats/ats.aspx?dsn=bucs602');
 // $adb_url = 'https://ws-test.ticketmaster.net/archtics/ats/'. $web_url .'?dsn=' . $dsn;
  // set header for ticketmaster
  $header = array(
      'Content-Type: text/json; charset=utf-8',
      'Content-Length:' . strlen($json),
      'Connection: Keep-Alive',
      'Keep-Alive: timeout=15, max=100'
  );
   $adb_option_defaults = array(
      CURLOPT_URL => $adb_url,
      CURLOPT_CUSTOMREQUEST => 'POST',
      CURLOPT_HTTPHEADER => $header,
      CURLOPT_POSTFIELDS => $json, // post data.
      CURLOPT_HEADER => 0,
      CURLOPT_RETURNTRANSFER => TRUE,
      CURLOPT_TIMEOUT => 30,
      CURLOPT_PORT => 443,
    //  CURLOPT_SSLVERSION => 3, 
      //ticketmaster ssl options
      CURLOPT_SSL_VERIFYPEER => 0,
      CURLOPT_SSL_VERIFYHOST => 0,
      CURLOPT_SSLCERT => realpath(drupal_get_path('module', 'tm_api') . '/certificates/test_tm_v2.pem'),
      CURLOPT_SSLCERTPASSWD => 'LetMeIn',
      // curl cookies file.
      CURLOPT_COOKIEJAR => $cookies_file,
  );
  curl_setopt_array($ch1, $adb_option_defaults);
  $response = curl_exec($ch1);
  $info = curl_getinfo($ch1);
  $error= curl_error($ch1);
 
  $httpcode = curl_getinfo($ch1, CURLINFO_HTTP_CODE);
  $header_size = curl_getinfo($ch1, CURLINFO_HEADER_SIZE);
  return array('httpcode' => $httpcode, 'response' => $response, 'error' => $error, 'info' => $info);
}

/**
 * Function to get the customer_query response
 * @param $acct_id numeric Account ID
 * @param $pin string Pin
 * @return TM response
 */
function _tmapi_customer_query($acct_id = NULL, $pin = NULL) {
  // customer query
  $param = tm_api_get_command_param('customer_query', $acct_id);
  $param['command1']['pin'] = $pin;
  $json = drupal_json_encode($param);
  $response = tm_api_call($json, 'ticketing_services.aspx');
  $data = drupal_json_decode($response['response']);
  if (isset($response['response_ok']) && $response['response_ok'] && isset($data['command1'])) {
    return $data['command1'];
  }
  return 0;
}

/**
 * Function to get the customer_query response
 * @param $acct_id numeric Account ID
 * @param $pin string Pin
 * @return TM response
 */
function _tmapi_customer_query_without_password($acct_id = NULL) {
  // customer query
  $param = tm_api_get_command_param('customer_query', $acct_id);
  $param['command1']['call_reason'] = 'S';
  $json = drupal_json_encode($param);
  $response = tm_api_call($json, 'ticketing_services.aspx');
  $data = drupal_json_decode($response['response']);
  if (isset($response['response_ok']) && $response['response_ok'] && isset($data['command1'])) {
    return $data['command1'];
  }
  return 0;
}
/**
 * Function to get the STH STATUS of a TM account
 * @param integer $acct_id Account ID of the customer
 * @return array API response
 */
function _tmapi_get_sth_status($acct_id = NULL, $item_set = 'IOM-STH-SET') {
  if ((int) $acct_id) {
    $api_status = variable_get('tm_api_sth_status', 'get_sth_status');
    $param = tm_api_get_command_param($api_status, $acct_id);
    $param['command1']['item_set'] = '#'.$item_set;
	
    $json = drupal_json_encode($param);
    $response = array();
    $response = tm_api_call($json, 'ticketing_services.aspx');
    //check for sth status and vali with response.

    $data = drupal_json_decode($response['response']);
    if (isset($response['response_ok']) && $response['response_ok'] && isset($data[0]['sth_status'])) {
    
      return $data;
    }
  }
  return 0;
}
/**
 * TM API call to fetch account rep of user
 * @param integer $acct_id account ID of the user
 * @return array Account Rep information
 */
/**
 * TM API call to fetch account rep of user
 * @param integer $acct_id account ID of the user
 * @return array Account Rep information
 */
function _tmapi_get_acct_rep($acct_id = NULL) {
  if ((int) $acct_id) {
    $param = tm_api_get_command_param('get_acct_rep', $acct_id);
    unset($param['command1']['ref']);
    unset($param['command1']['site_name']);
    //if all reps parameter is enabled
    if (variable_get('en_all_reps_api', 0)) {
      $param['command1']['all_reps'] = 'y';
    }
    $json = drupal_json_encode($param);
    $response = array();
    $response = tm_api_call($json, 'ticketing_services.aspx');
    //check for acct_rep status and vali with response.
    $result = 0;
    if (isset($response['response_ok']) && $response['response_ok']) {
      $data = drupal_json_decode($response['response']);
      if (!empty($data[0]['rep_acct_id']) && isset($data[0]['rep_acct_id'])) {
        //if all reps parameter is enabled then we will fetch the matching title
        if (variable_get('en_all_reps_api', 0)) {
          foreach($data as $key => $rep) {
            if ($rep['rep_type'] == variable_get('all_rep_type_title', '')) {
              $rep_data = $rep;
              break;
            }
          }
        }
        else {
          $rep_data = $data[0];
        }
        // We will up the account rep cache
        cache_clear_all('account_rep_' . $acct_id, 'cache_custom');
        // Account rep information is cached
        cache_set('account_rep_' . $acct_id, $rep_data, 'cache_custom');
        return $data;
      }
    }
  }
  return 0;
}

/**
 * TM invoice_list API to fetch invoice id 
 */
function _tmapi_cc_up_call($acct_id = NULL, $udata = NULL) {
  $cust_name_id = $_SESSION['cust_name_id'];
  $param = tm_api_get_command_param('cc_up', $acct_id);
  if (!variable_get('tm_api_test', 0)) {
    $param['header']['archtics_version'] = 'V999';
  }
  $param['command1']['cc_num'] = $udata['cc_num'];
  $param['command1']['cc_type'] = $udata['ctype'];
  $param['command1']['cc_exp'] = $udata['exp_'];
  $param['command1']['action'] = 'X';
  $param['command1']['cust_name_id'] = $cust_name_id;
  $json = drupal_json_encode($param);
  $response = tm_api_call($json, 'ticketing_services.aspx');
  //check for cc_up call status and up the transaction table with response.
  $result = 0;
  if (isset($response['response_ok']) && $response['response_ok']) {
    $data = drupal_json_decode($response['response']);
    $udata['command'] = 'cc_up';
    if (!isset($data['command1']['error_detail']) && isset($data['command1']['cc_seq_num']) && $data['command1']['cc_seq_num']) {
      $result = $data['command1']['cc_seq_num'];
      $udata['result'] = '1';
    }
    // We will up the cc_up transaction logs
    $udata['acct_id'] = $acct_id;
    _pawp_up_tm_transcation_table($udata, $response['response']);
  }
  return $result;
}

/**
 * TM invoice_list API to fetch invoice id
 * @acct_id integer account ID of the TM user
 * @udata user posted data
 */
function _tmapi_customer_up_call($acct_id = NULL, $udata = NULL, $cust_cc_seq_num = '', $flag = NULL) {
  $pass = isset($_SESSION['tmupa']) ? base64_decode($_SESSION['tmupa']) : '';
  // If flag is set to up customer on "No Thanks" or for successful cc_up customer sequential number exists
  if ($flag == 'NO' || $cust_cc_seq_num) {
    $param = tm_api_get_command_param('customer_up', $acct_id);
    $param['command1']['pin'] = $pass;
    $customer = $_SESSION['cust_info'];
    if (!empty($customer)) {
      $param['command1'] = array_merge($param['command1'], $customer);
    }
    $param['command1']['customer']['acct_id'] = $acct_id;
    $param['command1']['other_info_10'] = $cust_cc_seq_num;
    $param['command1']['other_info_4'] = (isset($cust_cc_seq_num) && $cust_cc_seq_num) ? 'YES' : 'NO';
    $json = drupal_json_encode($param);
    $api_sufix="";
    $response = array();
    $response = tm_api_call($json, 'ticketing_services.aspx');
    //check for sth status and vali with response.
    $result = 0;
    if (isset($response['response_ok']) && $response['response_ok']) {
      $data = drupal_json_decode($response['response']);
      $udata['command'] = 'customer_up';
      if (!isset($data['command1']['error_detail']) && isset($data['command1']['result']) && $data['command1']['result'] == 0) {
        $result = 1;
        $udata['result'] = (isset($flag) && ($flag == 'NO')) ? -1 : 1;
      }
      _pawp_up_tm_transcation_table($udata, $response['response']);
    }
    return $result;
  }
  return 0;
}

/******************************************************** INVOICE PAGE**************************************/

/**
 * Function to get the Invoice Detail of a TM account
 */
function tmapi_get_sth_invoice($acct_id = NULL) {
  $invoice_id = _tmapi_get_invoice_id($acct_id);
  // Check if invoice id exists then we fetch the invoice of TM account
  if ($invoice_id) {
    $param = tm_api_get_command_param('invoice_details', $acct_id);
    $param['command1']['invoice_id'] = $invoice_id;
    $json = drupal_json_encode($param);
    $response = array();
    $response = tm_api_call($json, 'ticketing_services.aspx');
    //check for sth status and vali with response.
    $rows = array();
    if (isset($response['response_ok']) && $response['response_ok']) {
      $data = drupal_json_decode($response['response']);
      if (isset($data['command1']['items']) && isset($data['command1']['result']) && $data['command1']['result'] == 0) {
        foreach ($data['command1']['items'] as $key => $row) {
          $row['acct_id'] = $acct_id;
          $row['invoice_id'] = $invoice_id;
          $row['due_date'] = $data['command1']['due_date'];
          $row['invoice_date'] = isset($data['command1']['invoice_date']) ? $data['command1']['invoice_date'] : '';
          $rows[] = $row;
        }
        return $rows;
      }
    }
  }
  return 0;
}

/**
 * TM invoice_list API to fetch invoice id
 * 
 */
function _tmapi_get_invoice_id($acct_id = NULL) {
  $param = tm_api_get_command_param('invoice_list', $acct_id);
  $json = drupal_json_encode($param);
  $response = array();
  $response = tm_api_call($json, 'ticketing_services.aspx');
  //check for invoice id vali with response.
  $result = 0;
  if (isset($response['response_ok']) && $response['response_ok']) {
    $data = drupal_json_decode($response['response']);
    if (!empty($data['command1']['invoices']) && isset($data['command1']['result']) && $data['command1']['result'] == 0) {
      foreach ($data['command1']['invoices'] as $key => $invoice) {
        $temp_invoices[$invoice['invoice_ids']] = $invoice;
        $invoices[] = $invoice['invoice_ids'];
      }
      $invoice_ids = max($invoices);
      $_SESSION['invoice_detail'] = isset($temp_invoices[$invoice_ids]) ? $temp_invoices[$invoice_ids] : "";
      return max($invoices);
    }
  }
  return 0;
}

function _tmapi_payment_request_old($data) {
  global $user;
  $acct_id = isset($data['tmuser_info']['acct_id']) ? $data['tmuser_info']['acct_id'] : $user->name;
  $invoice_id = isset($data['tmuser_info']['invoice_id']) ? $data['tmuser_info']['invoice_id'] : '';
  $order_id = isset($data['tmuser_info']['order_num']) ? $data['tmuser_info']['order_num'] : '';
  $param = tm_api_get_command_param('payment_request', $acct_id);
  if ($invoice_id) {
    $param['command1']['invoice_id'] = $invoice_id;
    $param['command1']['calling_reason'] = 'P';
    $param['command1']['cart_items']['order_line_item'] = 1;
	$param['command1']['cart_items']['dm_code'] = '';
  }
  elseif ($order_id) {
    $param['command1']['order_num'] = $order_id;
    $order_line_item = isset($data['tmuser_info']['order_line_item']) ? $data['tmuser_info']['order_line_item'] : 1;
    $param['command1']['calling_reason'] = 'B';
    $param['command1']['cart_items'] = $data['tmuser_info']['order_line_item'];
  }
  

  //if "default" payment plan id is selected then
  //payment plan id will not be added in the TM request
  if (isset($data['payment_plan_id']) && $data['payment_plan_id'] != 'default' && $data['payment_plan_id'] != '65') {
    $param['command1']['payment_plan_id'] = $data['payment_plan_id'];
  }
  elseif (isset($data['payment_plan_id']) && $data['payment_plan_id'] == '65' && $data['amount'] == 0) {
    $param['command1']['payment_plan_id'] = $data['payment_plan_id'];
  }
  $customer_info = get_user_info($acct_id);
  $customer = $customer_info->customer;
  $customer['name_first'] = (isset($customer['name_first']) && $customer['name_first']) ? $customer['name_first'] : '';
  $customer['name_last'] = (isset($customer['name_last']) && $customer['name_last']) ? $customer['name_last'] : '';
  $customer['zip'] = (isset($customer['zip']) && $customer['zip']) ? $customer['zip'] : '';
  $customer['address'] = (isset($customer['street_addr_1']) && $customer['street_addr_1']) ? $customer['street_addr_1'] : '';
  //$param['command1']['calling_reason'] = 'P';
  $param['command1']['mop'][0]['auth_mode'] = 'O';
  $param['command1']['mop'][0]['amount'] = $data['amount'];
  $param['command1']['mop'][0]['pmt_type'] = $data['ctype'];
  $param['command1']['mop'][0]['credit_card']['cc_num'] = $data['cc_num'];
  $param['command1']['mop'][0]['credit_card']['cin'] = $data['cin'];
  $param['command1']['mop'][0]['credit_card']['exp_date'] = $data['exp_date'];
  $param['command1']['mop'][0]['credit_card']['first_name'] = (isset($data['name_first']) && $data['name_first']) ? $data['name_first'] : $customer['name_first'];
  $param['command1']['mop'][0]['credit_card']['last_name'] = (isset($data['name_last']) && $data['name_last']) ? $data['name_last'] :  $customer['name_last'];
  $param['command1']['mop'][0]['credit_card']['Avs_address'] = (isset($data['address']) && $data['address']) ? $data['address']  : $customer['address'];
  $param['command1']['mop'][0]['credit_card']['Avs_postal_code'] = (isset($data['zip']) && $data['zip']) ? $data['zip'] : $customer['zip'];

  //params for stored credit card.
  if (isset($data['choose_card']) && $data['choose_card'] != 'new_card') {
    $stored_cc = _tmapi_cc_query();
    $credit_card = $stored_cc['command1']['credit_card'];
     if ($credit_card) {
        $stored_cards = array();
        foreach ($credit_card as $key => $card) {
           if ($data['cc_num'] == $card['data_mask']) {
             $stored_cards = $card;
             break;
           }
        }        
        if ($stored_cards) {
          $param['command1']['mop'][0]['credit_card']['Cc_num_mask'] = $stored_cards['data_mask'] ? $stored_cards['data_mask'] : '';
          $param['command1']['mop'][0]['credit_card']['CC_num_encrypted'] = $stored_cards['encrypt_value'] ? $stored_cards['encrypt_value'] : '';
          $param['command1']['mop'][0]['credit_card']['CC_num_hash'] = $stored_cards['hash_value'] ? $stored_cards['hash_value'] : '';
          $param['command1']['mop'][0]['credit_card']['Certificate_name'] = $stored_cards['cert_name'] ? $stored_cards['cert_name'] : '';
        }
     }
  }  
  
  $json = drupal_json_encode($param);
  //$json = _tmapi_unicode2html($json); //parse the unicode to special char
  $json = str_replace('&#x0027;', "'", html_entity_decode(_tmapi_unicode2html($json))); //parse the unicode to special char
  $response = array();
  $response = tm_api_call($json, 'ticketing_services.aspx');
 // flog_it($response); 

  //check for sth status and vali with response.
  $result = 0;
  if (isset($response['response_ok']) && $response['response_ok']) {
    return $response;
  }
  return 0;
}

/**
 * Updated PCI code for payment request
 */
function _tmapi_payment_request($data) {
  global $user;
  $acct_id = isset($data['tmuser_info']['acct_id']) ? $data['tmuser_info']['acct_id'] : $user->name;
  $invoice_id = isset($data['tmuser_info']['invoice_id']) ? $data['tmuser_info']['invoice_id'] : '';
  $order_id = isset($data['tmuser_info']['order_num']) ? $data['tmuser_info']['order_num'] : '';

  $param = tm_api_get_command_param('payment_request', $acct_id);
  if ($invoice_id) {
    $param['command1']['invoice_id'] = $invoice_id;
    $param['command1']['calling_reason'] = 'P';
    $param['command1']['cart_items']['order_line_item'] = 1;
  }
  elseif ($order_id) {
    $param['command1']['order_num'] = $order_id;
    $order_line_item = isset($data['tmuser_info']['order_line_item']) ? $data['tmuser_info']['order_line_item'] : 1;
    $param['command1']['calling_reason'] = 'B';
    $param['command1']['cart_items'] = $data['tmuser_info']['order_line_item'];
    if (variable_get('enable_service_charge', '') != '') {
      $param['command1']['service_charges'] = $data['tmuser_info']['service_charges'];
    }
  }

  //if "default" payment plan id is selected then
  //payment plan id will not be added in the TM request
  if (isset($data['payment_plan_id']) && $data['payment_plan_id'] != 'default') {
    $param['command1']['payment_plan_id'] = $data['payment_plan_id'];
  }
  // We will add the service_charges in case we receive them in user data and payment plan is full
  /* elseif (isset($data['tmuser_info']['service_charges']) && $data['tmuser_info']['service_charges'] && $data['payment_plan_id'] == 'default') {
    $param['command1']['service_charges'] = $data['tmuser_info']['service_charges'];
  }*/

  $customer_info = get_user_info($acct_id);
  $customer = $customer_info->customer;
  $customer['name_first'] = (isset($customer['name_first']) && $customer['name_first']) ? $customer['name_first'] : '';
  $customer['name_last'] = (isset($customer['name_last']) && $customer['name_last']) ? $customer['name_last'] : '';
  $customer['zip'] = (isset($customer['zip']) && $customer['zip']) ? $customer['zip'] : '';
  $customer['address'] = (isset($customer['street_addr_1']) && $customer['street_addr_1']) ? $customer['street_addr_1'] : '';
  //$param['command1']['calling_reason'] = 'P';
  $param['command1']['mop'][0]['auth_mode'] = 'O';
  $param['command1']['mop'][0]['amount'] = $data['amount'];
  $param['command1']['mop'][0]['pmt_type'] = $data['ctype'];


  if (isset($data['cc_num']) && is_numeric($data['cc_num'])) {
    $encryption = encrypt_cc($data['cc_num']); //Encrypt credit card number
    $masked_cc = mask_credit_card($data['cc_num']);
  }
//  $param['command1']['mop'][0]['credit_card']['cc_num'] = $data['cc_num'];
  $param['command1']['mop'][0]['credit_card']['cc_num_mask'] = $masked_cc['cc_num_mask'];
  $param['command1']['mop'][0]['credit_card']['cc_num_encrypted'] = $encryption['ccNumberEncrypted64'];
  $param['command1']['mop'][0]['credit_card']['cc_num_hash'] = $encryption['cc_num_hash'];
  $param['command1']['mop'][0]['credit_card']['certificate_name'] = basename($encryption['certificate_name']);


  $param['command1']['mop'][0]['credit_card']['cin'] = $data['cin'];
  $param['command1']['mop'][0]['credit_card']['exp_date'] = $data['exp_date'];
  $param['command1']['mop'][0]['credit_card']['first_name'] = (isset($data['name_first']) && $data['name_first']) ? $data['name_first'] : $customer['name_first'];
  $param['command1']['mop'][0]['credit_card']['last_name'] = (isset($data['name_last']) && $data['name_last']) ? $data['name_last'] :  $customer['name_last'];
  $param['command1']['mop'][0]['credit_card']['Avs_address'] = (isset($data['address']) && $data['address']) ? $data['address']  : $customer['address'];
  $param['command1']['mop'][0]['credit_card']['Avs_postal_code'] = (isset($data['zip']) && $data['zip']) ? $data['zip'] : $customer['zip'];

  //params for stored credit card.
  if (isset($data['choose_card']) && $data['choose_card'] != 'new_card') {
    $stored_cc = _tmapi_cc_query();
    $credit_card = $stored_cc['command1']['credit_card'];
     if ($credit_card) {
        $stored_cards = array();
        foreach ($credit_card as $key => $card) {
           if ($data['choose_card'].$data['ctype'] == $key . '_' . $card['data_mask'] . $card['cc_type']) {
             $stored_cards = $card;
             break;
           }
        }
        if ($stored_cards) {
          $param['command1']['mop'][0]['credit_card']['cc_num_mask'] = $stored_cards['data_mask'] ? $stored_cards['data_mask'] : '';
          $param['command1']['mop'][0]['credit_card']['cc_num_encrypted'] = $stored_cards['encrypt_value'] ? $stored_cards['encrypt_value'] : '';
          $param['command1']['mop'][0]['credit_card']['cc_num_hash'] = $stored_cards['hash_value'] ? $stored_cards['hash_value'] : '';
          $param['command1']['mop'][0]['credit_card']['certificate_name'] = $stored_cards['cert_name'] ? $stored_cards['cert_name'] : '';
        }
        
     }
  }
  $json = drupal_json_encode($param);
  //$json = _tmapi_unicode2html($json); //parse the unicode to special char
  $json = str_replace('&#x0027;', "'", html_entity_decode(_tmapi_unicode2html($json))); //parse the unicode to special char
  $response = array();
  $response = tm_api_call($json, 'ticketing_services.aspx');
  //check for sth status and vali with response.
  $result = 0;
  if (isset($response['response_ok']) && $response['response_ok']) {
    return $response;
  }
  return 0;
}

/**
 * Get Encrypted Credit card number.
 */
function encrypt_cc($credit_card_number) {
  module_load_include('php', 'tm_api', 'pts');
  $certificate_name = '/IOM-STP/Certificate/paysys.0.us2048.6';
  //$certificate_name = '/stp/core/encrypt_cc/cert/paysys.0.us2048.6';
  $encrypt = pts::encrypt_credit_card_number($credit_card_number, $certificate_name);
  return $encrypt;
}

/**
 * Function to mask credit card number.
 */
function mask_credit_card($credit_card_number, $masking_character = 'X') {
  $arr = array();
  $arr['cc_bin_num'] = substr($credit_card_number, 0, 6);
  $arr['cc_num_mask'] = $masking_character . substr($credit_card_number, -4);
  return $arr;
}

/**
 * TM API call to fetch seats on account user
 * @param $acct_id user account ID
 * @param $flag 1 to return TM data
 */
function _tmapi_get_seats_on_account($acct_id = NULL, $flag=0) {
  if ((int) $acct_id) {
    $param = tm_api_get_command_param('get_seats_on_account', $acct_id);
    $json = drupal_json_encode($param);
    $response = array();
    //$response = tm_api_call($json, 'atsext/ext.asp');
    $response = tm_api_call($json, 'ticketing_services.aspx');
    //check for acct_rep status and vali with response.
    if (isset($response['response_ok']) && $response['response_ok']) {
      $data = drupal_json_decode($response['response']);
      if (!empty($data) && count($data) && !$data['header']['error_detail'] && !(isset($data[0]['status']) && $data[0]['status']!= 'error')) {
        return $data;
      }
    }
  }
  return array();
}

/**
 * TM API call to fetch seats on account user
 * @param $acct_id user account ID
 * @param $flag 1 to return TM data
 */
function _tmapi_get_seats_on_account_reserved($acct_id = NULL, $flag=0) {
  if ((int) $acct_id) {
    $param = tm_api_get_command_param('get_seats_on_account_reserved', $acct_id);
    $json = drupal_json_encode($param);
    $response = array();
    $response = tm_api_call($json, 'ticketing_services.aspx');
    if (isset($response['response_ok']) && $response['response_ok']) {
      $data = drupal_json_decode($response['response']);
      if (!empty($data) && count($data) && !$data['header']['error_detail']) {
        return $data;
      }
    }
  }
  return array();
}

function tmapi_get_payment_plans() {
  $param = tm_api_get_command_param('payment_plan_details');
  $json = drupal_json_encode($param);
  $response = $plan_schedules = $sort_arr = $plans_sch = array();
  $response = tm_api_call($json, 'ticketing_services.aspx');
  if (isset($response['response_ok']) && $response['response_ok']) {
    $data = drupal_json_decode($response['response']);
    if (isset($data['command1']['result']) && $data['command1']['result'] == 0) {
      $plans = (isset($data['command1']['plans']) && count($data['command1']['plans']) > 0) ? $data['command1']['plans'] : FALSE;      
        //cache payment plans
        $cid = 'tmapi_get_payment_plans';
        $cache_time = time() + 60*60*3; //flush cache every 3 hours
        //$cache_time = time() + 120;
        $cache = cache_get($cid);
        if (!empty($cache->data)) {
          if (time() >= $cache->expire) {
            cache_clear_all($cid, 'cache');
            cache_set($cid, $plans, 'cache', $cache_time);            
          }
           else {
             $plans = $cache->data;
          }
        }
        else {
          cache_set($cid, $plans, 'cache', $cache_time); 
        }
        
      return $plans;
    }
    else {
      return FALSE;
    }
  }
  return 1;
}
/*
* payment plans for invoice
*/
function tmapi_get_payment_plans_for_invoice($acct_id = NULL, $invoice_id = NULL) {
  if (isset($invoice_id) && !empty($invoice_id)) {
    $param = tm_api_get_command_param('get_payment_plans_for_invoice', $acct_id);
    $param['command1']['invoice_id'] = $invoice_id;
    $schedule_json = drupal_json_encode($param);
    $response = array();
    $response = tm_api_call($schedule_json, 'ticketing_services.aspx');
    if (isset($response['response_ok']) && $response['response_ok']) {
      $data = drupal_json_decode($response['response']);
      if (isset($data[0]['result_code']) && $data[0]['result_code'] == 0) {
        $payments = (isset($data[0]['payment_plan_ids']) && $data[0]['payment_plan_ids']) ? $data[0]['payment_plan_ids'] : FALSE;
        return $payments;
      }
      else {
        //Error in API response
        return 0;
      }
    }
  }
  return 0;
}

/*
* Function to fetch Payment Schedules
* @param integer $acct_id Account ID of the customer
* @param integer $invoice_ID invoice id of the customer
* @param integer $plan_id selected payment_plan_id
* @return array response of all the available Schedules
*/
function tmapi_get_payment_schedules($acct_id=NULL, $invoice_id=NULL, $plan_id=NULL) {
  if(isset($plan_id) && !empty($plan_id)) {
    $param = tm_api_get_command_param('payment_schedule', $acct_id);
    $param['command1']['invoice_id'] = $invoice_id;
    $param['command1']['payment_plan_id'] = $plan_id;
    $schedule_json = drupal_json_encode($param);
    $response = array();
    $response = tm_api_call($schedule_json, 'ticketing_services.aspx');
    if(isset($response['response_ok']) && $response['response_ok']) {
      $data_schedule = drupal_json_decode($response['response']);
      if (isset($data_schedule['command1']['result']) && $data_schedule['command1']['result']==0) {
        $payments = (isset($data_schedule['command1']['payments']) && count($data_schedule['command1']['payments'])>0)? $data_schedule['command1']['payments']:false;        
        return $payments;
      }
      else{
        //Error in API response
        return 0;
      }
    }
  }
  return 0;
}

/**
 * TM invoice_list API to fetch invoices associated to an account
 * @param $acct_id integer account ID of the user
 * @return array invoice list 
 */
function _tmapi_get_invoice_all_list($acct_id = NULL) {
  if ((int) $acct_id) {
    $param = tm_api_get_command_param('invoice_list', $acct_id);
    $json = drupal_json_encode($param);
    $response = array();
    $response = tm_api_call($json, 'ticketing_services.aspx');
    //check for invoice id vali with response.
    $result = 0;
    $invoices = array();
    if (isset($response['response_ok']) && $response['response_ok']) {
      $data = drupal_json_decode($response['response']);
      if (!empty($data['command1']['invoices']) && isset($data['command1']['result']) && $data['command1']['result'] == 0) {
        foreach ($data['command1']['invoices'] as $key => $invoice) {
          $invoices[$invoice['invoice_ids']] = $invoice;
        }
        return $invoices;
      }
    }
  }
  return 0;
}

/**
 * Function to get the Invoice Detail of a TM account
 * @param $acct_id integer account ID of the customer
 * @param $invoice_id integer Invoice ID of the customer
 * @return array invoice detail response
 */
function tmapi_get_sth_invoice_using_InvoiceID($acct_id = NULL, $invoice_id = NULL) {
  // Check if invoice id exists then we fetch the invoice of TM account
  if ($invoice_id && ((int) $acct_id)) {
    $param = tm_api_get_command_param('invoice_details', $acct_id);
    $param['command1']['invoice_id'] = $invoice_id;
    $json = drupal_json_encode($param);
    $response = array();
    $response = tm_api_call($json, 'ticketing_services.aspx');//print_r($response); exit;
    //check for sth status and vali with response.
    $rows = array();
    if (isset($response['response_ok']) && $response['response_ok']) {
      $data = drupal_json_decode($response['response']); //print_r($data); exit;
      if (isset($data['command1']['items']) && isset($data['command1']['result']) && $data['command1']['result'] == 0) {
        foreach ($data['command1']['items'] as $key => $row) {
          $row['acct_id'] = $acct_id;
          $row['invoice_id'] = $invoice_id;
          $row['due_date'] = $data['command1']['due_date'];
          $row['invoice_date'] = isset($data['command1']['invoice_date']) ? $data['command1']['invoice_date'] : '';
          $rows[] = $row;
        }
        return $rows;
      }
    }
  }
  return 0;
}
/**
 * When json_encode() special char it convert to unicode
 * function to convert unicode to special char. 
 */
function _tmapi_unicode2html($string) {
    $output = preg_replace('/\\\\u([0-9a-z]{4})/', '&#x$1;', $string);
    $output = trim($output,'"');
    return $output;
}

/*
 * API call to get Order id
 */
function _tmapi_get_order_id($acct_id = NULL) {  
  if ((int) $acct_id) {
    $param = tm_api_get_command_param('seats_hold', $acct_id);
    $param['command1']['event_name'] = "IOGAEVEN";
    $param['command1']['price_code'] = "";
    $param['command1']['ticket_type'] = array(array("ticket_type_code" => "","num_seats" => 3));
    $param['command1']['count'] = 1;
    $param['command1']['hold_classes'] = array();
    unset($param['command1']['acct_id']);
    $json = drupal_json_encode($param);
    $response = array();
    $response = tm_api_call($json, 'ticketing_services.aspx');
    if (isset($response['response_ok']) && $response['response_ok']) {
      $data = drupal_json_decode($response['response']);
//      return $data['command1']['order_num_out'];
      return $data['command1'];
    }
  }
  return 0;
}

function _tmapi_payment_request_to_orderid($data) {
  global $user;
  $acct_id = isset($data['tmuser_info']['acct_id']) ? $data['tmuser_info']['acct_id'] : $user->name;
//  $invoice_id = isset($data['tmuser_info']['invoice_id']) ? $data['tmuser_info']['invoice_id'] : '';
  $order_details = _tmapi_get_order_id($acct_id);
//  print "<pre>"; print_r($orderid); die;
  $orderid = 12345;
  $param = tm_api_get_command_param('payment_request', $acct_id);
  $param['command1']['order_num'] = $order_details['order_num_out'];
//      print "<pre>"; print_r($data); die;

  //if "default" payment plan id is selected then
  //payment plan id will not be added in the TM request
  if (isset($data['payment_plan_id']) && $data['payment_plan_id'] != 'default' && $data['payment_plan_id'] != '65') {
    $param['command1']['payment_plan_id'] = $data['payment_plan_id'];
  }
  elseif (isset($data['payment_plan_id']) && $data['payment_plan_id'] == '65' && $data['amount'] == 0) {
    $param['command1']['payment_plan_id'] = $data['payment_plan_id'];
  }
  $customer_info = get_user_info($acct_id);
  $customer = $customer_info->customer;
  $customer['name_first'] = (isset($customer['name_first']) && $customer['name_first']) ? $customer['name_first'] : '';
  $customer['name_last'] = (isset($customer['name_last']) && $customer['name_last']) ? $customer['name_last'] : '';
  $customer['zip'] = (isset($customer['zip']) && $customer['zip']) ? $customer['zip'] : '';
  $customer['address'] = (isset($customer['street_addr_1']) && $customer['street_addr_1']) ? $customer['street_addr_1'] : '';
//  $param['command1']['calling_reason'] = 'P';
  $param['command1']['mop'][0]['auth_mode'] = 'O';
  $param['command1']['mop'][0]['amount'] = $data['amount'];
  $param['command1']['mop'][0]['pmt_type'] = $data['ctype'];
  $param['command1']['mop'][0]['credit_card']['cc_num'] = $data['cc_num'];
  $param['command1']['mop'][0]['credit_card']['cin'] = $data['cin'];
  $param['command1']['mop'][0]['credit_card']['exp_date'] = $data['exp_date'];
  $param['command1']['mop'][0]['credit_card']['first_name'] = (isset($data['name_first']) && $data['name_first']) ? $data['name_first'] : $customer['name_first'];
  $param['command1']['mop'][0]['credit_card']['last_name'] = (isset($data['name_last']) && $data['name_last']) ? $data['name_last'] :  $customer['name_last'];
  $param['command1']['mop'][0]['credit_card']['Avs_address'] = (isset($data['address']) && $data['address']) ? $data['address']  : $customer['address'];
  $param['command1']['mop'][0]['credit_card']['Avs_postal_code'] = (isset($data['zip']) && $data['zip']) ? $data['zip'] : $customer['zip'];
  
  /*************************************************************************************************************************/
  //Order Line Item should come from _tmapi_get_order_id API
  /*************************************************************************************************************************/
  
  $param['command1']['cart_items'] = array(array("order_line_item" => "1", "dm_code" => ""));
 
//  $param['command1']['cart_items']['order_line_item'] = 1;
//  $param['command1']['cart_items']['dm_code'] = "";
  $json = drupal_json_encode($param);  
$json = _tmapi_unicode2html($json); //parse the unicode to special char
// print "<pre>"; print_r($json); die;
  $response = array(); 
  $response = tm_api_call($json, 'ticketing_services.aspx');
  //check for sth status and vali with response.
  $result = 0;
  if (isset($response['response_ok']) && $response['response_ok']) {
    return $response;
  }
  return 0;
}

/************************** Customer update call to update new fields for questions*******/

function _tmapi_customer_update($acct_id = NULL, $customer = NULL) {
  $pass = isset($_SESSION['tmupa']) ? base64_decode($_SESSION['tmupa']) : '';
  $param = tm_api_get_command_param('customer_update', $acct_id);
  $param['command1']['pin'] = $pass;
  if (!empty($customer)) {
    $param['command1'] = array_merge($param['command1'], $customer);
  }
  //$param['command1']['customer']['acct_id'] = $acct_id;
  $json = drupal_json_encode($param);
  $json = html_entity_decode(_tmapi_unicode2html($json)); //parse the unicode to special char
  $api_sufix="";
  $response = array();
  $response = tm_api_call($json, 'ticketing_services.aspx');
  //check for sth status and validate with response.
  $result = 0;
  if (isset($response['response_ok']) && $response['response_ok']) {
    $data = drupal_json_decode($response['response']);
    if (!isset($data['command1']['error_detail']) && isset($data['command1']['result']) && $data['command1']['result'] == 0) {
      $result = 1;
    }
  }
  return $result;
}

/************************** Customer add call to update new fields for questions*******/

function _tmapi_customer_add($customer = NULL) {
  $param = tm_api_get_command_param('customer_add');
  $param['command1']['pin'] = $customer['customer']['pin'];
  if (!empty($customer)) {
    $param['command1'] = array_merge($param['command1'], $customer);
  }
  //$param['command1']['customer']['acct_id'] = $acct_id;
  $json = drupal_json_encode($param);
  $json = html_entity_decode(_tmapi_unicode2html($json)); //parse the unicode to special char
  $api_sufix="";
  $response = array();
  $response = tm_api_call($json, 'ticketing_services.aspx');
  //check for sth status and validate with response.
  $result = 0;
  if (isset($response['response_ok']) && $response['response_ok']) {
    $data = drupal_json_decode($response['response']);
    return $data;
    if (!isset($data['command1']['error_detail']) && isset($data['command1']['result']) && $data['command1']['result'] == 0) {
      $result = 1;
    }
  }
  return $result;
}
/************************** Buy Flow TM Apis*******/

function _tmapi_seats_hold($acct_id = NULL, $event_name = NULL, $price_code = NULL, $ticket_type_code = NULL, $num_seats = NULL, $order_num = 0) {
  $param = tm_api_get_command_param('seats_hold', $acct_id);
  $param['command1']['event_name'] = $event_name;
  $param['command1']['price_code'] = $price_code;
  $param['command1']['ticket_type'] = array(array('ticket_type_code' => $ticket_type_code, 'num_seats' => $num_seats));
  if ($order_num) {
    $param['command1']['order_num_in'] = $order_num;
  }
  //$param['command1']['customer']['acct_id'] = $acct_id;
  $json = drupal_json_encode($param);
  $response = array();
  $response = tm_api_call($json, 'ticketing_services.aspx');
  //check for sth status and validate with response.
  $result = 0;
  if (isset($response['response_ok']) && $response['response_ok']) {
    $data = drupal_json_decode($response['response']);
    return $data['command1'];
  }
  return 0;
}

/**
 * TM API for checkout
 */
function _tmapi_check_out($acct_id = NULL, $order_num = NULL) {
  $param = tm_api_get_command_param('check_out', $acct_id);
  $param['command1']['order_num'] = $order_num;
  //$param['command1']['customer']['acct_id'] = $acct_id;
  $json = drupal_json_encode($param);
  $response = array();
  $response = tm_api_call($json, 'ticketing_services.aspx');
  //check for sth status and validate with response.
  $result = 0;
  if (isset($response['response_ok']) && $response['response_ok']) {
    $data = drupal_json_decode($response['response']);
    return $data['command1'];
  }
  return $result;
}

/**
 * TM API for event_price_info
 */
function _tmapi_event_price_info($event_name = NULL) {
  $param = tm_api_get_command_param('event_price_info');
  $param['command1']['event_name'] = $event_name;
  //$param['command1']['customer']['acct_id'] = $acct_id;
  $json = drupal_json_encode($param);
  $response = array();
  $response = tm_api_call($json, 'ticketing_services.aspx');
  //check for sth status and validate with response.
  $result = 0;
  if (isset($response['response_ok']) && $response['response_ok']) {
    $data = drupal_json_decode($response['response']);
    return $data['command1'];
  }
  return $result;
}

function tmapi_get_buy_payment_schedules($acct_id=NULL, $order_id=NULL, $plan_id=NULL, $total_svc_chg_amount = 0, $credit = NULL) {
  if(isset($plan_id) && !empty($plan_id)) {
    $param = tm_api_get_command_param('payment_schedule', $acct_id);
    $param['command1']['order_num'] = $order_id;
    $param['command1']['payment_plan_id'] = $plan_id;
    $param['command1']['total_svc_chg_amount'] = $total_svc_chg_amount;
    if(isset($credit) && $credit) {
     $param['command1']['account_credit'] = $credit;
    }
    $schedule_json = drupal_json_encode($param);
    $response = array();
    $response = tm_api_call($schedule_json, 'ticketing_services.aspx');
    if(isset($response['response_ok']) && $response['response_ok']) {
      $data_schedule = drupal_json_decode($response['response']);
      if (isset($data_schedule['command1']['result']) && $data_schedule['command1']['result']==0) {
        $payments = (isset($data_schedule['command1']['payments']) && count($data_schedule['command1']['payments'])>0)? $data_schedule['command1']['payments']:false;        
        return $payments;
      }
      else{
        //Error in API response
        return 0;
      }
    }
  }
  return 0;
}

/**
 * API Call for upgrade_list(STP-STR)
 */
function _tmapi_get_upgrade_list($acct_id = NULL, $amClientId = NULL) {
  if ((int) $acct_id) {
    $param = tm_api_get_command_param('upgrade_list', $acct_id);
    $customer_info = get_user_info($acct_id);
    $customer = $customer_info->customer;
    $param['command1']['am_client_id'] = $amClientId;
    $param['command1']['cust_name_id'] = $customer['cust_name_id'];
    unset($param['command1']['site_name']);
    $json = drupal_json_encode($param);
    $response = array();
    $response = tm_api_call($json, 'ticketing_services.aspx');
    //check for invoice id validate with response.
    $result = 0;
    if (isset($response['response_ok']) && $response['response_ok']) {
      $data = drupal_json_decode($response['response']);
     if (isset($response['response_ok']) && $response['response_ok']) {
      $data = drupal_json_decode($response['response']);
      if (!empty($data) && count($data) && !$data['header']['error_detail'] && !(isset($data[0]['status']) && $data[0]['status']!= 'error')) {
          // We will update the account rep cache
        cache_clear_all('upgrade_list_' . $acct_id, 'cache_custom');
        // Account rep information is cached
        //cache_set('upgrade_list_' . $acct_id, $data, 'cache_custom');
        return $data;
       }
      }
    }
  }
  return 0;
}

/**
 * API Call for generate_thumbprint(STP-STR)
 */

function _tmapi_get_thumbprint($seat_events = NULL, $call_reason, $target_event_name) {
    global $user;
    $acct_id = $user->name;
    $param = tm_api_get_command_param('Generate_Thumbprint', $acct_id);
    unset($param['command1']['site_name']);
    $amClientId = variable_get('amClientId', '');
    $param['command1']['call_reason'] = $call_reason;
    $param['command1']['am_client_id'] = $amClientId;
    if(isset($seat_events) && count($seat_events)>0){
        $param['command1']['event_name'] = $target_event_name;
        $posting_session_id = "";
        foreach ($seat_events as $seat){
           $param['command1']['section_name'] = $seat['section_name'];
           $param['command1']['seat_num'] = $seat['seat_num'];
           $param['command1']['row_name'] = $seat['row_name'];
           $param['command1']['num_seats'] = $seat['num_seats'];
           $param['command1']['seat_increment'] = $seat['seat_increment'];
           if(isset($posting_session_id) && !empty($posting_session_id)){
               $param['command1']['posting_session_id'] = $posting_session_id;
           }
           $json = drupal_json_encode($param);
		   
           $response = array();
           $response = tm_api_call($json, 'ticketing_services.aspx');
		   if(isset($response['response_ok']) && $response['response_ok']){
			   $response_temp = drupal_json_decode($response['response']);
		       if(
				isset($response_temp['command1']['posting_session_id']) && 
				!empty($response_temp['command1']['posting_session_id'])
			   )
			   {
				   $posting_session_id = $response_temp['command1']['posting_session_id'];
			   }
		   }
        }
    }
    else {
           $param['command1']['target_event_name'] = $target_event_name;
           $json = drupal_json_encode($param);
           $response = array();
           $response = tm_api_call($json, 'ticketing_services.aspx');
           //$param['command1']['posting_session_id'] = isset ($response['command1']['posting_session_id']) ? $response['command1']['posting_session_id'] : '';
    }
    //check for invoice id validate with response.
    $result = 0;
    if (isset($response['response_ok']) && $response['response_ok']) {
     $data = drupal_json_decode($response['response']);
	   if (
		   !empty($data) && count($data) && 
		   !$data['header']['error_detail'] && 
		   !(isset($data[0]['status']) && 
		   $data[0]['status']!= 'error')
	   ) { 
			return $data;        
       }
     }
 return 0;
}


/*
get_event_details _API
*/

function tmapi_get_event_details($acct_id,$event_filter,$event_filter_type)
{
    $param = tm_api_get_command_param('event_details', $acct_id);
    $param['command1']['event_filter'] = $event_filter;
    $param['command1']['event_filter_type'] = $event_filter_type;
    unset($param['command1']['site_name']);
    unset($param['command1']['acct_id']);
    $param['command1']['ref'] = "1AB2C3";
    $json = drupal_json_encode($param);
    $response = array();
    $response = tm_api_call($json, 'ticketing_services.aspx');
    $result = 0;
    
    if (isset($response['response_ok']) && $response['response_ok'])
    {
        $data = drupal_json_decode($response['response']);
        return $data['command1'];
    }
    return $result;
}



/**
 * System_options tm api
 * We will cache the system_options api response for 24 hours
 * 
**/

function tmapi_get_system_options() {
  $object = cache_get('system_options', 'cache');
  if (!$object->data) {
    $param = tm_api_get_command_param('system_options');
    $json = drupal_json_encode($param);
    $response =  array();
    $response = tm_api_call($json, 'ticketing_services.aspx');
    if (isset($response['response_ok']) && $response['response_ok']) {
      $data = drupal_json_decode($response['response']);
      if (!empty($data) && count($data) && !$data['header']['error_detail'] && (isset($data['command1']['result']) && $data['command1']['result']==0)) {
        cache_set('system_options', $data['command1'], 'cache', time() + 24*60*60);
        return $data['command1'];
      }
    }
  }
  else {
    return $object->data;
  }
  return 0;
}


/**
 *
 * get_event_details _API
 * Cache: Yes, We will cache the system_options api response for 24 hours
*/

function tmapi_get_event_details_cache() {
  $object = cache_get('event_details', 'cache');
  if (!$object->data) {
    $param = tm_api_get_command_param('event_details');
    $json = drupal_json_encode($param);
    $response =  array();
    $response = tm_api_call($json, 'ticketing_services.aspx');
    if (isset($response['response_ok']) && $response['response_ok']) {
      $data = drupal_json_decode($response['response']);
      if (!empty($data) && count($data) && !$data['header']['error_detail'] && (isset($data['command1']['result']) && $data['command1']['result']==0)) {
        if (count($data['command1']['events'])) {
          foreach ($data['command1']['events'] as $key => $event) {
            $output[$event['event_name']] = $event;
          }
        }
        cache_set('event_details', $output, 'cache', time() + 24*60*60);
        return $output;
      }
    }
  }
  else {
    $output = $object->data;
  }
  
  return $output;
}
 
 function _tmapi_cc_query() {
  global $user;
  $acct_id = $user->name;
  $param = tm_api_get_command_param('cc_query', $acct_id);
  $json = drupal_json_encode($param); 
  $response =  array();
  $response = tm_api_call($json, 'ticketing_services.aspx');
  if (isset($response['response_ok']) && $response['response_ok']) {
    $data = drupal_json_decode($response['response']);
    //print '<pre>'; print_r($data); exit;
    if (!empty($data) && count($data)) {
      return $data;
    }
  }
  return 0;

 $cid = 'get_cc_query_'.$acct_id;
 $object = cache_get($cid, 'cache'); 
  if (!$object->data['command1']['credit_card']) {
     $param = tm_api_get_command_param('cc_query', $acct_id);
      $json = drupal_json_encode($param);
      $response =  array();
      $response = tm_api_call($json, 'ticketing_services.aspx');
      if (isset($response['response_ok']) && $response['response_ok']) {
        $data = drupal_json_decode($response['response']);
        if (!empty($data) && count($data)) {
          cache_clear_all($cid, 'cache');
          cache_set($cid, $data, 'cache', time() + 60*10);
          return $data;
        }
      }
  }
  else {
    return $object->data;
  }
  return 0;
}

/**
 * Function looks clone of _tmapi_cc_update_call().
 * @param $acct_id
 *   string - account id of the user
 *   $udata
     object -- user credit card details.
 * @return $response
 *   array - response array
 *
 **/
function _tmapi_cc_update_cc_query($acct_id, $udata = NULL) {
  $acct_id = isset($udata['tmuser_info']['acct_id']) ? $udata['tmuser_info']['acct_id'] : $acct_id;
  $param = tm_api_get_command_param('cc_update', $acct_id);
  if (!variable_get('tm_api_test', 0)) {
    $param['header']['archtics_version'] = 'V999';
  }
  $customer_info = get_user_info($acct_id);
  $customer = $customer_info->customer;
  $customer['name_first'] = (isset($customer['name_first']) && $customer['name_first']) ? $customer['name_first'] : '';
  $customer['name_last'] = (isset($customer['name_last']) && $customer['name_last']) ? $customer['name_last'] : '';
  $customer['zip'] = (isset($customer['zip']) && $customer['zip']) ? $customer['zip'] : '';
  $customer['address'] = (isset($customer['street_addr_1']) && $customer['street_addr_1']) ? $customer['street_addr_1'] : '';
  $param['command1']['cc_num'] = $udata['cc_num_full'];
  $param['command1']['cc_type'] = $udata['ctype'];
  $param['command1']['cc_exp'] = $udata['exp_date'];
  $param['command1']['action'] = 'X'; //action will be applied both add/update
  $param['command1']['generate_pid'] = 'Y';
  $param['command1']['display_on_inet'] = 'Y';

  ///$param['command1']['cust_name_id'] = $acct_id; //Please don't use this id. it will not save your data in TM.
  $param['command1']['avs_first_name'] = (isset($udata['name_first']) && $udata['name_first']) ? $udata['name_first'] : $customer['name_first'];
  $param['command1']['avs_last_name'] = (isset($udata['name_last']) && $udata['name_last']) ? $udata['name_last'] :  $customer['name_last'];
  $param['command1']['avs_address'] = (isset($udata['address']) && $udata['address']) ? $udata['address']  : $customer['address'];
  $param['command1']['avs_postal_code'] = (isset($udata['zip']) && $udata['zip']) ? $udata['zip'] : $customer['zip'];

 //params for stored credit card.
 //UPdate for existing credit card.
  if (isset($udata['choose_card']) && $udata['choose_card'] != 'new_card') {
    $stored_cc = _tmapi_cc_query();
    $credit_card = $stored_cc['command1']['credit_card'];    
     if ($credit_card) {
        $stored_cards = array();
        foreach ($credit_card as $key => $card) {
           if ($udata['cc_num'] == $card['data_mask']) {
             $stored_cards = $card;
             break;
           }
        }        
        if ($stored_cards) {
          //$param['command1']['action'] = 'U';
          $param['command1']['cc_seq_num'] = $stored_cards['cc_seq_num'];
            unset($param['command1']['generate_pid']);
          $param['command1']['Cc_num_mask'] = $stored_cards['data_mask'];
          $param['command1']['CC_num_encrypted'] = $stored_cards['encrypt_value'];
          $param['command1']['CC_num_hash'] = $stored_cards['hash_value'];
          $param['command1']['Certificate_name'] = $stored_cards['cert_name'];
        }
        else {
         //flog_it('Credit card Details not found');
         //return 0;
        }
     }
  }
        
  $json = drupal_json_encode($param);
  $response = tm_api_call($json, 'ticketing_services.aspx');
  
  $result = 0;
  if (isset($response['response_ok']) && $response['response_ok']) {
    $data = drupal_json_decode($response['response']);
    $udata['command'] = 'cc_update';
    if (!isset($data['command1']['error_detail']) && isset($data['command1']['cc_seq_num']) && $data['command1']['cc_seq_num']) {
      $result = $data['command1']['cc_seq_num'];
      $udata['result'] = '1';
    }    
  }
  return $result;
}