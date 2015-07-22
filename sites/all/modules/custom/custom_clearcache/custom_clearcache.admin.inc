<?php
/**
 * @file
 */

module_load_include('inc', 'system', 'system.admin');

function custom_clearcache_settings() 
{
  return system_settings_form($form);
}