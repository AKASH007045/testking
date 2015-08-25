<?php
/**
 * @file
 * If you change any thing please provide appropriate comment.
 */

function _custom_tm_rest_call_test($ch1, $cookies_file, $web_url = NULL) {
  $adb_option_defaults = array(
      CURLOPT_VERBOSE         => 1,
      CURLOPT_URL             => $web_url, 
      CURLOPT_CUSTOMREQUEST   => 'GET',
      CURLOPT_POST            => false,
      CURLOPT_POSTFIELDS      => NULL,
      CURLOPT_RETURNTRANSFER  => TRUE,
      CURLOPT_COOKIEJAR       => $cookies_file
  );
  curl_setopt_array($ch1, $adb_option_defaults);
 
  $response = curl_exec($ch1);
  $info = curl_getinfo($ch1);
  $error= curl_error($ch1);
  $httpcode = curl_getinfo($ch1, CURLINFO_HTTP_CODE);
  $header_size = curl_getinfo($ch1, CURLINFO_HEADER_SIZE);
  
  return array('httpcode' => $httpcode, 'response' => $response, 'error' => $error, 'info' => $info);
}

function _custom_tm_rest_login($ch1, $cookies_file) {
  $cookies_file = tempnam("cookies", "CURLCOOKIE");
  $fields = array(
      'username' => variable_get('nfl_feed_user', 'iomedia'),
      'password' => variable_get('nfl_feed_password', 'swc22cza654B')
  );
  $fields_string = "";
  $fields_string = http_build_query($fields);

  $header = array(
      'Content-Length: 178',
      'Connection: Keep-Alive',
      'Keep-Alive: timeout=15, max=100'
  );
  $auth_url = variable_get('nfl_feed_base_url', 'http://feeds.nfl.com/feeds/auth');
  $adb_option_defaults = array(
      CURLOPT_VERBOSE         => 1,
      CURLOPT_URL             => $auth_url,
      CURLOPT_POST            => 1,
      CURLOPT_POSTFIELDS      => $fields_string,
      CURLOPT_ENCODING        => "",
      CURLOPT_RETURNTRANSFER  => TRUE,
      CURLOPT_COOKIEJAR       => $cookies_file,
  );
  curl_setopt_array($ch1, $adb_option_defaults);
  $response = curl_exec($ch1);
  $prse = custom_nfl_import_parseFeedXml($response);
  $feed_url = str_replace("/feeds/auth", $prse, $auth_url);
  $response = $feed_url;
  
  $info = curl_getinfo($ch1);
  $error= curl_error($ch1);
  $httpcode = curl_getinfo($ch1, CURLINFO_HTTP_CODE);
  $header_size = curl_getinfo($ch1, CURLINFO_HEADER_SIZE);
  return array('httpcode' => $httpcode, 'response' => $response, 'error' => $error, 'info' => $info);
}

/**
 * Function to import XML feed for Events
 */
function custom_nfl_import_events_import($form, &$form_state) {
  $form = array();
  $auth_url = variable_get('nfl_feed_base_url', 'http://feeds.nfl.com/feeds/auth');
  $form['category'] = array(
    '#type' => 'select',
    '#title' => t('Category to Import'),
    '#options' => array(
    $auth_url => t($auth_url)
    ),
    '#description' => t('Full Game Schedule with Scores.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import Game Events'),
  );
  
  return $form;
}
function custom_nfl_import_events_import_submit($form, &$form_state) {
  $home_team_nid = custom_nfl_get_hometeam_nid();
  if (!$home_team_nid){
   watchdog('custom_nfl_import', 'Home team not available, please set', array(  ), WATCHDOG_INFO);
   return drupal_set_message(t('Home team not available, please set'), 'error');
  }
  $auth_url = $form_state['values']['category']; //'http://feeds.nfl.com/feeds/auth';
  $feed = custom_nfl_import_events_feed_import($auth_url); 
  if (!$feed){
    $feed_url = l('Please set feed url', 'admin/manage/settings/nfl_feed');
    drupal_set_message($feed, 'error');
  }

  if (count($feed) > 0){	 
    foreach ($feed as $record) {
      $record['hometeam_nid'] = $home_team_nid;      
      $operations[] = array(
        'custom_game_import_create_game_event',  //The function to run on each row. This function can be found in the parent module
        array($record),
      );
    }
    $batch = array(
      'title' => t('Importing Game Event XML...'),
      'operations' => $operations,  // Runs all of the queued processes from the while loop above.
      'finished' => 'custom_nfl_import_finished', // Function to run when the import is successful
      'error_message' => t('The import has encountered an error.'),
      'progress_message' => t('Imported @current of @total records.'),
      'file' => drupal_get_path('module', 'custom_nfl_import') . '/custom_nfl_import.inc',
    );    
    batch_set($batch);
    batch_process('admin/manage/settings/nfl_feed/manual-import');
  }
  else{
    drupal_set_message("Feed has no data.");
  }
}

/**
 * Custom function
 * Invoke from hook cron
 * Purpose: Get feed from concerned site as xml format and parsed the feed to an array.
 *  every array are stored in our database as game event.
 *  We tried to fired an mail to every recipient provided in the settings.
 */
function custom_nfl_import_events_cron_import() {
  $auth_url = variable_get('nfl_feed_base_url', 'http://feeds.nfl.com/feeds/auth');
  $feed = custom_nfl_import_events_feed_import($auth_url);
  if (count($feed) > 0){
    $home_team_nid = custom_nfl_get_hometeam_nid();
    if (!$home_team_nid){
     watchdog('custom_nfl_import', 'Home team not available, please set', array(), WATCHDOG_INFO);
     return drupal_set_message(t('Home team not available, please set'), 'error');
    }
    $team_title = db_query("SELECT title FROM {node} WHERE nid = $home_team_nid")->fetchField();
    $params['feed'] = $team_title;
    $lang = LANGUAGE_NONE;
    $to = variable_get('cron_recipients', '');
    if (variable_get('game_feed_cron_mail') && $to) {    
      drupal_mail('custom_game_import', 'start', $to, $lang, $params);
    }    
    
    foreach ($feed as $record){    
      $record['hometeam_nid'] = $home_team_nid;    
      custom_game_import_create_game_event($record); //The function to run on each row. This function can be found in the parent module    
    }
    //Send an email to specified users when cron successfully run
    if (variable_get('game_feed_cron_mail') && $to) {    
      drupal_mail('custom_game_import', 'success', $to, $lang, $params);
    }
  }//else take initiate  
}

/**
 * This function runs when the batch processing is complete
 *
 * @see custom_ticketmaster_import_customer_submit(),
 */
function custom_nfl_import_finished() {    
  watchdog('custom_nfl_import', 'Data imported successfully', array(), WATCHDOG_INFO);
  return drupal_set_message(t('Data Import Completed Successfully'));
}