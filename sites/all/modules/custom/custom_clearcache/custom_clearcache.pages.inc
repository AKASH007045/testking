<?php
/**
 * @file
 */

/**
* function to Clear all Caches
*/
function custom_clearcache_page() 
{
   drupal_set_message(t("Cleared all cache!!!"), 'status');
   
   drupal_flush_all_caches();
   //drupal_goto('home');
}