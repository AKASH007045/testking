<?php
/**
 * @file
 */


/**
 * A validate handler on the login form. Check supplied username/password
 * against local users table. If successful, $form_state['uid']
 * is set to the matching user ID.
 */
function custom_tm_rest_call($data) {
  $api_sufix="";
  if (variable_get('tm_api_test', 0)) {
    $api_sufix="_test";
  }
      $param = array(
          "header" => array(
              "ver" => variable_get('tm_ver' . $api_sufix, '1'),
              "src_sys_type" => variable_get('tm_src_sys_type' . $api_sufix, '2'),
              "src_sys_name" => variable_get('tm_src_sys_name'.  $api_sufix, 'IOMEDIA'),
              "archtics_version" => variable_get('tm_archtics_version' . $api_sufix, 'V605')
          ),
          "command1" => array(
            "cmd" =>  "payment_request",
            "uid" => variable_get('tm_uid' . $api_sufix, 'iomed05'),
            "dsn" => variable_get('tm_dsn' . $api_sufix, 'cowboys'),
            "site_name" => variable_get('tm_site_name' . $api_sufix, 'cowboys'),
            //"order_num" => isset($data['tmuser_info']['order_num']) ? $data['tmuser_info']['order_num'] : '21788',
            "invoice_id" => $data['tmuser_info']['invoice_id'],
            "calling_reason" => "P",
            "acct_id" => $data['tmuser_info']['acct_id'],
            "mop" => array(
              array(
                "auth_mode" => "O",
                "amount" => $data['amount'],
                "pmt_type" => $data['ctype'],
                "credit_card" => array(
                    "cc_num" => $data['cc_num'],
                    "cin" => $data['cin'],
                    "exp_date" => $data['exp_date'],
                    "first_name" => $data['first_name'],
                    "last_name" => $data['last_name'],
                    "Avs_address" => $data['Avs_address'],
                    "Avs_postal_code" => $data['Avs_postal_code'],
                )
              )
            ),
          "cart_items" => array(
            array(
              "order_line_item" => "1"
            )
          )
        ),
    );
      $json = drupal_json_encode($param);
      $reponse = array();
      // init curl object
      $ch1 = curl_init();
      // create a temp cookies file for request.
      $cookies_file = tempnam("cookeies", "CURLCOOKIE");
      $reponse_ok = 0;
      //multiple curl request for fetch user detail
      for ($i = 0; $i <= 10; $i++) {
        //curl request
        if (variable_get('tm_api_test', 0)){
          $reponse = _custom_tm_rest_call_test($ch1, $cookies_file, $json);
        }
        else{
          $reponse = _custom_tm_rest_call($ch1, $cookies_file, $json);
        }
        //stop on success
        if (isset($reponse['httpcode']) && $reponse['httpcode'] == 200) {
          $reponse_ok = 1;
          break;
        }
        //delay 3 seconds for next request. 
        sleep(3);
      }
      //curl object close.
      curl_close($ch1);
      unlink($cookies_file);
      return $reponse;
}