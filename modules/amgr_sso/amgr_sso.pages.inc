<?php

/**
 * @file
 * AMGR SSO callback to login a user into STP.
 * @param accessToken string
 * @param redir string
 */
function amgr_sso_user_authenticate() {
  global $user;
  drupal_add_js(drupal_get_path('module', 'amgr_sso') . '/amgr_sso.js');
  $query_param = drupal_get_query_parameters();
  $accessToken = $query_param['accessToken'];
  $redir_url = !empty($query_param['redir']) ? $query_param['redir'] : 'home';
  if ($user->uid) {
    cache_clear_all('invoice_list-' . $user->name, 'cache_custom', $wildcard = FALSE);
    drupal_goto($redir_url);
  }
  if (empty($accessToken)) {
    drupal_goto('user/login');
  }
  module_load_include('inc', 'amgr_sso', 'amgr_sso.api');
  $memberDetails = getAmgrMemberId($accessToken);
  if ($memberDetails->code == 200) {
    $apiResponse = json_decode($memberDetails->data);
    $memberToken = !empty($apiResponse->umember_token) ? $apiResponse->umember_token : 0;
    if ($memberToken) {
      $acct_arr = explode('.', $memberToken);
      $acct_id = count($acct_arr) ? $acct_arr[1] : 0;
      $getUid = custom_admin_tm_authenticate($acct_id, user_password(), TRUE);
      cache_clear_all('invoice_list-' . $acct_id, 'cache_custom', $wildcard = FALSE);
      $user = user_load($getUid);
      //login finalize
      watchdog('amgr_sso_user', 'Session opened for %name.', array('%name' => $user->name));
      $user->login = REQUEST_TIME;
      db_update('users')
      ->fields(array('login' => $user->login))
      ->condition('uid', $user->uid)
      ->execute();
      drupal_session_regenerate();
      $_SESSION['access_token'] = $accessToken;
      drupal_goto($redir_url);
    }
    
    $_SESSION['access_token_expiry'] = time() + $apiResponse->expires_in;
    watchdog('AMGR oAuth', 'oAuth: Successfully authorised for %email.', array('%email' => $email));
  }
  drupal_goto('user/login');
  return '';
}

/**
 * function to redirect a user to classic AMGR
 */
function amgr_sso_classic_account_manager() {
  drupal_add_js(drupal_get_path('module', 'amgr_sso') . '/amgr_sso.js');
  $access_token = $_SESSION['access_token'];
  //print_r($access_token);die;
  $query = drupal_get_query_parameters();
  $query_param = !empty($query['redir']) ? $query['redir'] : 'buy/browse';
  $class_amgr = variable_get('classic_amgr_site_url', 'https://oss.ticketmaster.com');
  $link = $class_amgr . '/aps/' . variable_get('amgr_x_client', '') . '/EN/sso?accessToken=' . $access_token . '&redir=' .  $query_param;
  drupal_goto($link);
  exit;
}

function amgr_sso_new_account_manager() {
  drupal_add_js(drupal_get_path('module', 'amgr_sso') . '/amgr_sso.js');
  $access_token = $_SESSION['access_token'];
  $query = drupal_get_query_parameters();
  $query_param = !empty($query['redir']) ? $query['redir'] : 'dashboard';
  $amgr_url = variable_get('amgr_site_url', 'https://am.ticketmaster.com/');
  //$link = $amgr_url . '/' . variable_get('amgr_x_client', '') . '/' .  $query_param;
  $link = $amgr_url . '/sso?accessToken=' . $access_token . '&redir=' .  $query_param;
  drupal_goto($link);
  drupal_exit();
}