<?php
/**
 * Function to import XML feed for Events
 */
function custom_nba_import_events_import($form, &$form_state) { 
  $form = array();	
  $form['category'] = array(
    '#type' => 'select',
    '#title' => t('Category to Import'),
    '#options' => array(
      'preseason' => t('Pre Season'),
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
 * @return false if we dont get any url set.
 * We don't want any default team's feed
 * so, we modified the function
 */
function custom_nba_import_get_feed_url($category = NULL){
  if (isset($category) && !empty($category) && $category=='preseason'){
    //eg. $url = "http://data.nba.com/data/5s/v2014/xml/mobile_teams/nba/2014/teams/knicks_schedule_01.xml";
    $feed_url = variable_get('nba_preseason_game_feed_url', '');
  }
  else {
    //$url = "http://data.nba.com/data/5s/v2014/xml/mobile_teams/nba/2014/teams/knicks_schedule_02.xml";
    $feed_url = variable_get('nba_game_feed_url', '');
  }
  
  return $feed_url;
}

/**
 * custom hook_submit().
 */
function custom_nba_import_events_import_submit($form, &$form_state) {
  $home_team_nid = custom_nba_get_hometeam_nid();
  if (!$home_team_nid){
   watchdog('custom_nba_import', 'Home team not available, please set', array(), WATCHDOG_INFO);
   return drupal_set_message(t('Home team not available, please set'), 'error');
  }
  
  $feed_url = custom_nba_import_get_feed_url($form_state['values']['category']);
  if (!$feed_url){
   $feed_url = l('Please set feed url', 'admin/manage/settings/nba_feed_import_settings');
   drupal_set_message($feed_url, 'error');
  }
  $feed = custom_nba_import_parseFeedXml($feed_url);

  if(count($feed)>0){
    foreach ($feed as $record) {
	  $record['hometeam_nid'] = $home_team_nid;
	  custom_nba_import_update_record($record);
      $operations[] = array(
        'custom_game_import_create_game_event',  //The function to run on each row. This function can be found in the parent module
        array($record),
      );
    }
    $batch = array(
      'title' => t('Importing Game Event XML...'),
      'operations' => $operations,  // Runs all of the queued processes from the while loop above.
      'finished' => 'custom_nba_import_finished', // Function to run when the import is successful
      'error_message' => t('The import has encountered an error.'),
      'progress_message' => t('Imported @current of @total records.'),
      'file' => drupal_get_path('module', 'custom_nba_import') . '/custom_nba_import.inc',
    );
    
    batch_set($batch);
    batch_process('admin/ticketmaster/nba_feed_import');
  }
  else{
    drupal_set_message("Feed has no data.");
  }
}

function custom_nba_import_events_cron_import() {
  $feed_url[] = custom_nba_import_get_feed_url('preseason');
  $feed_url[] = custom_nba_import_get_feed_url();
  
  $feed[] = custom_nba_import_parseFeedXml($feed_url[0]);
  $feed[] = custom_nba_import_parseFeedXml($feed_url[1]);
  
  $home_team_nid = custom_nba_get_hometeam_nid();
  if (!$home_team_nid){
   watchdog('custom_nba_import', 'Home team not available, please set', array(), WATCHDOG_INFO);
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
      custom_nba_import_update_record($record);
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
function custom_nba_import_finished() {
  watchdog('custom_nba_import', 'Data imported successfully', array(), WATCHDOG_INFO);
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
function custom_nba_import_update_record(&$record){
  $home_team_abbreviation = custom_game_import_get_hometeam_abbreviation($record['hometeam_nid']); //'MIA' or ""
  if (!$home_team_abbreviation){      
      watchdog('custom_game_import', 'Home team abbreviation not found', WATCHDOG_INFO);
    }
    
    if ($record['visiting-team'] == $home_team_abbreviation) {
       $team_id = custom_game_import_get_hometeam_get_team_id(strtoupper($record['home-team']));
    }
    else{
       $team_id = custom_game_import_get_hometeam_get_team_id(strtoupper($record['visiting-team']));
    }
    $record['team-id'] = $team_id;    
    $record['game_type'] = 1;
    $record['home_game_event'] = 1;
    $record['team_score'] = $record['home-team-score'];
    $record['other_team_score'] = $record['visiting-team-score'];
    $event_name = strtoupper($record['home-team']) . ' <span>VS</span> ' . strtoupper($record['visiting-team']);
    
    if ($record['visiting-team'] == $home_team_abbreviation) {
      $record['game_type'] = 0;
      $record['home_game_event'] = 0;
      $record['team_score'] = $record['visiting-team-score'];
      $record['other_team_score'] = $record['home-team-score'];
      $event_name = strtoupper($record['visiting-team']) . ' <span>@</span> ' . strtoupper($record['home-team']);
    }
    $record['title'] = strip_tags($event_name);
        
    if (strstr($record['time'], 'Final')){
      //$record['time'] = trim(str_replace(array('ET', 'pm'), array('','PM'), $record['date'] . ' ' . '9:00 pm ET'));       
      $record['time'] = '';
    }
    $record['time'] = strtotime($record['time']);
}