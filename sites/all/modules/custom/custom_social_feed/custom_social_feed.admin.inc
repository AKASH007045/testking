<?php
/**
 * function to return twitter feed.
 */
/**
 * Facebook pull Admin settings.
 */
function custom_social_feed_facebook_pull_settings() {
  $form = array();
  $application_id = l(t('Application ID'), 'https://developers.facebook.com/apps');
  $form['fb_app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('App ID'),
    '#default_value' => variable_get('fb_app_id', '432913403457549'),
    '#description' => t('Unique !application_id for your facebook app.', array('!application_id' => $application_id)),
  );

  $form['fb_app_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('App Secret'),
    '#default_value' => variable_get('fb_app_secret', '643b3275f3c07a96c9fbe430207ad9e1'),
    '#description' => t('Your sites app secret.'),
  );

  $graph_link = l(t('Graph ID'), 'http://developers.facebook.com/docs/reference/api/');
  $form['fb_graph_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Graph ID'),
    '#default_value' => variable_get('fb_graph_id', '21410634615'),
    '#description' => t('The !graph_id of the page, user, or group.', array('!graph_id' => $graph_link)),
  );
  return system_settings_form($form);
}

function custom_social_feed_instagram_settings() {
  $form = array();
  $form['inst_client_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Client ID'),
      '#default_value' => variable_get('inst_client_id', 'd45989cdd95e43d9bfc291694b4b369a'),
      '#description' => t('Unique Client ID for your Instagram app.'),
  );

  $form['inst_client_secret'] = array(
      '#type' => 'textfield',
      '#title' => t('Client Secret'),
      '#default_value' => variable_get('inst_client_secret', '11bdb757e2fd43a19c40b3760671777a'),
      '#description' => t('Instagrams app secret.'),
  );

  $form['inst_user_id'] = array(
      '#type' => 'textfield',
      '#title' => t('User ID'),
      '#default_value' => variable_get('inst_user_id', '2122607'),
      '#description' => t('Instagram User ID.'),
  );

  $form['inst_access_token'] = array(
      '#type' => 'textfield',
      '#title' => t('Access Token'),
      '#default_value' => variable_get('inst_access_token', ''),
      '#description' => t('Instagram Access Token.'),
  );

  $client_id = variable_get('inst_client_id', '');
  $client_secret = variable_get('inst_client_secret', '');
  $redirect_uri = url("instagram/inst_code", array("absolute" => TRUE));
  $oauth_url = "https://instagram.com/oauth/authorize/?client_id={$client_id}&redirect_uri={$redirect_uri}&response_type=code";
  $form['inst_button_1'] = array(
      '#type' => 'markup',
      '#markup' => l(t('Get Instagram Access Token'), $oauth_url),
      '#description' => t('Instagram User ID.'),
  );
  return system_settings_form($form);
}
/**
 * Instagram pull Admin settings.
 */
function custom_social_feed_instagram_get_code() {
  $param_arr = array(
    'client_id' => variable_get('inst_client_id', 'd45989cdd95e43d9bfc291694b4b369a'),
    'client_secret' => variable_get('inst_client_secret', '11bdb757e2fd43a19c40b3760671777a'),
    'grant_type' => 'authorization_code',
    'redirect_uri' => url("instagram/inst_code", array("absolute" => TRUE)),
    'code' => $_GET["code"]
  );
  $oauth_url = "https://api.instagram.com/oauth/access_token";
  $cdata = getInstagramFeeds($oauth_url, $param_arr, TRUE);
  $cdata = json_decode($cdata);

  variable_set('inst_access_token', $cdata->access_token);
  drupal_goto('admin/manage/settings/instagram');
  return "";
}

function custom_social_feed_twitter_settings() {
  $form = array();
   $form['twitter_feed_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Feed'),
    '#default_value' => variable_get('twitter_feed_url', '@TBBuccaneers'),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}