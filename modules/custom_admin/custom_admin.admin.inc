<?php

/**
 * @FILE
 *
 */
module_load_include('inc', 'system', 'system.admin');

/* * *
 * General settings form
 */

function custom_admin_general_settings() {
  $form = array();
  $form['subsite_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Site Name'),
    '#description' => t('Please enter site full name for global usage'),
    '#default_value' => variable_get('subsite_name', t('Kings')),
    '#required' => TRUE,
  );
  $form['stp_league'] = array(
    '#type' => 'select',
    '#title' => t('League'),
    '#options' => array('NBA' => 'NBA', 'NFL' => 'NFL', 'NHL' => 'NHL', 'WNBA' => 'WNBA', 'MLB' => 'MLB'),
    '#description' => t('Please select the league e.g. NFL, NHL, NBA etc'),
    '#default_value' => variable_get('stp_league', t('NBA')),
    '#required' => TRUE,
  );
  $form['stp_logo_cloud_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Cloud Front Url: League Images'),
    '#description' => t('Please enter the path for Cloud front url.'),
    '#default_value' => variable_get('stp_logo_cloud_url', 'https://d1w358dfdqgxb2.cloudfront.net/stp'),
    '#required' => TRUE,
  );
  $form['home_team'] = array(
    '#type' => 'textfield',
    '#title' => t('Team Name'),
    '#description' => t('Please enter season home team abribiation for global usage .e.g ABC'),
    '#default_value' => variable_get('home_team', t('COW')),
    '#required' => TRUE,
  );
  $form['home_ground'] = array(
    '#type' => 'textfield',
    '#title' => t('Home Ground'),
    '#description' => t('Please enter Team Home-Ground name'),
    '#default_value' => variable_get('home_ground', t('AT&T Stadium')),
    '#required' => TRUE,
  );
  $form['tm_purchase_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Purchase Ticket Url'),
    '#default_value' => variable_get('tm_purchase_url', 'https://oss.ticketmaster.com/html/home.htmI?team=cowboys&l=EN&STAGE=1&CNTX=120962&out=1'),
    '#required' => TRUE,
  );
  $form['forgot_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Forgot Password Link'),
    '#default_value' => variable_get('forgot_link', 'https://oss.ticketmaster.com/html/home.htmI?team=cowboys&l=EN&STAGE=1&CNTX=120962&out=1'),
    '#required' => TRUE,
  );
  $form['ticketmaster_run_cron'] = array(
    '#type' => 'checkbox',
    '#title' => t('Run game Feed Import cron?'),
    '#default_value' => variable_get('ticketmaster_run_cron', 1),
  );
  $form['ticketmaster_cron_interval'] = array(
    '#type' => 'textfield',
    '#title' => t('Ticketmaster Cron Interval (in sec)'),
    '#default_value' => variable_get('ticketmaster_cron_interval', 24 * 60 * 60),
    '#required' => TRUE,
  );
  $form['stp_print_api'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Invoice Print APIs?'),
    '#default_value' => variable_get('stp_print_api', 0),
  );
  return system_settings_form($form);
}

/**
 * checkbox enable/disable
 * date renew
 * three types of block content 
 *
 */
function custom_admin_renew_settings() {
  $form['ticket_renew_enable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Ticket Renew block Enable'),
//  '#description' => t('Please enter message text for ticket promotion note appeared on Promtoion Page.'),
      '#default_value' => variable_get('ticket_renew_enable', 0),
  );
  $form['ticket_renew_date'] = array(
      '#type' => 'date',
      '#title' => t('Last Date of Ticket Renew'),
      '#default_value' => variable_get('ticket_renew_date', array('year' => date('Y'), 'month' => date('n'), 'day' => date('d') - 1)),
      '#description' => t('Last Date of Ticket Renew'),
      '#required' => TRUE,
  );
  $form['ticket_renew_link'] = array(
      '#type' => 'textfield',
      '#title' => t('Ticket Renew Link'),
      '#default_value' => variable_get('ticket_renew_link', 'https://oss.ticketmaster.com/html/home.htmI?team=cowboys&l=EN&STAGE=1&CNTX=120962&out=1'),
      '#description' => t('Last Date of Ticket Renew'),
      '#required' => TRUE,
      '#size' => 120,
  );
  $form['ticket_renew_block1'] = array(
      '#type' => 'textarea',
      '#title' => t('Ticket Renew block 1 Message'),
      '#rows' => 10,
      '#description' => t('Ticket Renew block 1 Message, tokens:- [ticket_renew:ticket-renew-days-left], [ticket_renew:ticket-renew-link] and [ticket_renew:ticket-renew-days-only]'),
      '#default_value' => variable_get('ticket_renew_block1', '[ticket_renew:ticket-renew-days-left] left to renew your season ticket [ticket_renew:ticket-renew-link]'),
      '#required' => TRUE,
  );
  $form['ticket_renew_block2'] = array(
      '#type' => 'textarea',
      '#title' => t('Ticket Renew block 2 Message'),
      '#rows' => 10,
      '#description' => t('Ticket Renew block 2 Message, tokens:- [ticket_renew:ticket-renew-days-left], [ticket_renew:ticket-renew-link] and [ticket_renew:ticket-renew-days-only]'),
      '#default_value' => variable_get('ticket_renew_block2', theme('renew_banner_box_template')),
      '#required' => TRUE,
  );
  $form['ticket_renew_block3'] = array(
      '#type' => 'textarea',
      '#title' => t('Ticket Renew block 3 Message'),
      '#rows' => 10,
      '#description' => t('Ticket Renew block 3 Message, tokens:- [ticket_renew:ticket-renew-days-left], [ticket_renew:ticket-renew-link] and [ticket_renew:ticket-renew-days-only]'),
      '#default_value' => variable_get('ticket_renew_block3', theme('renew_content_box_template')),
      '#required' => TRUE,
  );
  $form['ticket_renew_block4'] = array(//homepage
      '#type' => 'textarea',
      '#title' => t('Ticket Renew block 4 Message'),
      '#rows' => 10,
      '#description' => t('Ticket Renew block 4 Message, tokens:- [ticket_renew:ticket-renew-days-left], [ticket_renew:ticket-renew-link] and [ticket_renew:ticket-renew-days-only]'),
      '#default_value' => variable_get('ticket_renew_block4', theme('renew_content_box_template')),
      '#required' => TRUE,
  );
  return system_settings_form($form);
}

/**
 * checkbox enable/disable
 * Playoff
 * three types of block content 
 *
 */
function custom_admin_playoff_settings() {
  $form['playoff_enable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Ticket Playoff block Enable'),
//  '#description' => t('Please enter message text for ticket promotion note appeared on Promtoion Page.'),
      '#default_value' => variable_get('playoff_enable', 0),
  );
  $form['playoff_date'] = array(
      '#type' => 'date',
      '#title' => t('Date of Playoff'),
      '#default_value' => variable_get('playoff_date', array('year' => date('Y'), 'month' => date('n'), 'day' => date('d') - 1)),
      '#description' => t('Date of Playoff'),
      '#required' => TRUE,
  );
  $form['playoff_link'] = array(
      '#type' => 'textfield',
      '#title' => t('Playoff Link'),
      '#default_value' => variable_get('playoff_link', 'https://oss.ticketmaster.com/html/home.htmI?team=cowboys&l=EN&STAGE=1&CNTX=120962&out=1'),
      '#description' => t('Link for Playoff'),
      '#required' => TRUE,
      '#size' => 120,
  );
  $form['playoff_block1'] = array(
      '#type' => 'textarea',
      '#title' => t('Playoff block 1 Message'),
      '#rows' => 10,
      '#description' => t('Playoff block 1 Message, tokens:- [ticket-playoff:playoff-link], [ticket_playoff:playoff-date]'),
      '#default_value' => variable_get('playoff_block1', '<span>Secure your Playoff tickets by <strong>[ticket_playoff:playoff-date]</strong></span> [ticket_playoff:playoff-link]'),
      '#required' => TRUE,
  );
  $form['playoff_block2'] = array(
      '#type' => 'textarea',
      '#title' => t('Playoff block 2 Message'),
      '#rows' => 10,
      '#description' => t('Playoff block 2 Message, tokens:- [ticket-playoff:playoff-link], [ticket_playoff:playoff-date]'),
      '#default_value' => variable_get('playoff_block2', theme('playoff_banner_box_template')),
      '#required' => TRUE,
  );
  $form['playoff_block3'] = array(
      '#type' => 'textarea',
      '#title' => t('Playoff block 3 Message'),
      '#rows' => 10,
      '#description' => t('Playoff block 3 Message, tokens:- [ticket-playoff:playoff-link], [ticket_playoff:playoff-date]'),
      '#default_value' => variable_get('playoff_block3', theme('playoff_content_box_template')),
      '#required' => TRUE,
  );
   $form['stp_browser_support'] = array(
    '#type' => 'textfield',
    '#title' => t('Browser Support IE'),
    '#default_value' => variable_get('stp_browser_support', '9.0'),
    '#description' => t('Please enter suported IE version e.g. 9.0'),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}

function custom_admin_invoice_setting() {
  $form['invoice_mail_subject'] = array(
      '#type' => 'textfield',
      '#title' => t('Invoice Body Subject'),
      '#description' => t('Please enter subject for invoice mail.'),
      '#default_value' => variable_get('invoice_mail_subject', t('2013-14 Dallas Cowboys Invoice')),
      '#required' => TRUE,
  );
  $form['invoice_mail_body'] = array(
      '#type' => 'textarea',
      '#title' => t('Invoice Body Mail'),
      '#rows' => 5,
      '#description' => t('Please enter message text for invoice_mail_body.'),
      '#default_value' => variable_get('invoice_mail_body', t('Attached to this email is a copy of your invoice for the 2015-16 Sacramento Kings Season Subscription.')),
      '#required' => TRUE,
  );
  return system_settings_form($form);
}

/*
 * form: Renewal Invoice settings
 */

function custom_admin_invoice_payment_setting() {
  $form = array();
  $form['box6'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('stp-legend-heading')),
  );
  $form['box6']['payment_plans'] = array(
    '#type' => 'textfield',
    '#title' => t('Payment Plans'),
    '#description' => t('Please enter payment plans. Use comma separated values for multiple Plans.'),
    '#default_value' => variable_get('payment_plans', ''),    
  );
   $form['invoice_payment_subject'] = array(
      '#type' => 'textfield',
      '#title' => t('Invoice Payment Mail Subject'),
      '#description' => t('Please enter subject for invoice payment mail.'),
      '#default_value' => variable_get('invoice_payment_subject', t('Thanks For Your Payment')),
      '#required' => TRUE,
  );
  $form['invoice_payment_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Invoice Body Mail'),
    '#rows' => 5,
    '#description' => t('Please enter message text for successful payment mail.'),
    '#default_value' => variable_get('invoice_payment_body', 'Thanks for your payment.<br/> $ %amount was charged to your card ending in %cc_num<br/>Get ready for a great 2014 season! Go Cowboys <br/>Your account manager will reflect payment within 24 hours.  We are excited for another season in AT&T Stadium and appreciate your support as a loyal Dallas Cowboys season ticket holder.  You will receive your tickets through UPS before the first preseason game. Please contact your service coordinator if you have any questions.'),
    '#required' => TRUE,
  );
  $form['invoice_payment_notify_mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Invoice Payment Admin Recipient Email address'),
    '#default_value' => variable_get('invoice_payment_notify_mail', ''),
  );
  $form['invoice_payment_notify_subject'] = array(
      '#type' => 'textfield',
      '#title' => t('Invoice Payment Admin Mail Subject'),
      '#description' => t('Please enter subject for invoice payment mail.'),
      '#default_value' => variable_get('invoice_payment_notify_subject', t('Payment has been received')),
  );
  $form['invoice_payment_notify_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Invoice Payment Admin Body Mail'),
    '#rows' => 5,
    '#description' => t('Please enter message text for successful payment notification to admin. Tokens are %amount, %cc_num'),
    '#default_value' => variable_get('invoice_payment_notify_body', 'Payment has been received.<br/> $ %amount was charged from card ending in %cc_num<br/>'),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}

/**
 * Settings form for Ticketmaster Login url
 */
function custom_admin_tm_signon_settings() {
  $form = array();
  $form['tm_single_sign_on'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable TM Single Sign on to this website'),
    '#description' => t('Please clear cache after enabling/disabling this option.'),
    '#default_value' => variable_get('tm_single_sign_on', 0),
  );
  $form['tm_api_test'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable TM TEST API(Payment Request)'),
    '#description' => t('Please clear cache after enabling/disabling this option.'),
    '#default_value' => variable_get('tm_api_test', 0),
  );
  // Live TM API
  $form['tmapi_live'] = array(
    '#type' => 'fieldset',
    '#title' => t('Live TM API Parameters'),
    '#weight' => 4,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['tmapi_live']['tm_login_url'] = array(
    '#type' => 'textfield',
    '#title' => t('TM Login Url'),
    '#description' => t('Ticketmaster customer login url'),
    '#default_value' => variable_get('tm_login_url', 'https://ws.ticketmaster.com/archtics/ats/ticketing_services.aspx?dsn=cowboys'),
    '#required' => TRUE,
    '#size' => 100,
  );
  $form['tmapi_live']['tm_cert_type'] = array(
    '#type' => 'select',
    '#title' => t('TM Certificate Type'),
    '#description' => t('Ticketmaster Certificate Type Windows/Linux'),
    '#default_value' => variable_get('tm_cert_type', 'Linux'),
    '#options' => array('Windows', 'Linux'),
    '#required' => TRUE,
  );
  $form['tmapi_live']['header'] = array(
    '#type' => 'fieldset',
    '#title' => t('Header Parameters'),
    '#weight' => 4,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['tmapi_live']['header']['tm_ver'] = array(
    '#type' => 'textfield',
    '#title' => t('TM JSON API Version'),
    '#description' => t('Ticketmaster JSON API Version.'),
    '#default_value' => variable_get('tm_ver', '0.9'),
    '#required' => TRUE,
  );
  $form['tmapi_live']['header']['tm_src_sys_type'] = array(
    '#type' => 'textfield',
    '#title' => t('TM Source system type.'),
    '#description' => t('Source system type.  Identifies the calling system.  Always 2.'),
    '#default_value' => variable_get('tm_src_sys_type', '2'),
    '#required' => TRUE,
  );
  $form['tmapi_live']['header']['tm_src_sys_name'] = array(
    '#type' => 'textfield',
    '#title' => t('TM: Name of the source system'),
    '#description' => t('Name of the source system. e.g. '),
    '#default_value' => variable_get('tm_src_sys_name', 'IOMEDIA'),
    '#required' => TRUE,
  );
  $form['tmapi_live']['header']['tm_archtics_version'] = array(
    '#type' => 'textfield',
    '#title' => t('Archtics DB version'),
    '#description' => t('Ticketmaster Archtics DB version.  The individual commands may be different from version to version. e.g. V605'),
    '#default_value' => variable_get('tm_archtics_version', 'V605'),
    '#required' => TRUE,
  );
  $form['tmapi_live']['command'] = array(
    '#type' => 'fieldset',
    '#title' => t('Command1 Parameter'),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['tmapi_live']['command']['tm_cmd'] = array(
    '#type' => 'textfield',
    '#title' => t('Ticketmaster Command'),
    '#description' => t('Always "customer_query"'),
    '#default_value' => variable_get('tm_cmd', 'customer_query'),
    '#required' => TRUE,
  );
  $form['tmapi_live']['command']['tm_ref'] = array(
    '#type' => 'textfield',
    '#title' => t('TM Reference attribute'),
    '#description' => t('Reference attribute; echoed to result.'),
    '#default_value' => variable_get('tm_ref', 'IOM_CUSTOMER_QUERY'),
    '#required' => TRUE,
  );
  $form['tmapi_live']['command']['tm_uid'] = array(
    '#type' => 'textfield',
    '#title' => t('TM: User ID'),
    '#description' => t('User ID (used on connection to DB). Uid will be provided by TM'),
    '#default_value' => variable_get('tm_uid', 'iomed05'),
    '#required' => TRUE,
  );
  $form['tmapi_live']['command']['tm_dsn'] = array(
    '#type' => 'textfield',
    '#title' => t('TM Data Source Name'),
    '#description' => t('Data Source Name (e.g. DB) to connect to'),
    '#default_value' => variable_get('tm_dsn', 'magic'),
    '#required' => TRUE,
  );
  $form['tmapi_live']['command']['tm_site_name'] = array(
    '#type' => 'textfield',
    '#title' => t('TM web-site name'),
    '#description' => t('Identifies the web-site the consumer is logging in from'),
    '#default_value' => variable_get('tm_site_name', 'mytruebluenation'),
    '#required' => TRUE,
  );
  
  // TEST TM API
  $form['tmapi_test'] = array(
    '#type' => 'fieldset',
    '#title' => t('TEST TM API Parameters'),
    '#weight' => 6,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['tmapi_test']['tm_login_url_test'] = array(
    '#type' => 'textfield',
    '#title' => t('TM Login Url'),
    '#description' => t('Ticketmaster customer login url'),
    '#default_value' => variable_get('tm_login_url_test', 'https://ws.ticketmaster.com/archtics/ats/ticketing_services.aspx?dsn=cowboys'),
    '#required' => TRUE,
    '#size' => 100,
  );
  $form['tmapi_test']['header'] = array(
    '#type' => 'fieldset',
    '#title' => t('Header Parameters'),
    '#weight' => 7,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['tmapi_test']['header']['tm_ver_test'] = array(
    '#type' => 'textfield',
    '#title' => t('TM JSON API Version'),
    '#description' => t('Ticketmaster JSON API Version.'),
    '#default_value' => variable_get('tm_ver_test', '0.9'),
    '#required' => TRUE,
  );
  $form['tmapi_test']['header']['tm_src_sys_type_test'] = array(
    '#type' => 'textfield',
    '#title' => t('TM Source system type.'),
    '#description' => t('Source system type.  Identifies the calling system.  Always 2.'),
    '#default_value' => variable_get('tm_src_sys_type_test', '2'),
    '#required' => TRUE,
  );
  $form['tmapi_test']['header']['tm_src_sys_name_test'] = array(
    '#type' => 'textfield',
    '#title' => t('TM: Name of the source system'),
    '#description' => t('Name of the source system. e.g. '),
    '#default_value' => variable_get('tm_src_sys_name_test', 'IOMEDIA'),
    '#required' => TRUE,
  );
  $form['tmapi_test']['header']['tm_archtics_version_test'] = array(
    '#type' => 'textfield',
    '#title' => t('Archtics DB version'),
    '#description' => t('Ticketmaster Archtics DB version.  The individual commands may be different from version to version. e.g. V605'),
    '#default_value' => variable_get('tm_archtics_version_test', 'V605'),
    '#required' => TRUE,
  );
  $form['tmapi_test']['command'] = array(
    '#type' => 'fieldset',
    '#title' => t('Command1 Parameter'),
    '#weight' => 8,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['tmapi_test']['command']['tm_cmd_test'] = array(
    '#type' => 'textfield',
    '#title' => t('Ticketmaster Command'),
    '#description' => t('Always "customer_query"'),
    '#default_value' => variable_get('tm_cmd_test', 'customer_query'),
    '#required' => TRUE,
  );
  $form['tmapi_test']['command']['tm_ref_test'] = array(
    '#type' => 'textfield',
    '#title' => t('TM Reference attribute'),
    '#description' => t('Reference attribute; echoed to result.'),
    '#default_value' => variable_get('tm_ref_test', 'IOM_CUSTOMER_QUERY'),
    '#required' => TRUE,
  );
  $form['tmapi_test']['command']['tm_uid_test'] = array(
    '#type' => 'textfield',
    '#title' => t('TM: User ID'),
    '#description' => t('User ID (used on connection to DB). Uid will be provided by TM'),
    '#default_value' => variable_get('tm_uid_test', 'iomed05'),
    '#required' => TRUE,
  );
  $form['tmapi_test']['command']['tm_dsn_test'] = array(
    '#type' => 'textfield',
    '#title' => t('TM Data Source Name'),
    '#description' => t('Data Source Name (e.g. DB) to connect to'),
    '#default_value' => variable_get('tm_dsn_test', 'magic'),
    '#required' => TRUE,
  );
  $form['tmapi_test']['command']['tm_site_name_test'] = array(
    '#type' => 'textfield',
    '#title' => t('TM web-site name'),
    '#description' => t('Identifies the web-site the consumer is logging in from'),
    '#default_value' => variable_get('tm_site_name_test', 'mytruebluenation'),
    '#required' => TRUE,
  );
  
  // Other TM API
  $form['tmapi_other'] = array(
    '#type' => 'fieldset',
    '#title' => t('Other TM API Parameters'),
    '#weight' => 6,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $form['tmapi_other']['en_tm_invoice'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Invoice APIs on Login?'),
    '#default_value' => variable_get('en_tm_invoice', 0),
  );
  $form['tmapi_other']['en_tm_get_seats_on_account'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable get seats on Account APIs on login?'),
    '#default_value' => variable_get('en_tm_get_seats_on_account', 0),
  );
  $form['tmapi_other']['en_tm_get_seats_on_account_reserved'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable APIs to get seats on on Account Reserved?'),
    '#default_value' => variable_get('en_tm_get_seats_on_account_reserved', 0),
  );
   $form['tmapi_other']['en_all_reps_api'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable "all_reps" parameter for "get_acct_rep" TM APIs?'),
    '#default_value' => variable_get('en_all_reps_api', 0),
  );
  $form['tmapi_other']['all_rep_type_title'] = array(
    '#type' => 'textfield',
    '#title' => t('All rep type title'),
    '#description' => t('Please enter the rep type if all_rep paramaters are enabled.'),
    '#default_value' => variable_get('all_rep_type_title', ''),
    '#required' => FALSE,
    '#size' => 100,
  );
  return system_settings_form($form);
}

function custom_admin_sth_data_settings() {
  $form = array();
  $form['sth_handlingfee'] = array(
      '#type' => 'textfield',
      '#title' => t('Handling Fee'),
      '#description' => t('This value will appear on Invoice page under Handling Fee. Please enter the single word without space e.g. HAND or HANDLE or handling.'),
      '#default_value' => variable_get('sth_handlingfee', 'handling'),
  );
  $form['sth_full_season'] = array(
      '#type' => 'textfield',
      '#title' => t('Full Season Tickets'),
      '#description' => t('Full Season full season plan'),
      '#default_value' => variable_get('sth_full_season', '15FS'),
  );
  $form['sth_parking_full'] = array(
      '#type' => 'textfield',
      '#title' => t('Parking Fee'),
      '#description' => t('This value will appear on Invoice page under Handling Fee. Please enter the single word without space e.g. PARK or PRK or parking.'),
      '#default_value' => variable_get('sth_parking_full', 'parking'),
  );
  return system_settings_form($form);
}

function custom_admin_rsvp_settings() {
  $form = array();
  $form['en_rsvp'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable RSVP'),
    '#description' => t('Please clear cache after enabling/disabling this option.'),
    '#default_value' => variable_get('en_rsvp', 0),
  );
  $form['rsvp_title'] = array(
      '#type' => 'textfield',
      '#title' => t('RSVP Event Title'),
      '#description' => t('Please enter event title for RSVP.'),
      '#default_value' => variable_get('rsvp_title', ''),
      '#required' => TRUE,
  );
  return system_settings_form($form);
}

function custom_admin_birthday_takeover() {
  $form = array();
  $form['en_birthday'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Birthday Takeover'),
    '#description' => t('Please clear cache after enabling/disabling this option.'),
    '#default_value' => variable_get('en_birthday', 0),
  );
  return system_settings_form($form);
}

function custom_ga_form_setting(){
  $form = array();
  $form['ga'] = array(
    '#type' => 'fieldset',
    '#title' => t('Analytics'),
    '#weight' => 0,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['ga']['calendar'] = array(
    '#type' => 'textfield',
    '#title' => t('Calendar'),
    '#default_value' => variable_get('calendar','calendar'), 
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['ga']['my_calendar'] = array(
    '#type' => 'textfield',
    '#title' => t('My Calendar'),
    '#default_value' => variable_get('my_calendar','my-calendar'), 
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['ga']['full_calendar'] = array(
    '#type' => 'textfield',
    '#title' => t('Full Calendar'),
    '#default_value' => variable_get('full_calendar','full-calendar'), 
    '#weight' => 2,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  return system_settings_form($form);
}