<?php
// @file

function custom_ticketmaster_import_customer($form, &$form_state) {
  $form = array();
  // Cause return to beginning if we just completed an import
  if (isset($form_state['storage']['step']) && $form_state['storage']['step'] >= 3) {
    unset($form_state['storage']);
  }

  $step = empty($form_state['storage']['step']) ? 1 : $form_state['storage']['step'];
  $form_state['storage']['step'] = $step;

  switch ($step) {
    case 1:
      $tables = array(
        '' => '-- NONE--',
        'tm_user_details' => 'TicketMaster Customer Details',
        'tm_seats_hold' => 'Ticketmaster Seats Hold',
        'tbl_user_acct_rep' => 'Customer\'s Account Representatives',
      );

      $form['notes'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="import-notes">A few notes when uploading. <ul><li>Make sure the file is in a .csv format (comma delimited).</li><li>Be sure to click the "Upload" button when you select a csv.</li></ul></div>', '#upload_location' => 'public://tmp/',
      );

      $form['table'] = array(
        '#type' => 'select',
        '#title' => t('Table'),
        '#description' => t('Please select a table from the list to import data.'),
        '#options' => $tables,
        '#required' => TRUE
      );

      $form['import'] = array(
        '#title' => t('Import'),
        '#type' => 'managed_file',
        '#description' => t('The uploaded csv will be imported and temporarily saved.'),
        '#upload_location' => 'public://tmp/',
        '#upload_validators' => array('file_validate_extensions' => array('csv'), ),
      );

      $form['field_delimiter'] = array(
        '#type' => 'select',
        '#title' => t('Field delimiter'),
        '#default_value' => ',',
        '#options' => sports_field_delimiters(),
        '#description' => t('Select field delimiter. Comma is typical for CSV export files.'),
      );
      $form['next'] = array(
        '#type' => 'submit',
        '#value' => t('Next')
      );

      // Set form parameters so we can accept file uploads.
      $form['#attributes'] = array('enctype' => 'multipart/form-data');
      break;

    case 2:
      $form['skip_row'] = array(
        '#type' => 'checkbox',
        '#title' => 'Exclude First Row',
        '#description' => 'Check this checkbox if first row includes column headings',
        '#default_value' => 1
      );

      $form['update_existing'] = array(
        '#type' => 'checkbox',
        '#title' => 'Update Existing Records',
        '#description' => 'Check this checkbox if you need to update existing records.',
        '#default_value' => 1
      );
      $form['truncate_existing'] = array(
        '#type' => 'checkbox',
        '#title' => 'Delete All Existing Records',
        '#description' => 'Check this checkbox if you need to delete all existing records.',
      );
      $form['instructions'] = array(
        '#markup' => t('<ul><li>Please map the database fields with the Uploaded file fields.</li><li>Label is database field name, and select list showing list of options from excel imported.</li></ul>'),
        '#prefix' => '<div id="uif_form_instructions">',
        '#suffix' => '</div>',
      );

      $schema = drupal_get_schema($form_state['storage']['table']);
      $options1 = array('' => '--<none>--');
      $options2 = $form_state['storage']['data'][0];
      $options = array_merge($options1, $options2);
      $primary_key = isset($schema['primary key'][0]) ? $schema['primary key'][0] : NULL;
      $table_variable = variable_get($form_state['storage']['table'], '');
      foreach ($schema['fields'] as $key => $field) {
        $type = '';
        if (isset($field['type'])) {
          $type = ' (' . $field['type'] . ', ' . (isset($field['length']) ? $field['length'] : '') . ')';
        }
        elseif (isset($field['mysql_type'])) {
          $type = ' (' . $field['mysql_type'] . ')';;
        }

        $form[$key] = array(
          '#type' => 'select',
          '#options' => $options,
          '#title' => $key . $type,
          '#default_value' => isset($table_variable[$key]) ? $table_variable[$key] : '',
        );
      }

      $form['primary_key'] = array(
        '#type' => 'value',
        '#value' => $primary_key
      );

      $form['back'] = array(
        '#type' => 'submit',
        '#value' => t('Back'),
        '#submit' => array('custom_ticketmaster_import_back'),
      );
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Import'),
      );
      break;
    }
  return $form;
}

function custom_ticketmaster_import_customer_submit($form, &$form_state) {
  $step = empty($form_state['storage']['step']) ? 1 : $form_state['storage']['step'];
  if (1 == $step) {
    $form_state['storage']['data'] = custom_ticketmaster_get_csv_field_row($form_state);
    $form_state['storage']['table'] = $form_state['values']['table'];
    $form_state['storage']['field_delimiter'] = $form_state['values']['field_delimiter'];
    $form_state['rebuild'] = TRUE;
  }

  if (2 == $step) {
    // Check to make sure that the file was uploaded to the server properly
    $data_arr = $form_state['storage']['data'];
    $fields = $form_state['values'];
    unset($fields['back']);
    unset($fields['submit']);
    unset($fields['form_build_id']);
    unset($fields['form_token']);
    unset($fields['form_id']);
    unset($fields['op']);
    $table = $form_state['storage']['table'];
    $primary_key = $fields['primary_key'];
    $skip_row = $fields['skip_row'];
    $update_existing = $fields['update_existing'];
    $truncate_existing = $fields['truncate_existing'];
    unset($fields['primary_key']);
    unset($fields['skip_row']);
    unset($fields['update_existing']);
    unset($fields['truncate_existing']);
    // setting a variable for next import
    variable_set($table, $fields);
    if ($truncate_existing) {
      db_truncate($table)->execute();
    }
    // Go through each row in the csv and run a function on it.
    if (count($data_arr) > 1) {
      foreach ($data_arr as $key => $data) {
        $row = array();
        if ($key == 0 && $skip_row) {
          continue;
        }
        foreach ($fields as $field => $id) {
          $row[$field] = isset($data[$id]) ? check_plain($data[$id]) : NULL;
        }
        $operations[] = array(
          'custom_ticketmaster_import_customer_batch_processing',  // The function to run on each row
          array($row, $table, $primary_key, $update_existing),
        );
      }
      // Once everything is gathered and ready to be processed... well... process it!
      $batch = array(
        'title' => t('Importing CSV...'),
        'operations' => $operations,  // Runs all of the queued processes from the while loop above.
        'finished' => 'custom_ticketmaster_import_customer_finished', // Function to run when the import is successful
        'error_message' => t('The import has encountered an error.'),
        'progress_message' => t('Imported @current of @total records.'),
        'file' => drupal_get_path('module', 'custom_ticketmaster') . '/custom_ticketmaster.import.inc',
      );
      batch_set($batch);
      batch_process('admin/ticketmaster/import');
    }
  }
  $form_state['storage']['step'] = $step + 1;
}

/**
 * This function runs the batch processing and creates records with then given information
 * @see custom_ticketmaster_import_customer_submit(),
 */
function custom_ticketmaster_import_customer_batch_processing($row, $table, $primary_key, $update_existing) {
  if ($table == 'tm_seats_hold') {
    if ($row['seat_increment'] == '' && $row['seat_num']) {
      $row['seat_increment'] = (($row['last_seat'] - $row['seat_num']) +1)/$row['num_seats'];
    }
    $row['purchase_price'] = get_purchase_price_number($row['purchase_price']);
    $row['block_purchase_price'] = get_purchase_price_number($row['block_purchase_price']);
    $row['paid_amount'] = get_purchase_price_number($row['paid_amount']);
    $row['acct_credit'] = get_purchase_price_number($row['acct_credit']);
    $row['total_amt_due'] = get_purchase_price_number($row['total_amt_due']);
    $row['shipping_fee'] = get_purchase_price_number($row['shipping_fee']);
  }
  
  if ($table == 'tm_playoff_data') {
    $row['p1_purchase_price'] = get_purchase_price_number($row['p1_purchase_price']);
    $row['p1_total_price'] = get_purchase_price_number($row['p1_total_price']);
    $row['p1_paid_amount'] = get_purchase_price_number($row['p1_paid_amount']);
    
    $row['p2_purchase_price'] = get_purchase_price_number($row['p2_purchase_price']);
    $row['p2_total_price'] = get_purchase_price_number($row['p2_total_price']);
    $row['p2_paid_amount'] = get_purchase_price_number($row['p2_paid_amount']);
    
    $row['p3_purchase_price'] = get_purchase_price_number($row['p3_purchase_price']);
    $row['p3_total_price'] = get_purchase_price_number($row['p3_total_price']);
    $row['p3_paid_amount'] = get_purchase_price_number($row['p3_paid_amount']);
    
    $row['p4_purchase_price'] = get_purchase_price_number($row['p4_purchase_price']);
    $row['p4_total_price'] = get_purchase_price_number($row['p4_total_price']);
    $row['p4_paid_amount'] = get_purchase_price_number($row['p4_paid_amount']);
    
    $row['paid_amount'] = get_purchase_price_number($row['paid_amount']);
    $row['acct_credit'] = get_purchase_price_number($row['acct_credit']);
    $row['total_amt_due'] = get_purchase_price_number($row['total_amt_due']);
    $row['shipping_fee'] = get_purchase_price_number($row['shipping_fee']);
  }
  
  if ($table == 'tbl_user_acct_rep') {
    if ($row['acct_rep_num'] == '' ) {
      $row['acct_rep_num'] = 0;
    }
  }
  if (isset($primary_key)) {
    if (!(bool) db_select($table, 't')->fields('t', array($primary_key))->condition($primary_key, $row[$primary_key], '=')->range(0, 1)->execute()->fetchField()) {
        db_insert($table)
          ->fields($row)
          ->execute();
    }
    elseif ($update_existing) {
      db_update($table)
        ->fields($row)
	      ->condition($primary_key, $row[$primary_key])
        ->execute();
    }
  }
  else {
    db_insert($table)
      ->fields($row)
      ->execute();
  }
  if ($table == 'tm_user_details') {
    custom_ticketmaster_register_imported_customer($row, $update_existing);
  }
}

/**
 * This function runs when the batch processing is complete
 *
 * @see custom_ticketmaster_import_customer_submit(),
 */
function custom_ticketmaster_import_customer_finished() {
  watchdog('custom_ticketmaster', 'CSV data imported successfully', array(), WATCHDOG_INFO);
  return drupal_set_message(t('Data Import Completed Successfully'));
}

/**
 * Return user to starting point on template multi-form.
 */
function custom_ticketmaster_import_back($form, &$form_state) {
  $form_state['storage']['step'] = 1;
}

function custom_ticketmaster_get_csv_field_row(&$form_state) {
  $rows = array();
  // Check to make sure that the file was uploaded to the server properly
  $uri = db_query("SELECT uri FROM {file_managed} WHERE fid = :fid", array(
    ':fid' => $form_state['values']['import'],
  ))->fetchField();
  if (!empty($uri)) {
    if (file_exists(drupal_realpath($uri))) {
      // Open the csv
      $handle = fopen(drupal_realpath($uri), "r");
      // Go through each row in the csv and run a function on it. In this case we are parsing by '|' (pipe) characters.
      // If you want commas are any other character, replace the pipe with it.
      $rows = array();
      while (($data = fgetcsv($handle, 0, $form_state['values']['field_delimiter'], '"')) !== FALSE) {
        $rows[] = $data;  // The row in the csv
      }
      fclose($handle);
    }
  }
  return $rows;
}

/**
 * Field delimiter options.
 */
function sports_field_delimiters() {
  return array(
    ',' => ',',
    ';' => ';',
    '|' => '|',
  );
}

/**
 * function to create account representative node
 */
function custom_ticketmaster_register_imported_customer($row, $update_existing = 0) {
  $users = db_select('users', 'u')
    ->fields('u', array('uid'))
    ->condition('u.name', $row['acct_id'], '=')
    ->range(0, 1)->execute()->fetchObject();
  $roles = array(DRUPAL_STH_USER => TRUE);
    // we will create a new array for user
  if ((isset($users->uid) && $users->uid)) {
    if ($update_existing == 1) {
      $user_load = user_load($users->uid);
      $edit = array();
      $edit['pass'] = $row['pin'];
      $edit['roles'] = $roles;
      $edit['mail'] = isset($row['email']) ? $row['email'] : $user_load->mail;
      user_save($user_load, $edit);
    }
    watchdog('custom_ticketmaster', 'User updated with account id: %name and email: %email', array('%name' => $user_load->name, '%email' => $user_load->mail), WATCHDOG_INFO);
  }
  else {
    $account = new stdClass;
    $pass = (isset($row['pin']) && $row['pin']) ? $row['pin'] : $row['acct_id'];
    $new_user = array (
      'name' => $row['acct_id'],
      'mail' => $row['email'],
      'pass' => $pass,
      'init' => $row['email'],
      'status' => 1,
      'access' => REQUEST_TIME,
      'roles' => $roles,
    );
    // $account returns user object
    $account = user_save($account->is_new, $new_user);
    watchdog('custom_ticketmaster','User created with account id: %name', array('%name' => $account->name), WATCHDOG_INFO);
  }
}


/**
 * function to delete Ticketmaster customer data
 */
function custom_ticketmaster_clean_data() {
  $form = array();
  return confirm_form($form,
    t('Are you sure you want to delete all TM user accounts and there data?'),
    '<front>', t('This action cannot be undone.'),
    t('Delete All Data'), t('Cancel'));
}
function custom_ticketmaster_clean_data_submit() {
  $query = db_query("
    SELECT ur.uid, u.name FROM {users_roles} ur
    INNER JOIN {users} u ON u.uid = ur.uid
    WHERE ur.rid = :rid AND ur.rid NOT IN(3,6) AND u.uid != 1", array(':rid' => 4))->fetchAll();
  foreach ($query as $record) {
    $operations[] = array(
      'custom_ticketmaster_delete_user_data',  // The function to run on each row
      array($record),
    );
  }
  $batch = array(
    'title' => t('Deleting TM Users...'),
    'operations' => $operations,  // Runs all of the queued processes from the while loop above.
    'finished' => 'custom_ticketmaster_delete_user_data_finished', // Function to run when the import is successful
    'error_message' => t('The import has encountered an error.'),
    'progress_message' => t('Deleted @current of @total records.'),
    'file' => drupal_get_path('module', 'custom_ticketmaster') . '/custom_ticketmaster.import.inc',
  );
  batch_set($batch);
  batch_process('admin/ticketmaster/clean-data');
  
}

function custom_ticketmaster_delete_user_data($record) {
  user_delete($record->uid);
  db_delete('tm_seats_hold')->condition('acct_id', $record->name)->execute();
  db_delete('tm_user_details')->condition('acct_id', $record->name)->execute();
  //db_delete('tbl_promote_tickets')->condition('acct_id', $record->name)->execute();
  db_delete('tbl_rsvp_event')->condition('uid', $record->uid)->execute();
  db_delete('tbl_user_acct_rep')->condition('acct_id', $record->name)->execute();
  //db_delete('tm_playoff_data')->condition('acct_id', $record->name)->execute();
}

/**
 * 
 */
function custom_ticketmaster_delete_user_data_finished() {
  watchdog('custom_ticketmaster', 'TM Users and Data deleted successfully', array(), WATCHDOG_INFO);
  return drupal_set_message(t('Data Deletion Completed Successfully'));
}