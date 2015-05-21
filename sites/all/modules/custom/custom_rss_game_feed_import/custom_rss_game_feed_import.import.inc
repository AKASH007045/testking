<?php
/**
 * Function to import XML feed for Events
 */
function custom_rss_game_feed_import_events_import($form, &$form_state) { 
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

function get_feed_url($category=NULL){
  if(isset($category) && !empty($category) && $category=='preseason'){
    $url = "http://data.nba.com/data/5s/v2014/xml/mobile_teams/nba/2014/teams/suns_schedule_01.xml";
    $feed_url = variable_get('preseason_game_feed_url', $url);
  }
  else{
    $url = "http://data.nba.com/data/5s/v2014/xml/mobile_teams/nba/2014/teams/suns_schedule_02.xml";
    $feed_url = variable_get('game_feed_url', $url);
  }
  
  return $feed_url;
}

function custom_rss_game_feed_import_events_import_submit($form, &$form_state) {  
  $feed_url = get_feed_url($form_state['values']['category']);
  $feed = custom_rss_game_feed_import_parseFeedXml($feed_url);

  if(count($feed)>0){
    foreach ($feed as $record) {
      $operations[] = array(
        'custom_rss_game_feed_import_create_game_event',  // The function to run on each row
        array($record),
      );
    }
    $batch = array(
      'title' => t('Importing Game Event XML...'),
      'operations' => $operations,  // Runs all of the queued processes from the while loop above.
      'finished' => 'custom_rss_game_feed_import_finished', // Function to run when the import is successful
      'error_message' => t('The import has encountered an error.'),
      'progress_message' => t('Imported @current of @total records.'),
      'file' => drupal_get_path('module', 'custom_rss_game_feed_import') . '/custom_rss_game_feed_import.import.inc',
    );
    
    batch_set($batch);
    batch_process('admin/ticketmaster/feed_import');
  }
  else{
    drupal_set_message("Feed has no data.");
  }
}

function custom_rss_game_feed_import_events_cron_import() {
  $feed_url[] = get_feed_url('preseason');
  $feed_url[] = get_feed_url();
  
  $feed[] = custom_rss_game_feed_import_parseFeedXml($feed_url[0]);
  $feed[] = custom_rss_game_feed_import_parseFeedXml($feed_url[1]);  
  
  $params['feed'] = 'Kings';
  $lang = LANGUAGE_NONE;
  $to = variable_get('cron_recipients', ''); 
  drupal_mail('custom_cron_alert', 'start', $to, $lang, $params);
  
  foreach($feed as $fd){
    foreach ($fd as $record) {
      custom_rss_game_feed_import_create_game_event($record);
    }
  }  
  //Send an email to specified users when cron successfully run  
  drupal_mail('custom_cron_alert', 'success', $to, $lang, $params);  
}
/**
 * This function runs when the batch processing is complete
 *
 * @see custom_ticketmaster_import_customer_submit(),
 */
function custom_rss_game_feed_import_finished() {
  watchdog('custom_rss_game_feed_import', 'CSV data imported successfully', array(), WATCHDOG_INFO);
  return drupal_set_message(t('Data Import Completed Successfully'));
}

function custom_rss_game_feed_import_create_game_event($record) {
  $count = $node_count = 0;
  
  $game = db_select('field_data_field_game_code', 't')
  ->fields('t', array('entity_id'))
  ->condition('t.field_game_code_value', $record['gamecode'], '=')
  ->range(0, 1)->execute()->fetchField();
  
  if(strstr($record['time'], 'Final')){
    $_time = "";
  }
  else{
    $conv_date = get_converted_time($record['time'], 'America/New_York', 'America/Los_Angeles');
    $_time = strtotime($conv_date);
    //$_time = strtotime($record['time']);
  }
    
  if (isset($game) && $game) { 
    $new_node = node_load($game);
    $lang = $new_node->language;
    $new_node->field_team_score[$lang][0]['value'] = $record['home-team-score'];
    $new_node->field_other_team_score[$lang][0]['value'] = $record['visiting-team-score'];
    if ($record['visiting-team'] == 'SAC') {
      $new_node->field_team_score[$lang][0]['value'] = $record['visiting-team-score'];
      $new_node->field_other_team_score[$lang][0]['value'] = $record['home-team-score'];
    }
    
    if(isset($_time) && !empty($_time)){
      $new_node->field_event_date[$lang][0]['value'] = $_time;
    }
    node_save($new_node);
    watchdog('custom_rss_game_feed_import', 'Game event update with NID: %nid', array('%nid' => $new_node->nid), WATCHDOG_INFO);
  }
  else {  
    if ($record['visiting-team'] == 'SAC') {
       $team_id = get_team_id(strtoupper($record['home-team']));
    }
    else{
       $team_id = get_team_id(strtoupper($record['visiting-team']));
    }
    
    if($team_id){
      global $user;
      $lang = LANGUAGE_NONE;
      $new_node = new stdClass();
      $new_node->uid = $user->uid;
      $new_node->status = 1;
      $new_node->language = $lang;
      $new_node->type = 'games';
      $new_node->field_game_code[$lang][0]['value'] = $record['gamecode'];
      $new_node->field_opponent_team[$lang][0]['nid'] = $team_id;
      $event_name = strtoupper($record['home-team']) . ' <span>VS</span> ' . strtoupper($record['visiting-team']);
      $new_node->field_ground[$lang][0]['value'] = $record['stadium'];
      $new_node->field_game_type[$lang][0]['value'] = 1;
      $new_node->field_home_game_event[$lang][0]['value'] = 1;
      $new_node->field_team_score[$lang][0]['value'] = $record['home-team-score'];
      $new_node->field_other_team_score[$lang][0]['value'] = $record['visiting-team-score'];
      if ($record['visiting-team'] == 'SAC') {
        $event_name = strtoupper($record['visiting-team']) . ' <span>@</span> ' . strtoupper($record['home-team']);
        $new_node->field_game_type[$lang][0]['value'] = 0;
        $new_node->field_home_game_event[$lang][0]['value'] = 0;
        $new_node->field_ground[$lang][0]['value'] = $record['stadium'];
        $new_node->field_team_score[$lang][0]['value'] = $record['visiting-team-score'];
        $new_node->field_other_team_score[$lang][0]['value'] = $record['home-team-score'];
      }
          
      //$new_node->field_event_name[$lang][0]['value'] = $event_name;
      $new_node->title = strip_tags($event_name); //variable_get('home_team', t('SAC')) . ' ' . strip_tags($event_name);
      if(isset($_time) && !empty($_time)){
        $new_node->field_event_date[$lang][0]['value'] = $_time;
        $new_node->field_game_countdown[$lang][0]['countdown_timer'] = $_time;
      }      
      node_save($new_node);
      watchdog('custom_rss_game_feed_import', 'Game event created with NID: %nid', array('%nid' => $new_node->nid), WATCHDOG_INFO);    
      $node_count++;
    }
  }  
}

function get_converted_time($date_val=NULL, $from_timezone, $to_timezone){
  if(isset($date_val) && !empty($date_val)){
    $dte = new DateTime($date_val, new DateTimeZone($from_timezone));
    $dte->setTimeZone(new DateTimeZone($to_timezone));
    return $dte->format('Y-m-d H:i:s');
  }
}

function get_team_id($abbr=NULL){
  If(!empty($abbr)){
    $query = db_select('field_data_field_team_abbreviation', 'f'); 
    $result = $query->fields('f', array('entity_id'))    
      ->condition('f.field_team_abbreviation_value', $abbr, '=')
      ->execute()
      ->fetchField();
    if($result){
      return $result;
    }
    else{
      return false;
    } 
  }  
}
