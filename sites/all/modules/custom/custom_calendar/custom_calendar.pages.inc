<?php
// @file
// Helper functions for calendar list view

/*
 * Function to generate html
 */
function custom_calendar_list_view() {
  $home_team_nid = db_query("SELECT n.entity_id FROM {field_data_field_home_team} n WHERE field_home_team_value = 1")->fetchField();
  global $user;
  $arg = arg(0);
  $calendar_link = ($arg == 'season-schedule') ? 'my-calendar' : 'calendar';
  
  $query = new EntityFieldQuery();
    $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'games')
    ->propertyCondition('status', 1)
    ->fieldOrderBy('field_event_date', 'value', 'ASC');
    $result = $query->execute(); 
  $month = '';
  $games = '';
  $c= 0;
  $next_game = 0;
  $class = '';
  $home_team_detail = get_team_info($home_team_nid, 'event_calendar');
  foreach($result['node'] as $nid => $obj) {
    $item = array();
    $node = node_load($nid); 
    $lang = isset($node->language) ? $node->language : LANGUAGE_DEFAULT;    
    $month = strtolower(date('F_Y', $node->field_event_date[$lang][0]['value']));    
    $team_id = isset($node->field_opponent_team[$lang][0]['nid']) ? $node->field_opponent_team[$lang][0]['nid'] : 0;
    $item['nid'] = $node->nid;
    $item['day'] = date('M j', $node->field_event_date[$lang][0]['value']);
    $item['time'] = date('g:i A T', $node->field_event_date[$lang][0]['value']);
    $item['num_month'] = date('n', $node->field_event_date[$lang][0]['value']);
    $item['full_year'] = date('Y', $node->field_event_date[$lang][0]['value']);
    $item['sym'] = (isset($node->field_game_type[$lang][0]['value']) && $node->field_game_type[$lang][0]['value']) ? 'VS' : '@';
    $item['team_details'] = get_team_info($team_id, 'event_calendar');
    $item['stadium'] = $node->field_ground[$lang][0]['value'];
    $game_over = (time() > $node->field_event_date[$lang][0]['value']) ? 1 : 0;
    $home_score = (isset($node->field_team_score[$lang][0]['value']) && $node->field_team_score[$lang][0]['value']) ? $node->field_team_score[$lang][0]['value'] : 0;
    $other_score = (isset($node->field_other_team_score[$lang][0]['value']) && $node->field_other_team_score[$lang][0]['value']) ? $node->field_other_team_score[$lang][0]['value'] : 0;
    if ((isset($home_score) && isset($other_score)) && (($home_score || $other_score)) && $game_over) {
      $item['result'] = 1;
      if ($home_score > $other_score) {
        $item['score'] = '<span class="list-view-child">W &nbsp;' . $home_score . '-' . $other_score . '</span>';
      }
      else {
        $item['score'] = '<span class="list-view-child">L &nbsp;' . $home_score . '-' . $other_score . '</span>';
      }
    }
    else {
      $item['result'] = 0;
    }
    if (time() < $node->field_event_date[$lang][0]['value'] && !$next_game) {
      $next_game = 1;
      $class = 'next-game-active';
    }
    else {
      $class = '';
    }
    $games .= theme('list_game_box' , array('data' => $item, 'count' => $c, 'home_team' => $home_team_detail, 'class' => $class));
    $c++;
  }
  drupal_add_css(drupal_get_path('module', 'custom_calendar') . '/css/calendar_list_view.css');
  drupal_add_js(drupal_get_path('module', 'custom_calendar') . '/js/list_view.js');
  return theme('calendar_list', array('games' => $games, 'calendar_link' => $calendar_link));
}