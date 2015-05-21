<?php
/**
 * Function to import XML feed for Events
 */
function custom_mlb_import_events_import($form, &$form_state) { 
  $form = array();	
  $form['category'] = array(
    '#type' => 'select',
    '#title' => t('Category to Import'),
    '#options' => array(
      'schedule' => t('Full Game Schedule with Scores')
    ),
    '#description' => t('Please choose the appropriate category to start the import.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import Game Events'),
  );
  
  return $form;
}

/**
 * custom hook_submit().
 */
function custom_mlb_import_events_import_submit($form, &$form_state) {
  $home_team_nid = custom_mlb_get_hometeam_nid();
  if (!$home_team_nid){
   watchdog('custom_mlb_import', 'Home team not available, please set', array(), WATCHDOG_INFO);
   return drupal_set_message(t('Home team not available, please set'), 'error');
  }
  
  $feed_url = variable_get('mlb_game_feed_url', '');
  if (!$feed_url){
   $feed_url = l('Please set feed url', 'admin/manage/settings/mlb_feed_import_settings');
   drupal_set_message($feed_url, 'error');
  }
  $feed = custom_mlb_import_parseFeedXml($feed_url);

  if(count($feed)>0){
    foreach ($feed as $record) {
	  $record['hometeam_nid'] = $home_team_nid;
	  custom_mlb_import_update_record($record);
      $operations[] = array(
        'custom_game_import_create_game_event',  //The function to run on each row. This function can be found in the parent module
        array($record),
      );
    }
    $batch = array(
      'title' => t('Importing Game Event XML...'),
      'operations' => $operations,  // Runs all of the queued processes from the while loop above.
      'finished' => 'custom_mlb_import_finished', // Function to run when the import is successful
      'error_message' => t('The import has encountered an error.'),
      'progress_message' => t('Imported @current of @total records.'),
      'file' => drupal_get_path('module', 'custom_mlb_import') . '/custom_mlb_import.inc',
    );
    
    batch_set($batch);
    batch_process('admin/ticketmaster/mlb_feed_import');
  }
  else{
    drupal_set_message("Feed has no data.");
  }
}

function custom_mlb_import_events_cron_import() {
  $feed[] = custom_mlb_import_parseFeedXml();
  
  $home_team_nid = custom_mlb_get_hometeam_nid();
  if (!$home_team_nid){
   watchdog('custom_mlb_import', 'Home team not available, please set', array(), WATCHDOG_INFO);
   return drupal_set_message(t('Home team not available, please set'), 'error');
  }
  $team_title = db_query("SELECT title FROM {node} WHERE nid = $home_team_nid")->fetchField();
  $params['feed'] = $team_title;
  $lang = LANGUAGE_NONE;
  $to = variable_get('cron_recipients', ''); 
  drupal_mail('custom_game_import', 'start', $to, $lang, $params);
  
  foreach($feed as $fd){
    foreach ($fd as $record) {
      $record['hometeam_nid'] = $home_team_nid;
      custom_mlb_import_update_record($record);
      custom_game_import_create_game_event($record); //The function to run on each row. This function can be found in the parent module
    }
  }
  //Send an email to specified users when cron successfully run  
  drupal_mail('custom_game_import', 'success', $to, $lang, $params);    
  
}

/**
 * This function runs when the batch processing is complete
 *
 * @see custom_ticketmaster_import_customer_submit(),
 */
function custom_mlb_import_finished() {
  watchdog('custom_mlb_import', 'Data imported successfully', array(), WATCHDOG_INFO);
  return drupal_set_message(t('Data Import Completed Successfully'));
}

/**
 * Now we are not using this function.
 *  We have kept here for future
 */
function get_converted_time($date_val = NULL, $from_timezone, $to_timezone){
  if (isset($date_val) && !empty($date_val)){
    $dte = new DateTime($date_val, new DateTimeZone($from_timezone));
    $dte->setTimeZone(new DateTimeZone($to_timezone));
    return $dte->format('Y-m-d H:i:s');
  }
}

/**
 * Put some logic for home team and other team information
 * @params, $record is a reference to feeds array
 **/
function custom_mlb_import_update_record(&$record){
  $team_id = custom_game_import_get_hometeam_get_team_id(strtoupper($record['visiting-team']));
  $record['team-id'] = $team_id;    
  $record['time'] = strtotime($record['time']);    
}