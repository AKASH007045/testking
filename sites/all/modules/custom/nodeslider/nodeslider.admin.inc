<?php

/**
 * @file
 * Administrative pages for nodeslider module.
 */

/**
 * Menu callback; display module settings form.
 */
function nodeslider_settings() {
  $form = array();

  // Set the id of the top-level form tag.
  $form['#id'] = 'paging';

  // General paging settings, not specific to node type.
  $form['paging_general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General paging settings'),
    '#collapsible' => FALSE,
  );

  // Paging separator string.
  // @TODO will need an upgrade path.
  $form['paging_general']['paging_separator'] = array(
    '#type' => 'textfield',
    '#title' => t('Page separator string'),
    '#size' => 20,
    '#maxlength' => 255,
    '#required' => TRUE,
    '#default_value' => variable_get('paging_separator', '<!--pagebreak-->'),
    '#description' => t('Use an HTML tag that will render reasonably when
      paging is not enabled, such as %pagebreak or %hr.', array('%pagebreak' => variable_get('paging_separator', '<!--pagebreak-->'), '%hr' => '<hr />')),
  );

  $paging_filter = FALSE;
  // Retrieve all input filters.
  foreach (filter_formats() as $format) {
    // Further retrieve all input formats.
    foreach (filter_list_format($format->format) as $filter) {
      // Check if any of the input formats have paging filter enabled.
      if ($filter->module == 'paging') {
        $paging_filter = TRUE;
        break;
      }
    }
  }
  if (!$paging_filter) {
    // Warn if paging filter is not yet enabled for any input format.
    drupal_set_message(t('Paging filter has not yet been enabled for any text
      formats. !link!', array('!link' => l(t('Enable now'), 'admin/config/content/formats'))), 'warning paging-warning');
  }

  // Get all valid fields.
  $fields = field_info_fields();
  $field_options = array();
  // Remove fields that are not on nodes.
  $valid_bundles = array('node'); // TODO make this work for other entity types?
  // Remove fields that are not lontext, or longtext and sumamry from the list.
  $valid_fields = array('text_long', 'text_with_summary');
  // Remove fields with multiple values.
  // @TODO
  foreach ($fields as $fieldname => $field) {
    $option = TRUE;
    if (!in_array($field['type'], $valid_fields)) {
      unset($fields[$fieldname]);
      $option = FALSE;
    }
    else {
      foreach ($valid_bundles as $bundle_name) {
        if (!array_key_exists($bundle_name, $field['bundles'])) {
          unset($fields[$fieldname]);
          $option = FALSE;
        }
      }
    }
    if ($option) {
      $field_options[$fieldname] = $fieldname;
    }
  }

  // Loop over all the available content types.
  foreach (node_type_get_types() as $type => $node_type) {
    $form[$type]['paging_config'] = array(
      '#type' => 'fieldset',
      '#title' => $node_type->name,
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#attributes' => array('id' => 'paging-type-' . $type, 'class' => array('paging-fieldset')),
    );

    // Left column fieldset.
    $form[$type]['paging_config']['paging_left'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#attributes' => array('class' => array('paging-left')),
    );

    // Paging toggle checkbox.
    $form[$type]['paging_config']['paging_left']['nodeslider_enabled_' . $type] = array(
      '#type' => 'checkbox',
      '#title' => 'Enable paging',
      '#default_value' => variable_get('nodeslider_enabled_' . $type, 0),
      '#attributes' => array('class' => array('paging-enabled')),
    );

    // Paging toggle checkbox.
    $form[$type]['paging_config']['paging_left']['paging_field_' . $type] = array(
      '#type' => 'radios',
      '#title' => 'Select field to use for page breaks',
      '#options' => $field_options,
      '#default_value' => variable_get('paging_field_' . $type, 0),
      '#attributes' => array('class' => array('paging-enabled')),
      '#states' => array(
        'visible' => array(// action to take.
          ':input[name="nodeslider_enabled_' . $type . '"]' => array('checked' => TRUE),
        ),
      ),
    );
    
  }

  $module_path = drupal_get_path('module', 'nodeslider');
  drupal_add_css($module_path . '/nodeslider.admin.css', 'module');

  return system_settings_form($form);
}