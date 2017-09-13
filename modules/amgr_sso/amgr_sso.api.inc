<?php

/**
 * @file
 * Access Token for amgr sso.
 */

/**
 * FAPI callback to get the Access Token.
 */
function getAmgrAccessToken($name, $pin) {
  $authTokenUrl = variable_get('amgr_oauth_token_url', '');
  $data = array(
    'client_id' => variable_get('amgr_client_id', ''),
    'client_secret' => variable_get('amgr_client_secret', ''),
    'grant_type' => 'password',
    'username' => strtolower($name),
    'password' => $pin,
  );
  //You can concatenate more tags to the data.
  $options = array(
    'method' => 'POST',
    'data' => http_build_query($data),
    'timeout' => 90,
    'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
  );
  $result = drupal_http_request($authTokenUrl, $options);
  return $result;
}

/**
 * FAPI callback to get the Member ID.
 */
function getAmgrMemberId($accessToken) {
  $member_id_url = variable_get('amgr_member_id_url', '');
  $url = str_replace('@access_token', $accessToken, $member_id_url);
  $result = drupal_http_request($url);
  return $result;
}