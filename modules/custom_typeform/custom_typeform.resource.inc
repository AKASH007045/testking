<?php
/**
 * Type-form save data.
 * @param typeform_id
 */
function _custom_services_save_data($typeform_id) {
  module_load_include('inc', 'custom_typeform', 'custom_typeform.admin');
  $save_data = typeform_account_group_data($typeform_id, '');
  $status['submission'] = $save_data;
  return $status;
}