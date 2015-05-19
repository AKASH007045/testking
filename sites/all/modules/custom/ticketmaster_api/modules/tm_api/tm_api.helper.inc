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
  //$adb_url = variable_get('tm_login_url_test', 'https://ws-test.ticketmaster.net/archtics/ats/ats.aspx?dsn=bucs602');
  $adb_url = 'https://ws-test.ticketmaster.net/archtics/ats/'. $web_url .'?dsn=' . $dsn;
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
 * Function to get the STH STATUS of a TM account
 */
function _tmapi_get_sth_status($acct_id = NULL) {
  $param = tm_api_get_command_param('get_sth_status', $acct_id);
  $param['command1']['item_set'] = '#IOM-STH-SET';
  $json = drupal_json_encode($param);
  $response = array();
  $response = tm_api_call($json, 'ticketing_services.aspx');
  //check for sth status and validate with response.
  $result = 1;
  $data = drupal_json_decode($response['response']);
  if (isset($response['response_ok']) && $response['response_ok'] && isset($data[0]['sth_status']) && $data[0]['sth_status'] == 0) {
    $result = 0;
  }
  return $result;
}

/**
 * Function to get the STH STATUS of a TM account
 */
function _tmapi_get_sth_status_rsvp($acct_id = NULL) {
  $param = tm_api_get_command_param('get_sth_status', $acct_id);
  $json = drupal_json_encode($param);
  $param['command1']['uid'] = 'ATS01';
  $response = array();
  $response = tm_api_call($json, 'ticketing_services.aspx');
  //check for sth status and validate with response.
  $result = 0;
  if (isset($response['response_ok']) && $response['response_ok']) {
    $data = drupal_json_decode($response['response']);
    if (!isset($data['command1']['error_detail']) && isset($data[0]['sth_status']) && $data[0]['sth_status'] == 1) {
      $result = $data[0];
    }
  }
  return $result;
}
/**
 * TM API call to fetch account rep of user
 */
function _tmapi_get_acct_rep($acct_id = NULL) {
  $param = tm_api_get_command_param('get_acct_rep', $acct_id);
  unset($param['command1']['ref']);
  unset($param['command1']['site_name']);
  $json = drupal_json_encode($param);
  $response = array();
  $response = tm_api_call($json, 'ticketing_services.aspx');
  //check for acct_rep status and validate with response.
  $result = 0;
  if (isset($response['response_ok']) && $response['response_ok']) {
    $data = drupal_json_decode($response['response']);
    if (!empty($data[0]['rep_acct_id']) && isset($data[0]['rep_acct_id'])) {
      $result = 1;
      // we will update the account rep information of TM user
      tm_update_acct_rep($data[0]['rep_acct_id'], $acct_id);
    }
  }
  return $result;
}

/**
 * TM invoice_list API to fetch invoice id 
 */
function _tmapi_cc_update_call($acct_id = NULL, $udata = NULL) {
  $cust_name_id = $_SESSION['cust_name_id'];
  $param = tm_api_get_command_param('cc_update', $acct_id);
  if (!variable_get('tm_api_test', 0)) {
    $param['header']['archtics_version'] = 'V999';
  }
  $param['command1']['cc_num'] = $udata['cc_num'];
  $param['command1']['cc_type'] = $udata['ctype'];
  $param['command1']['cc_exp'] = $udata['exp_date'];
  $param['command1']['action'] = 'X';
  $param['command1']['cust_name_id'] = $cust_name_id;
  $json = drupal_json_encode($param);
  $response = tm_api_call($json, 'ticketing_services.aspx');
  //check for cc_update call status and update the transaction table with response.
  $result = 0;
  if (isset($response['response_ok']) && $response['response_ok']) {
    $data = drupal_json_decode($response['response']);
    $udata['command'] = 'cc_update';
    if (!isset($data['command1']['error_detail']) && isset($data['command1']['cc_seq_num']) && $data['command1']['cc_seq_num']) {
      $result = $data['command1']['cc_seq_num'];
      $udata['result'] = '1';
    }
    // We will update the cc_update transaction logs
    $udata['acct_id'] = $acct_id;
    _pawp_update_tm_transcation_table($udata, $response['response']);
  }
  return $result;
}

/**
 * TM invoice_list API to fetch invoice id
 * @acct_id integer account ID of the TM user
 * @udata user posted data
 */
function _tmapi_customer_update_call($acct_id = NULL, $udata = NULL, $cust_cc_seq_num = '', $flag = NULL) {
  $pass = isset($_SESSION['tmupa']) ? base64_decode($_SESSION['tmupa']) : '';
  // If flag is set to update customer on "No Thanks" or for successful cc_update customer sequential number exists
  if ($flag == 'NO' || $cust_cc_seq_num) {
    $param = tm_api_get_command_param('customer_update', $acct_id);
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
    //check for sth status and validate with response.
    $result = 0;
    if (isset($response['response_ok']) && $response['response_ok']) {
      $data = drupal_json_decode($response['response']);
      $udata['command'] = 'customer_update';
      if (!isset($data['command1']['error_detail']) && isset($data['command1']['result']) && $data['command1']['result'] == 0) {
        $result = 1;
        $udata['result'] = (isset($flag) && ($flag == 'NO')) ? -1 : 1;
      }
      _pawp_update_tm_transcation_table($udata, $response['response']);
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
    //check for sth status and validate with response.
    $result = 0;
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
        tmapi_data_tm_invoice_details($acct_id, $rows);
        return 1;
      }
    }
    return $result;
  }
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
  //check for invoice id validate with response.
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

function _tmapi_payment_request($data) {
  global $user;
  $acct_id = isset($data['tmuser_info']['acct_id']) ? $data['tmuser_info']['acct_id'] : $user->name;
  $invoice_id = isset($data['tmuser_info']['invoice_id']) ? $data['tmuser_info']['invoice_id'] : '';
  $param = tm_api_get_command_param('payment_request', $acct_id);
  $param['command1']['invoice_id'] = $invoice_id;
  //if "default" payment plan id is selected then
  //payment plan id will not be added in the TM request
  if (isset($data['payment_plan_id']) && $data['payment_plan_id']!='default') {
    $param['command1']['payment_plan_id'] = $data['payment_plan_id'];
  }
  $param['command1']['calling_reason'] = 'P';
  $param['command1']['mop'][0]['auth_mode'] = 'O';
  $param['command1']['mop'][0]['amount'] = $data['amount'];
  $param['command1']['mop'][0]['pmt_type'] = $data['ctype'];
  $param['command1']['mop'][0]['credit_card']['cc_num'] = $data['cc_num'];
  $param['command1']['mop'][0]['credit_card']['cin'] = $data['cin'];
  $param['command1']['mop'][0]['credit_card']['exp_date'] = $data['exp_date'];
  $param['command1']['cart_items']['order_line_item'] = 1;
  $json = drupal_json_encode($param);
  $response = array();
  $response = tm_api_call($json, 'ticketing_services.aspx');
  //check for sth status and validate with response.
  $result = 0;
  if (isset($response['response_ok']) && $response['response_ok']) {
    return $response;
  }
  return 0;
}

/**
 * TM API call to fetch account rep of user
 */
function _tmapi_get_seats_on_account($acct_id = NULL) {
  $param = tm_api_get_command_param('get_seats_on_account', $acct_id);
  $param['command1']['uid'] = 'ATS01';
  $json = drupal_json_encode($param);
  $response = array();
  //$response = tm_api_call($json, 'atsext/ext.asp');
  $response = tm_api_call($json, 'ticketing_services.aspx');
  //check for acct_rep status and validate with response.
  $result = 0;
  if (isset($response['response_ok']) && $response['response_ok']) {
    $data = drupal_json_decode($response['response']);
    if (!empty($data) && count($data) && !$data['header']['error_detail']) {
      $result = 1;
      // we will update the account rep information of TM user
      tm_update_tmapi_seats_hold($acct_id, $data);
    }
  }
  return $result;
}


function tmapi_get_payment_plans() {
  $param = tm_api_get_command_param('payment_plan_details');
  $json = drupal_json_encode($param);
  $response = $plan_schedules = array();
  $response = tm_api_call($json, 'ticketing_services.aspx');
  if(isset($response['response_ok']) && $response['response_ok']) {
    $data = drupal_json_decode($response['response']);
    if (isset($data['command1']['result']) && $data['command1']['result'] == 0) {
      $plans = (isset($data['command1']['plans']) && count($data['command1']['plans'])>0) ? $data['command1']['plans'] : false;
      $payment_plans = explode(',', variable_get('payment_plans', ''));
      if ($plans) {
        foreach($plans as $plan) {
          if (in_array($plan['payment_plan_id'], $payment_plans)) {
            $plan_schedules[$plan['payment_plan_id']] = $plan;
            $sort_arr[$plan['payment_plan_id']] = $plan['plan_desc'];
          }
        }
        //sorting plan names
        arsort($sort_arr);
        foreach($sort_arr as $sa_key => $sa_value){
          $plans_sch[$sa_key] = $plan_schedules[$sa_key];
        }
        array_unshift($plans_sch, array(
          'payment_plan_id' => 'default',
          'plan_long_name' => 'Full Payment',
          'plan_desc' => 'Full Payment',
          'suppress_payment_plan_details' => 'N'
        ));
        return $plans_sch;
      
      }
      return $plans;
    }
    else {
      return false;
    }
  }
  return 1;
}

/*
* Function to fetch Payment Schedules
*/
function tmapi_get_payment_schedules($acct_id=NULL, $invoice_id=NULL, $plan_id=NULL) {
  if(isset($plan_id) && !empty($plan_id)){ 
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