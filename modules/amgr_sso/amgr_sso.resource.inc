<?php
// @file

/**
 * Login a user using the specified accessToken.
 *
 * @param $accessToken
 *   accessToken to be passed in.
 */
function amgr_sso_resource_login($accessToken) {
  global $user;
  module_load_include('inc', 'amgr_sso', 'amgr_sso.api');
  if ($user->uid) {
    // user is already logged in
    return services_error(t('User already logged in'), 406);
  }
  if (empty($accessToken)) {
    return services_error(t('No valid access token found'), 401);
  }
  
  $memberDetails = getAmgrMemberId($accessToken);
  if ($memberDetails->code == 200) {
    $apiResponse = json_decode($memberDetails->data);
    $memberToken = !empty($apiResponse->umember_token) ? $apiResponse->umember_token : 0;
    if ($memberToken) {
      $acct_arr = explode('.', $memberToken);
      $acct_id = count($acct_arr) ? $acct_arr[1] : 0;

      $getUid = custom_admin_tm_authenticate($acct_id, user_password(), TRUE);
      if ($getUid) {
        $user = user_load($getUid);
        if ($user->uid) { 
          user_login_finalize();
          $return = new stdClass();
          $return->sessid = session_id();
          $return->session_name = session_name();
          $return->X_CSRF_TOKEN = drupal_get_token('services');
          $return->acct_id = $user->name;
          $user_data = get_user_info($user->name);
          $user_info = (isset($user_data->customer) && $user_data->customer) ? $user_data->customer : '';
          $return->email = $user->mail;
          $return->customer = $user_info;
          $roles = array_keys($user->roles);
          $_SESSION['access_token'] = $accessToken;
          return $return;
        }
      }
    }
    watchdog('AMGR oAuth', 'Invalid login attempt  for %username.', array('%username' => $acct_id));
  }
   return services_error(t('Invalid Access Token.'), 401);
}

function amgr_sso_resource_autologout($accessToken) {
  module_load_include('inc', 'amgr_sso', 'amgr_sso.api');
  $memberDetails = getAmgrMemberId($accessToken);
  if ($memberDetails->code == 200) {
    $apiResponse = json_decode($memberDetails->data);
    $memberToken = !empty($apiResponse->umember_token) ? $apiResponse->umember_token : 0;
    if ($memberToken) {
      $acct_arr = explode('.', $memberToken);
      $acct_id = count($acct_arr) ? $acct_arr[1] : 0;
      $user_account = user_load_by_name($acct_id);
      if ($user_account->uid) {
        drupal_session_destroy_uid($user_account->uid);
        return true;
      }
    }
  }
  return false;
}

function amgr_sso_resource_generate_link($url) {
  global $user;
  if ($user->uid) {
    $urls = explode('?', $url);
    $redir = 'redir=dashboard';
    $redirClassic = 'redir=buy/browse';
    $access_token = $_SESSION['access_token'];
    
    if ($urls[0] == 'amgr') {
      if (!empty($urls[1])) {
        $redir = $urls[1];
      }
      $amgr_url = variable_get('amgr_site_url', 'https://am.ticketmaster.com/');
      $link = $amgr_url . '/sso?accessToken=' . $access_token . '&' .  $redir;
      return array('status' => 1, 'url' => $link);
    }
    elseif ($urls[0] == 'classic-amgr') {
      if (!empty($urls[1])) {
        $redirClassic = $urls[1];
      }
      $class_amgr = variable_get('classic_amgr_site_url', 'https://oss.ticketmaster.com');
      $link = $class_amgr . '/aps/' . variable_get('amgr_x_client', '') . '/EN/sso?accessToken=' . $access_token . '&' .  $redirClassic;
      return array('status' => 1, 'url' => $link);
    }
  }
  return array('status' => 0, 'url' => NULL);
}