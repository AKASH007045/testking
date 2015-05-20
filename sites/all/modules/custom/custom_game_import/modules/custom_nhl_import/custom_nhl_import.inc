<?php
/**
 * Function to import XML feed for Events
 */
function custom_nhl_import_events_import($form, &$form_state) { 
  $form = array();	
  $form['category'] = array(
    '#type' => 'select',
    '#title' => t('Category to Import'),
    '#options' => array(      
      'schedule' => t('All Game Schedule')
    ),
    '#description' => t('Please choose the appropriate category to start the import.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import NHL Game Events'),
  );
  
  return $form;
}

/**
 * custom hook_submit().
 */
function custom_nhl_import_events_import_submit($form, &$form_state) {
  $home_team_nid = custom_nhl_get_hometeam_nid();
  if (!$home_team_nid){
   watchdog('custom_nhl_import', 'Home team not available, please set', array(), WATCHDOG_INFO);
   return drupal_set_message(t('Home team not available, please set'), 'error');
  }
  
  $feed = custom_nhl_import_events_feed_import($form_state['values']['category']);  
  if (!$feed){
   $feed_url = l('Please set feed url', 'admin/manage/settings/nhl_feed_import_settings');
   drupal_set_message($feed, 'error');
  }
  
  if(count($feed)>0){
    foreach ($feed as $record) {
	  $record['hometeam_nid'] = $home_team_nid;
	  custom_nhl_import_update_record($record);
      $operations[] = array(
        'custom_game_import_create_game_event',  //The function to run on each row. This function can be found in the parent module
        array($record),
      );
    }
    $batch = array(
      'title' => t('Importing Game Event XML...'),
      'operations' => $operations,  // Runs all of the queued processes from the while loop above.
      'finished' => 'custom_nhl_import_finished', // Function to run when the import is successful
      'error_message' => t('The import has encountered an error.'),
      'progress_message' => t('Imported @current of @total records.'),
      'file' => drupal_get_path('module', 'custom_nhl_import') . '/custom_nhl_import.inc',
    );
    
    batch_set($batch);
    batch_process('admin/ticketmaster/nhl_feed_import');
  }
  else{
    drupal_set_message("Feed has no data.");
  }
}

function custom_nhl_import_events_cron_import() {
    
  $home_team_nid = custom_nhl_get_hometeam_nid();
  if (!$home_team_nid){
   watchdog('custom_nhl_import', 'Home team not available, please set', array(), WATCHDOG_INFO);
   return drupal_set_message(t('Home team not available, please set'), 'error');
  }
  $team_title = db_query("SELECT title FROM {node} WHERE nid = $home_team_nid")->fetchField();
  $params['feed'] = $team_title;
  $lang = LANGUAGE_NONE;
  $to = variable_get('cron_recipients', ''); 
  drupal_mail('custom_game_import', 'start', $to, $lang, $params);
  
  $feed = custom_nhl_import_events_feed_import();
  
  foreach($feed as $record){    
      $record['hometeam_nid'] = $home_team_nid;
      custom_nhl_import_update_record($record);
      custom_game_import_create_game_event($record); //The function to run on each row. This function can be found in the parent module    
  }
  //Send an email to specified users when cron successfully run  
  drupal_mail('custom_game_import', 'success', $to, $lang, $params); 
  
}

/**
 * This function runs when the batch processing is complete
 *
 * @see custom_ticketmaster_import_customer_submit(),
 */
function custom_nhl_import_finished() {
  watchdog('custom_nhl_import', 'Data imported successfully', array(), WATCHDOG_INFO);
  return drupal_set_message(t('Data Import Completed Successfully'));
}

/**
 * Put some logic for home team and other team information
 * @params, $record is a reference to feeds array
 * [home-team] => NYR, is always fixed here.
 **/
function custom_nhl_import_update_record(&$record){
  
  $home_team_abbreviation = custom_game_import_get_hometeam_abbreviation($record['hometeam_nid']); //'MIA' or ""
  if (!$home_team_abbreviation){      
      watchdog('custom_game_import', 'Home team abbreviation not found', WATCHDOG_INFO);
    }
    
    $team_id = custom_game_import_get_hometeam_get_team_id(strtoupper($record['visiting-team']));
        
    $record['stadium'] = 'Madison Square Garden';
    $record['team-id'] = $team_id;    
    $record['game_type'] = 1;
    $record['home_game_event'] = 1;
    $record['team_score'] = $record['home-team-score']; //@see func custom_nhl_import_events_feed_import.
    $record['other_team_score'] = $record['visiting-team-score']; //@see func custom_nhl_import_events_feed_import.
    $event_name = strtoupper($record['home-team']) . ' <span>VS</span> ' . strtoupper($record['visiting-team']);
    
    if (trim($record['location']) == 'away') {
      $record['game_type'] = 0;
      $record['home_game_event'] = 0;      
      $event_name = strtoupper($record['visiting-team']) . ' <span>@</span> ' . strtoupper($record['home-team']);
      $record['stadium'] = '';      
    }
    $record['title'] = strip_tags($event_name);
            
    $_time = strtotime($record['date']);
    if ($record['time'] == '00:00:00 ET') {
	$_time = strtotime($record['date']) + 68400;
    }
    $record['time'] = $_time; 
}