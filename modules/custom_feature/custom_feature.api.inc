<?php
/**
 * @file
 */


/**
 * Go to account manager form
 */
function custom_feature_loyalty_points() {
 global $user;
 $acct_id = $user->name;
 $salt = 'kingslrs';
 $time = date('Y-m-d\TH:i:s\Z', time());
 $string = $time.$acct_id;
 $ref = hash_hmac('md5', $string, $salt);
 drupal_goto('http://sac-kings.skidataus.com/Login.aspx?account='.$acct_id.'&ref='.$ref.'&time='.$time.'');
}
