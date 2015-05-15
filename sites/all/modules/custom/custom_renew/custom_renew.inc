<?php
/**
 * @file
 */
/**
 * Birthday page
 */
 function custom_renew_page() {
  module_load_include('inc', 'tm_data', 'tm_data'); 
  global $user;
  $show_renew_popup = (bool) variable_get('show_renew_popup', '');
  $result = false;
  $check_pass = true; 
  $rows = array();
  $seats = array();
  
  $user_info = get_user_info($user->name);
  $acct_id = (isset($user_info->acct_id) && $user_info->acct_id)?$user_info->acct_id:"";
  $get_amount_info = get_tm_user_amount_info($acct_id);
  $get_amount_info['due_amt'] = isset($get_amount_info['due_amt']) ? $get_amount_info['due_amt'] : '';
  $get_amount_info['due_amt'] = intval($get_amount_info['due_amt']);

  $pdf_link = 'STH_15-16_RENEWAL_NEW_SeatingPricing_8.pdf';
  if(
      $show_renew_popup 
      && $check_pass
      && isset($get_amount_info['due_amt']) 
      && !empty($get_amount_info['due_amt'])
      && ($get_amount_info['due_amt']>0)
  ){    
    $query = db_select('tbl_renew_takeover', 'b');
    $result = (bool)$query->fields('b', array('show_popup'))
    ->condition('acct_id', $user->name, '=')
    ->execute()
    ->fetchField();
    
    if(!$result){  
      $seats = tm_get_seats_on_account($user->name);
      foreach ($seats as $key => $seat) {
        if (strpos(strtolower($seat->price_code), 'k') ===2) {
            $rows[$seat->price_code] = $seat->price_code;
        }
      }
      if (count($rows)) {
        $pdf_link = 'STH_15-16_RENEWAL_SeatingPricing_8.pdf';
      } 
      return theme('renew', array('name' => NULL, 'pdf_link' => $pdf_link));
    }  
    else{
      drupal_goto('home');
    }
  }
  
  if(!$result){
    drupal_goto('home');
  }  
}