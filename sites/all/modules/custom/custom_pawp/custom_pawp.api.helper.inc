<?php
/**
 * @file

/**
 * A validate handler on the login form. Check supplied username/password
 * against local users table. If successful, $form_state['uid']
 * is set to the matching user ID.
 */
function custom_tm_pawp_rest_call($udata) {
  $acct_id = $udata['tmuser_info']['acct_id'];
  $pass = isset($_SESSION['tmupa']) ? base64_decode($_SESSION['tmupa']) : '';
  $tm_cc_update = _tmapi_cc_update_call($udata);
  if ($tm_cc_update) {
    $param = tm_api_get_command_param('customer_update', $acct_id);
    $param['command1']['pin'] = $pass;
    $customer = $_SESSION['cust_info'];
    $param['command1']['customer']['acct_id'] = $acct_id;
    $param['command1']['other_info_10'] = $tm_cc_update;
    $param['command1']['other_info_4'] = 'YES';
    if (!empty($customer)) {
      $param['command1']['customer'] =  $customer;
    }
    $json = drupal_json_encode($param);
    $api_sufix="";
    $response = array();
    // init curl object
    $ch1 = curl_init();
    // create a temp cookies file for request.
    $cookies_file = tempnam("cookeies", "CURLCOOKIE");
    $reponse_ok = 0;
    //multiple curl request for fetch user detail
    for ($i = 0; $i <= 15; $i++) {
      if (variable_get('tm_api_test', 0)) {
        $response = _custom_tm_rest_call_test($ch1, $cookies_file, $json, 'ticketing_services.aspx');
      }
      else{
        $response = custom_pawp_live_tm_rest($ch1, $cookies_file, $json);
      }
      //stop on success
      if (isset($response['httpcode']) && $response['httpcode'] == 200) {
        $reponse_ok = 1;
        break;
      }
      //delay 3 seconds for next request. 
      sleep(3);
    }
    //curl object close.
    curl_close($ch1);
    unlink($cookies_file);
    //check for sth status and validate with response.
    $result = 0;
    if ($reponse_ok) {
      $data = drupal_json_decode($response['response']);
      $udata['command'] = 'customer_update';
      if (!isset($data['command1']['error_detail']) && isset($data['command1']['result']) && $data['command1']['result'] == 0) {
        $result = 1;
        $udata['result'] = 1;
      }
      custom_pawp_update_tm_transcation_table($udata, $response['response']);
    }
    return $result;
  }
  return 0;
}

/**
 * A validate handler on the login form. Check supplied username/password
 * against local users table. If successful, $form_state['uid']
 * is set to the matching user ID.
 */
function custom_tm_pawp_no_thanks_cc_update_call($udata) {
  $udata = array();
  global $user;
  $form = array();
  if ($user->uid) {
    $acct_id = $user->name;
  }
  $pass = isset($_SESSION['tmupa']) ? base64_decode($_SESSION['tmupa']) : '';
  $param = tm_api_get_command_param('customer_update', $acct_id);
  $param['command1']['pin'] = $pass;
  $customer = $_SESSION['cust_info'];
  $param['command1']['customer']['acct_id'] = $acct_id;
  $param['command1']['other_info_10'] = '';
  $param['command1']['other_info_4'] = 'NO';
  if (!empty($customer)) {
    $param['command1']['customer'] =  $customer;
  }
  $json = drupal_json_encode($param);
  $api_sufix="";
  $response = array();
  // init curl object
  $ch1 = curl_init();
  // create a temp cookies file for request.
  $cookies_file = tempnam("cookeies", "CURLCOOKIE");
  $reponse_ok = 0;
  //multiple curl request for fetch user detail
  for ($i = 0; $i <= 15; $i++) {
    if (variable_get('tm_api_test', 0)) {
      $response = _custom_tm_rest_call_test($ch1, $cookies_file, $json, 'ticketing_services.aspx');
    }
    else{
      $response = custom_pawp_live_tm_rest($ch1, $cookies_file, $json);
    }
    //stop on success
    if (isset($response['httpcode']) && $response['httpcode'] == 200) {
      $reponse_ok = 1;
      break;
    }
    //delay 3 seconds for next request. 
    sleep(3);
  }
  //curl object close.
  curl_close($ch1);
  unlink($cookies_file);
  //check for sth status and validate with response.
  $result = 0;
  if ($reponse_ok) {
    $data = drupal_json_decode($response['response']);
    $udata['command'] = 'customer_update';
    if (!isset($data['command1']['error_detail']) && isset($data['command1']['result']) && $data['command1']['result'] == 0) {
      $result = 1;
      $udata['result'] = -1;
    }
    $udata['tmuser_info']['acct_id'] = $acct_id;
    custom_pawp_update_tm_transcation_table($udata, $response['response']);
  }
  return $result;
}


/**
 * TM invoice_list API to fetch invoice id 
 */
function _tmapi_cc_update_call($udata = NULL) {
  $acct_id = $udata['tmuser_info']['acct_id'];
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
  $response = array();
  // init curl object
  $ch1 = curl_init();
  // create a temp cookies file for request.
  $cookies_file = tempnam("cookeies", "CURLCOOKIE");
  $reponse_ok = 0;
  //multiple curl request for fetch user detail
  for ($i = 0; $i <= 15; $i++) {
    if (variable_get('tm_api_test', 0)) {
      $response = _custom_tm_rest_call_test($ch1, $cookies_file, $json, 'ticketing_services.aspx');
    }
    else{
      $response = custom_pawp_live_tm_rest($ch1, $cookies_file, $json);
    }
    //stop on success
    if (isset($response['httpcode']) && $response['httpcode'] == 200) {
      $reponse_ok = 1;
      break;
    }
    //delay 3 seconds for next request. 
    sleep(3);
  }
  //curl object close.
  curl_close($ch1);
  unlink($cookies_file);
  //check for sth status and validate with response.
  $result = 0;
  if ($reponse_ok) {
    $data = drupal_json_decode($response['response']);
    $udata['command'] = 'cc_update';
    if (!isset($data['command1']['error_detail']) && isset($data['command1']['cc_seq_num']) && $data['command1']['cc_seq_num']) {
      $result = $data['command1']['cc_seq_num'];
      $udata['result'] = '1';
    }
    custom_pawp_update_tm_transcation_table($udata, $response['response']);
  }
  return $result;
}

function custom_pawp_update_tm_transcation_table($data, $response = '') {
  db_insert('tm_pawp_data')
  ->fields(array(
    'cc_type' => (isset($data['ctype']) && $data['ctype']) ? $data['ctype'] : '',
    'cc_num' => (isset($data['cc_num']) && $data['cc_num']) ? '************'. substr($data['cc_num'], -4, 4) : '',
    'exp_date' => (isset($data['exp_date']) && $data['exp_date']) ? $data['exp_date'] : '',
    'acct_id' => (isset($data['tmuser_info']['acct_id']) && $data['tmuser_info']['acct_id']) ? $data['tmuser_info']['acct_id'] : '',
    'timestamp' => time(),
    'email_addr' => $data['email'],
    'command' => $data['command'],
    'result' => $data['result'],
    'tm_raw_response' => $response))->execute();
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
 */
function _custom_tm_rest_call_test($ch1, $cookies_file, $json = NULL, $web_url = NULL) {
  $api_sufix="";
  if (variable_get('tm_api_test', 0)) {
    $api_sufix="_test";
  }
  $dsn = variable_get('tm_dsn' . $api_sufix, 'bucs602');
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
      CURLOPT_SSLCERT => realpath(drupal_get_path('module', 'custom_admin') . '/certificates/test_tm_v2.pem'),
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
 *    - responce - response string.
 *     
 */
function custom_pawp_live_tm_rest($ch1, $cookies_file, $json = NULL) {
  //ticketmaster url
  $adb_url = variable_get('tm_login_url', 'https://ws.ticketmaster.com/archtics/ats/ticketing_services.aspx?dsn=sackings');
  // set header for ticketmaster
  $header = array(
      'Content-Type: text/json; charset=utf-8',
      'Content-Length:' . strlen($json),
      'Connection: Keep-Alive',
      'Keep-Alive: timeout=15, max=100'
  );
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
      CURLOPT_SSLCERT => realpath(drupal_get_path('module', 'custom_admin') . '/certificates/live_tm_v2.pem'),
      CURLOPT_SSLCERTPASSWD => 'letmein',
      // curl cookies file.
      CURLOPT_COOKIEJAR => $cookies_file,
  );
  curl_setopt_array($ch1, $adb_option_defaults);
  $responce = curl_exec($ch1);
  $httpcode = curl_getinfo($ch1, CURLINFO_HTTP_CODE);
  $header_size = curl_getinfo($ch1, CURLINFO_HEADER_SIZE);
  return array('httpcode' => $httpcode, 'response' => $responce);
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
 */
function _custom_tm_rest_call($ch1, $cookies_file, $json = NULL, $web_url = NULL) {
  $dsn = variable_get('tm_dsn', 'sackings');
  //ticketmaster url
  //$adb_url = variable_get('tm_login_url', 'https://ws.ticketmaster.com/archtics/ats/ticketing_services.aspx?dsn=sackings');
  $adb_url = 'https://ws.ticketmaster.com/archtics/ats/' . $web_url . '?dsn=' . $dsn;
  // set header for ticketmaster
  $header = array(
      'Content-Type: text/json; charset=utf-8',
      'Content-Length:' . strlen($json),
      'Connection: Keep-Alive',
      'Keep-Alive: timeout=15, max=100'
  );
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
      CURLOPT_SSLCERT => realpath(drupal_get_path('module', 'custom_admin') . '/certificates/live_tm_v2.pem'),
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