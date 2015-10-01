<?php
/**
 * @file
 */


/**
 * Go to account manager form
 */
function custom_hooks_account_manager() {
  global $user;
  if (isset($user->uid) && $user->uid) {
    $name = $user->name;
  }
  else {
    $name = base64_decode($_SESSION['tmusn']);
    unset($_SESSION['tmusn']);
  }
  $dsn = variable_get('tm_dsn', '');
  $pass = isset($_SESSION['tmupa']) ? base64_decode($_SESSION['tmupa']) : '';
  $output = '<html><body onload="document.formLogin.submit()">
  <div class="section-preloader" style="width: 100%;
    height: 100%;
    background: url(../sites/all/themes/sports/images/loader_big.gif) center center no-repeat #eee;
    position: absolute;
    top: 0;
    left: 0;
    z-index: 99;">
  <form action="https://oss.ticketmaster.com/cgi/request.cgi" method="POST" name="formLogin" id="formLogin">
  <input type="hidden" name="team" value="' . $dsn . '">
  <input type="hidden" name="l" value="EN">
  <input type="hidden" name="STAGE" value="1">
  <input type="hidden" name="PROC" value="LOGIN">
  <input type="hidden" name="iAcctId" value="'. $name .'">
  <input type="hidden" name="iPIN" value="'. $pass .'"> 
  </form></div></body></html>';
  print $output;
  drupal_exit();
}