<?php
//@file
define('R_19', 7);
define('NOT_R_19', 8);
define('R_20', 9);
define('R_21', 10);

module_load_include('inc', 'tm_api', 'tm_api.helper');
function tm_user_birthday_access($page = NULL) {
  global $user;
  $current_month = date('n', time());
  $current_year = date('Y', time());
  $acct_id = isset($user->name) ? $user->name : 0;
  if (isset($acct_id) && $acct_id && variable_get('en_birthday', 0)) {
    $check_birthday_access = (bool) db_select('tbl_tm_user_birthday', 't')->fields('t', array('acct_id'))->condition('t.year', $current_year, '=')->condition('t.acct_id', $acct_id, '=')->execute()->fetchField();
    $user_info = get_user_info($acct_id);
    if (isset($user_info->birth_date) && $user_info->birth_date) {
      $timestamp = strtotime($user_info->birth_date);
      $birthday_month = (isset($timestamp) && $timestamp) ? date('n', $timestamp) : 0;
    }
    if ($birthday_month == $current_month && !$check_birthday_access) {
      $data = array('acct_id' => $acct_id, 'year' => $current_year);
      if ($page) {
        db_insert('tbl_tm_user_birthday')->fields($data)->execute();
      }
      return 1;
    }
  }
  return 0;
}

function tm_get_seats_on_account($acct_id) {
  $seat_detail =  db_select('tmapi_seats_hold', 't')
      ->fields('t')
      ->condition('t.acct_id', $acct_id, '=')
      ->execute()->fetchAll();
	  return $seat_detail;
}

function get_acc_rep_mail($acct_id) {
    $object = cache_get('rep_mail-' . $acct_id, 'cache_custom');
    if (!$object) {
      $mail = db_query('SELECT field_email_value FROM {tbl_user_acct_rep} acc_rep inner join {field_data_field_acct_rep_num} rep_num on acc_rep.acct_rep_num = rep_num.field_acct_rep_num_value inner join {field_data_field_email} dta_mail on dta_mail.entity_id = rep_num.entity_id  inner join {node} n on n.nid = dta_mail.entity_id WHERE acct_id = :acct_id AND n.status =  1', array(':acct_id' => $acct_id))->fetchField();
      cache_set('rep_mail-' . $acct_id, $mail, 'cache_custom');
    } else {
      $mail = $object->data;
    }
    return $mail;
}

function acc_rep_detail($acct_rep_num = NULL) {
    $object = cache_get('rep_all_detail-' . $acct_rep_num, 'cache_custom');
    if (!$object) {
        $data = db_query("SELECT node.title AS name,field_email_value as field_email_value, field_phone_value as field_phone_value, node.nid AS nid, node.created AS node_created
      FROM  {node} node
      LEFT JOIN {field_data_field_acct_rep_num} field_data_field_acct_rep_num ON node.nid = field_data_field_acct_rep_num.entity_id AND (field_data_field_acct_rep_num.entity_type = 'node' AND field_data_field_acct_rep_num.deleted = '0')
      LEFT JOIN {field_data_field_phone} field_data_field_phone ON node.nid = field_data_field_phone.entity_id
      LEFT JOIN {field_data_field_email} field_data_field_email ON node.nid = field_data_field_email.entity_id
      WHERE (( (field_data_field_acct_rep_num.field_acct_rep_num_value = :acct_rep_num ) )AND(( (node.status = '1') AND (node.type IN  ('acct_rep')) )))
      ORDER BY node_created DESC  ", array(':acct_rep_num' => $acct_rep_num))->fetchAll();
      cache_set('rep_all_detail-' . $acct_rep_num, $data, 'cache_custom');
    } else {
      $data = $object->data;
    }
    return $data;
}

function get_acct_rep_id($acct_id = NULL) {
 if ($acct_id) {
   $query = db_select('tbl_user_acct_rep', 'ar');
   $query->fields('ar', array('acct_rep_num'))
     ->condition('ar.acct_id', $acct_id, '=')
     ->execute()->fetchObject();
   return $query->acct_rep_num;
 }
 return 0;
}


//function get_acct_rep_id($acct_id = NULL) {
//  $rep_detail = cache_get('account_rep_' . $acct_id, 'cache_custom');
//  if ($rep_detail) {
//    return $rep_detail->rep_acct_id;
//  }
//  return '';
//}
/**
 * function which return account rep detail
 */

//function acct_rep_load($acct_id = NULL) {
// static $rep_detail = NULL;
// if ($acct_id) {
//   if (!isset($rep_detail[$acct_id])) {
//     $rep_detail[$acct_id] = _acct_rep_load($acct_id);
//   }
//   return isset($rep_detail[$acct_id]) ? $rep_detail[$acct_id] : 0 ;
// }
// return 0;
//}

/**
 * function which return account rep detail
 * @param integer $acct_id account ID of the customer
 * @return object TM account rep data
 */
function acct_rep_load($acct_id = NULL) {
  if ($acct_id) {
    $object = cache_get('account_rep_' . $acct_id, 'cache_custom');
    if (!$object) {
      $data = _tmapi_get_acct_rep($acct_id);
      return (object)$data;
    }
    return (object)$object->data;
  }
  return 0;
}

// Helper for acct_rep_load
function _acct_rep_load($acct_id= NUlL) {
  //$object = cache_get('rep_detail-' . $acct_id, 'cache_custom');
  //get products if cache is not set.
  //if (!$object) {
    $query = db_select('tbl_user_acct_rep', 'ar');
    $rep_detail = $query->fields('ar')
     ->condition('ar.acct_id', $acct_id, '=')
     ->execute()->fetchObject();
     
    //cache_set('rep_detail-' . $acct_id, $rep_detail, 'cache_custom');  
  //}
  /*else {
    $rep_detail = $object->data;
  }*/
  return $rep_detail;
}

/**
 * Function which returns user info
 */
/*
 * function get_user_info($acct_id = NULL) {
 if ($acct_id) {
   static $user_detail = NULL;
   if (!isset($user_detail[$acct_id])) {
    $user_detail[$acct_id]= _get_user_info($acct_id);
   }
   return isset($user_detail[$acct_id]) ? $user_detail[$acct_id] : 0;
 }
 return 0;
}
 */
function get_user_info($acct_id = NULL) {
  if ($acct_id) {
    $object = cache_get('user_info-' . $acct_id, 'cache_custom');
    if (!$object && (int) $acct_id) {
      $pass = isset($_SESSION['tmupa']) ? base64_decode($_SESSION['tmupa']) : '';
      $data = _tmapi_customer_query($acct_id, $pass);
      if ($data) {
        cache_set('user_info-' . $acct_id, $data, 'cache_custom');
      }
      return (object)$data;
    }
    return (object)$object->data;
  }
  return 0;
}

// Helper for get_user_info
function _get_user_info($acct_id = NULL) {
 $object = cache_get('user_info-' . $acct_id, 'cache_custom');
  //get products if cache is not set.
  if (!$object) {
    $user_detail = db_select('tm_user_details', 't')
      ->fields('t')
      ->condition('t.acct_id', $acct_id, '=')
      ->execute()->fetchObject();
      cache_set('user_info-' . $acct_id, $user_detail, 'cache_custom');
   }
   else {
     $user_detail = $object->data;
   }
   return $user_detail;
}

/*
// get seats hold by a customer
function get_seats_hold($acct_id = NULL) {
 if ($acct_id) {
  static $seat_detail = NULL;
   if (!isset($seat_detail[$acct_id])) {
    $seat_detail[$acct_id] =  _get_seats_hold($acct_id);
   }
   return isset($seat_detail[$acct_id]) ? $seat_detail[$acct_id] : array();
 }
 return array();
}
*/

/**
 * function to fetch seats holds by customer
 * @param integer $acct_id account id of the user
 * @return array TM response of invoice_detail API
 * 
 */
function get_seats_hold($acct_id = NULL) {
  if ($acct_id) {
    $object = cache_get('seat_hold-' . $acct_id, 'cache_custom');
    if (!$object) {
      $data = tmapi_get_sth_invoice($acct_id);
      if ($data) {
        cache_set('seat_hold-' . $acct_id, $data, 'cache_custom');
      }
      return $data;
    }
    return $object->data;
  }
  return 0;
}

// Helper for get_seats_hold
function _get_seats_hold($acct_id = NULL) {
  $object = cache_get('seat_hold-' . $acct_id, 'cache_custom');
  if (!$object) {
    $seat_detail =  db_select('tm_invoice_details', 't')
      ->fields('t')
      ->condition('t.acct_id', $acct_id, '=')
      ->orderBy('section_name', 'ASC')
      ->orderBy('row_name', 'ASC')
      ->execute()->fetchAll();
    cache_set('seat_hold-' . $acct_id, $seat_detail, 'cache_custom');
  }
  else {
    $seat_detail = $object->data;
  }
  return $seat_detail;
}


// get seats on an Account
//function get_seats_on_an_account($acct_id = NULL) {
// if ($acct_id) {
//  static $seat_detail = NULL;
//   if (!isset($seat_detail[$acct_id])) {
//    $seat_detail[$acct_id] =  _get_seats_on_account($acct_id);
//   }
//   return isset($seat_detail[$acct_id]) ? $seat_detail[$acct_id] : array();
// }
// return array();
//}


/**
 *get seats on an Account
 *@param $acct_id integer account ID of the customer
 *@return array() seat details
 */
function get_seats_on_an_account($acct_id = NULL) {
  if ($acct_id) {
    $object = cache_get('seat_on_account-' . $acct_id, 'cache_custom');
    if (!$object) {
      $data = _tmapi_get_seats_on_account($acct_id);
      if (count($data)) {
        cache_set('seat_on_account-' . $acct_id, $data, 'cache_custom');
      }
      return $data;
    }
    return $object->data;
  }
  return 0;
}

// Helper for get_seats_on_an_account
function _get_seats_on_account($acct_id = NULL) {
  $object = cache_get('seat_on_account-' . $acct_id, 'cache_custom');
  if (!$object) {
    $seat_detail =  db_select('tmapi_seats_hold', 't')
      ->fields('t')
      ->condition('t.acct_id', $acct_id, '=')
      ->orderBy('section_name', 'ASC')
      ->orderBy('row_name', 'ASC')
      ->execute()->fetchAll();
    cache_set('seat_on_account-' . $acct_id, $seat_detail, 'cache_custom');
  }
  else {
    $seat_detail = $object->data;
  }
  return $seat_detail;
}


/**
 * function to fetch seats holds by customer
 * @param integer $acct_id account id of the user
 * @return array TM response of invoice_detail API
 * 
 */
function get_invoice_list($acct_id = NULL) {
  if ($acct_id) {
    $object = cache_get('invoice_list-' . $acct_id, 'cache_custom');
    if (!$object) {
      $data = _tmapi_get_invoice_all_list($acct_id);
      if ($data) {
        cache_set('invoice_list-' . $acct_id, $data, 'cache_custom');
      }
      return $data;
    }
    return $object->data;
  }
  return 0;
}

/**
 * function to fetch seats holds by customer
 * @param integer $acct_id account id of the user
 * @return array TM response of invoice_detail API
 * 
 */
function get_invoice_details($acct_id = NULL, $invoice_id = NULL) {
  if ($acct_id) {
    $object = cache_get('invoice_detail-' . $acct_id . '-' . $invoice_id, 'cache_custom');
    if (!$object) {
      $data = tmapi_get_sth_invoice_using_InvoiceID($acct_id, $invoice_id);
      if ($data) {
        cache_set('invoice_detail-' . $acct_id . '-' . $invoice_id, $data, 'cache_custom');
      }
      return $data;
    }
    return $object->data;
  }
  return 0;
}

// get seats hold by a customer
function validate_user_acct_id($acct_id = NULL, $pin = NULL) {
 return (bool) db_select('tm_user_details', 't')
   ->fields('t', array('acct_id'))
   ->condition('t.acct_id', $acct_id, '=')
   ->condition('t.pin', $pin, '=')
   ->range(0, 1)
   ->execute()->fetchObject();
}

// get seats hold by a customer
function get_total_seats_hold($acct_id = NULL) {
 $seats_info = array(variable_get('sth_full_season', 'EMAG14FS'), variable_get('sth_chasesuites', 'CHSE1415'));
 if ($acct_id) {
   return db_query("SELECT SUM(t.num_seat) FROM {tm_invoice_details} t WHERE t.acct_id = :acct_id and t.event_name IN (:seat_info)", array(':acct_id' => $acct_id, ':seat_info' => $seats_info))->fetchField();
 }
 return array();
}
// get seats hold by a customer
function get_total_event_seats_hold($acct_id = NULL, $eventId = NULL) {
 if ($acct_id) {
   return db_query("SELECT SUM(t.num_seat) FROM {tm_invoice_details} t WHERE t.acct_id = :acct_id AND event_name = :event_name", array(':acct_id' => $acct_id, ':event_name' => $eventId))->fetchField();
 }
 return array();
}

// get playoff data hold by a customer
function get_playoff_data($acct_id = NULL) {
  if ($acct_id) {
    static $playoff_detail = NULL;
      if (!isset($playoff_detail[$acct_id])) {
        $playoff_detail[$acct_id] =  _get_playoff_data($acct_id);
      }
      return isset($playoff_detail[$acct_id]) ? $playoff_detail[$acct_id] : array();
    }
  return array();
}

// helper function for get_playoff_data
function _get_playoff_data($acct_id = NULL) {
  $object = cache_get('playoff_seats-' . $acct_id, 'cache_custom');
  if (!$object) {
    $seat_detail =  db_select('tm_playoff_data', 't')
     ->fields('t')
     ->condition('t.acct_id', $acct_id, '=')
     ->execute()->fetchAll();
    cache_set('playoff_seats-' . $acct_id, $seat_detail, 'cache_custom');
  }
  else {
    $seat_detail = $object->data;
  }
  return $seat_detail;
}

// get seats hold by a customer
function get_virtual_venue_data($section = NULL, $row = NULL, $seat = NULL) {
 if ($section && $row && $seat) {
   return db_select('tbl_section_data', 't')
     ->fields('t')
     ->condition('t.section_number', $section, '=')
     ->condition('t.row_number', $row, '=')
     ->condition('t.seat_number', $seat, '=')
     ->execute()->fetchAll();
 }
 return array();
}

/***
 * function get user tm account info
 */
function get_tm_user_amount_info($acct_id) {
  $seat_holds = get_seats_hold($acct_id);
  $amount_due['invoice_id'] = '';
  $amount_due['due_amt'] = 0;
  if (count($seat_holds)):
    $total = $paid = $parking_fee = $handling_fee = $resale_acc_credit = 0;
    foreach ($seat_holds as $seats) {
      $event_name = ($seats->item_name_long && !is_null($seats->item_name_long))? $seats->item_name_long : $seats->item_name;
      $event_name1 = strtolower($event_name);
      $pos = strpos($event_name1, 'parking');
      $h_fee = strpos($event_name1, 'handling');
      $invoice_id = (isset($invoice_id) && $invoice_id) ? $invoice_id : $seats->invoice_id;
      if ($pos!== false) {
        $parking_fee += $seats->invoiced_amount;
      }
      elseif($h_fee!==false) {
        $handling_fee += $seats->invoiced_amount;
      }
      else {
        $total += $seats->invoiced_amount;
      }
      $paid += $seats->paid_amount;
    }
    $amount_due['invoice_id'] = $invoice_id;
    $amount_due['due_amt'] = ($total + $parking_fee + $handling_fee) - $paid;
  endif;
  flog_it($amount_due);
  return $amount_due;
}

function get_tm_payment_flag($acct_id) {
  return (bool) db_select('tbl_tm_payment_flag')->fields('tbl_tm_payment_flag', array('flag'))->condition('acct_id', $acct_id)->range(0, 1)->execute()->fetchField();
}

/**
 * function to create/update TM user from API
 */
function tmapi_data_account_update($acct_id = NULL, $data = NULL) {

  $acc_grp = $data['account_groups'];
  foreach($acc_grp as $key=>$nk) 
       {
        if(in_array(19, $nk)) 
        {
         $roles[R_19] = TRUE;    
        }
         if(in_array(20, $nk)) 
        {
         $roles[R_20] = TRUE;    
        }
		 if(in_array(21, $nk)) 
        {
         $roles[R_21] = TRUE;    
        }
       }
       if((!$roles[R_19]))
       {
        $roles[NOT_R_19] = TRUE;
       }
     $users = db_select('users', 'u')
    ->fields('u', array('uid'))
    ->condition('u.name', $acct_id, '=')
    ->range(0, 1)->execute()->fetchObject();
   // print_r($nk);die();
 $roles[DRUPAL_STH_USER] = TRUE;
  // $roles = array(DRUPAL_STH_USER => TRUE);
  // we will create a new array for user
  if ((isset($users->uid) && $users->uid)) {
    $account = user_load($users->uid);
     $account->roles = $roles;
    user_save($account);
    if (isset($account->mail) && isset($data['email']) && $account->mail != $data['email'])
    $edit = array();
    $edit['mail'] = isset($data['email']) ? $data['email'] : $account->mail;
    user_save($account, $edit);
    watchdog('tm_api', 'User updated with account id: %name and email: %email', array('%name' => $account->name, '%email' => $account->mail), WATCHDOG_INFO);
  }
  else {
    $account = new stdClass;
    $account->is_new = 1;
    $pass = user_password();
    $new_user = array (
      'name' => $acct_id,
      'mail' => $data['email'],
      'pass' => $pass,
      'init' => $data['email'],
      'status' => 1,
      'access' => REQUEST_TIME,
      'roles' => $roles,
    );
    // $account returns user object
    $account = user_save($account, $new_user);
    watchdog('tm_api','User created with account id: %name', array('%name' => $account->name), WATCHDOG_INFO);
  }
 // print_r($account); die();
  // update tm_user_details data
  //tmapi_data_tm_user_details($acct_id, $data);
   cache_clear_all('user_info-' . $acct_id, 'cache_custom', $wildcard = FALSE);
  cache_set('user_info-' . $acct_id, $data, 'cache_custom');
  return $account;
}

/*
 * function to update the tm_user_details table
 */
function tmapi_data_tm_user_details($acct_id, $data) {
  if (!empty($data) && isset($data['acct_id'])) {
    if (isset($data['alt_id_array']) && count($data['alt_id_array'])) {
        unset($data['alt_id_array']);
    }
    
    if (!(bool) db_select('tm_user_details', 't')->fields('t', array('acct_id'))->condition('acct_id', $acct_id, '=')->range(0, 1)->execute()->fetchField()) {
      db_insert('tm_user_details')
        ->fields($data)
        ->execute();
    }
    else {
      db_update('tm_user_details')
       ->fields($data)
       ->condition('acct_id', $acct_id)
       ->execute();
    }
  }
  cache_clear_all('user_info-' . $acct_id, 'cache_custom', $wildcard = FALSE);
  watchdog('tm_api','TM user details has been updated for account id: %name', array('%name' => $acct_id), WATCHDOG_INFO);
}

/*
 * function to update the tm_invoice_details table
 */
function tmapi_data_tm_invoice_details($acct_id, $data = array()) {
  if (!empty($data)) {
    $num_deleted = db_delete('tm_invoice_details')
       ->condition('acct_id', $acct_id)
       ->execute();
   
    foreach ($data as $record) {
        $query = db_insert('tm_invoice_details')->fields($record)->execute();
      //$query->values($record);
    }
    //$query->execute();
    cache_clear_all('seat_hold-' . $acct_id, 'cache_custom');
    watchdog('tm_api','TM Invoice details has been updated for account id: %name', array('%name' => $acct_id), WATCHDOG_INFO);
  }
}
function tm_update_tmapi_seats_hold($acct_id, $data = array()) {
  if (!empty($data)) {
    $num_deleted = db_delete('tmapi_seats_hold')
       ->condition('acct_id', $acct_id)
       ->execute();
   
    foreach ($data as $record) {
        $record['acct_id'] = $acct_id;
        $query = db_insert('tmapi_seats_hold')->fields($record)->execute();
    }
    cache_clear_all('tmapi_seat_hold-' . $acct_id, 'cache_custom');
    watchdog('tm_api','TM get seats on an account has been updated for account id: %name', array('%name' => $acct_id), WATCHDOG_INFO);
  }
}
/**
 * function to update acct_re information
 */
function tm_update_acct_rep($rep_id, $acct_id) {
  if ($rep_id) {
    db_merge('tbl_user_acct_rep')
     ->key(array('acct_id' => $acct_id))
     ->fields(array(
       'acct_rep_num ' => $rep_id
     ))
   ->execute();
  }
}

function tmapi_get_rsvp_access() {
  module_load_include('inc', 'tm_api', 'tm_api.helper');
  
  global $user;
  $acct_id = isset($user->name) ? $user->name : 0;
  $seat_holds = 0;
  if (isset($acct_id) && $acct_id) {
    $object = cache_get('tmapi_seat_hold-' . $acct_id, 'cache_custom');
    if (!$object) {
      $price_code = array('AER', 'AR', 'AN', 'BON', 'BOR', 'BLN', 'BLR', 'CBLR', 'CBLN');
      $seat_holds = (bool) db_select('tmapi_seats_hold')
      ->fields('tmapi_seats_hold', array('price_code'))
      ->condition('acct_id', $acct_id)
      ->condition('price_code', $price_code, 'IN')
      ->condition('plan_event_name', '14SEAS%', 'LIKE')
      ->range(0, 1)->execute()->fetchField();
      $plan_name = 0;
     // $data = _tmapi_get_sth_status_rsvp($acct_id = NULL);
      flog_it( $acct_id . ' - ' . $_SESSION['plan_name11']);
      if (isset($_SESSION['plan_name11'])) {
        $array = array('14SEAS','14SEAS1','14SEAS2','14SEAS3','14SEAS4','14SEAS5','14SEAS6','14SEAS7','14SEAS8','14SEAS9');
        if (in_array($_SESSION['plan_name11'], $array)) {
            $plan_name =1;
           // flog_it($plan_name);
        }
      }
      $seat_holds = ($seat_holds && $plan_name)?1:0;
      cache_set('tmapi_seat_hold-' . $acct_id, $seat_holds, 'cache_custom');
    }
    else {
      return $object->data;
    }
  }
  return $seat_holds;
}


function tmapi_get_rsvp_submission_result() {
  global $user;
  $acct_id = $user->uid;
  if (isset($acct_id) && $acct_id) {
    return (bool) db_select('tmapi_seats_hold')->fields('tmapi_seats_hold', array('price_code'))->condition('acct_id', $acct_id)->condition('price_code', $price_code, 'IN')->range(0, 1)->execute()->fetchField();
  }
  return 0;
}

